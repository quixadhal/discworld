/*  -*- LPC -*-  */
/**
 * The Good Fighter shadow.  The main docs are stored in the effect
 * header file, not here...  Mostly for standardisation reasons.
 *
 * @see /std/effects/npc/good_fighter
 *
 * @author Sin
 * @created 12 November 1997
 * @changed 13 November 1997 -- Sin
 *     Converted it from a pure shadow to a shadow/effect pair
 * @changed 11 April 1998 -- Sin
 *     Added support for USE_UNARMED.
 *     God rid of some dead wood.
 * @changed 12 April 1998 -- Sin
 *     Added support for crush
 * @changed 3 August 2002 -- Rhinehold
 *     Converted to work with new non-effect combat system
 *     Added support for behead, pierce, bash, smash, chop
 *     Removed strike
 */

#include <good_fighter.h>

#define LOGFILE "good_fighter"
#define CMDS "/cmds/guild-race/fighting/"

#define DEBUG

inherit "/std/effect_shadow";

//                     "riposte" : "riposte $N with $W", \
#define W_ATTACKS ([ "crush"   : "crush $N with $W", \
                     "behead"  : "behead $N with $W", \
                     "impale"  : "impale $N with $W", \
                     "stab"    : "stab $N with $W", \
                     "pierce"  : "pierce $N with $W", \
                     "bash"    : "bash $N with $W", \
                     "smash"   : "smash $N with $W", \
                     "slash"   : "slash $N with $W", \
                     "slice"   : "slice $N with $W", \
                     "hack"    : "hack $N with $W", \
                     "chop"    : "chop $N with $W", \
                     "feint"   : "feint at $N with $W" ])
#define U_ATTACKS ([ "trip"    : "trip $N", \
                     "shove"   : "shove $N" ])
                   
string *specials;
mapping weapons;
int bluntorsharp;
int dodgeorparry;
object victim;

/**
 * @ignore
 * Used only to insure that the specials array is actually an array at
 * all times.
 */
void create()
{
  specials = ({ });
  weapons = ([ ]);
}

/**
 * This is a helper function used by the good_fighter_setup() function to
 * ensure that the specified skill is at least at a particular level.
 *
 * @param skill The skill to check
 *
 * @param level The minimum acceptable level for this skill
 */
protected void check_skill(string skill, int level)
{
  int cur;

  cur = player->query_skill(skill);
  if (cur < level)
    player->add_skill_level(skill, level - cur);
}

/**
 * This helper function is used by good_fighter_setup() to add a known
 * command to the NPC's repertoire _only_ if the relevant skill is above
 * a certain level.  If the command is added, then it is also stored in the
 * specials[] array for later use by fight_in_progress()
 *
 * @param command The command to be added
 *
 * @param skill The skill that controls the NPC's effectiveness with this
 * command.
 *
 * @param level The minimum level before the command can be added.
 */
protected void check_add_command(string command, string skill, int level)
{
  int cur;

  cur = player->query_skill(skill);
  if (cur >= level) {
    player->add_known_command(command);
    specials += ({ command });
  }
}

/**
 * This function is called from a callout() registered when setup_shadow()
 * is called.  It is responsible for ensuring that all of the NPC's skills
 * are at a reasonable level, that the NPC has the commands appropriate
 * for its priorities and level, and that the tactics are set appropriately.
 */
void good_fighter_setup()
{
  int adjust;
  int lvl;
  int *args = arg();

  if (!args || !arrayp(args) || sizeof(args) != 2)
    return;

  specials = ({ });
  bluntorsharp = args[0];
  dodgeorparry = args[1];

  lvl = player->query_level();
  check_skill("other.perception", lvl / 2);
  check_skill("other.health", lvl);
  adjust += (lvl / 3);
  if (bluntorsharp == USE_PIERCE) {
    check_skill("fighting.combat.melee.blunt", lvl - adjust);
    check_skill("fighting.combat.melee.sharp", lvl - adjust);
    check_skill("fighting.combat.melee.pierce", lvl + adjust);
    check_skill("fighting.combat.melee.unarmed", lvl - adjust);
  } else if (bluntorsharp == USE_BLUNT) {
    check_skill("fighting.combat.melee.blunt", lvl + adjust);
    check_skill("fighting.combat.melee.sharp", lvl - adjust);
    check_skill("fighting.combat.melee.pierce", lvl - adjust);
    check_skill("fighting.combat.melee.unarmed", lvl - adjust);
  } else if (bluntorsharp == USE_SHARP) {
    check_skill("fighting.combat.melee.blunt", lvl - adjust);
    check_skill("fighting.combat.melee.sharp", lvl + adjust);
    check_skill("fighting.combat.melee.pierce", lvl - adjust);
    check_skill("fighting.combat.melee.unarmed", lvl - adjust);
  } else if (bluntorsharp == USE_UNARMED) {
    check_skill("fighting.combat.melee.blunt", lvl - adjust);
    check_skill("fighting.combat.melee.sharp", lvl - adjust);
    check_skill("fighting.combat.melee.pierce", lvl - adjust);
    check_skill("fighting.combat.melee.unarmed", lvl + adjust);
  } else {
    check_skill("fighting.combat.melee.blunt", lvl);
    check_skill("fighting.combat.melee.sharp", lvl);
    check_skill("fighting.combat.melee.pierce", lvl);
    check_skill("fighting.combat.melee.unarmed", lvl);
  }

  adjust = lvl * 2 / 3;
  if (dodgeorparry == DEFEND_DODGE) {
    check_skill("fighting.combat.dodging", lvl + adjust);
    check_skill("fighting.combat.parry", lvl - adjust);
    player->init_command("tactics response dodge", 1);
  } else if (dodgeorparry == DEFEND_PARRY) {
    check_skill("fighting.combat.dodging", lvl - adjust);
    check_skill("fighting.combat.parry", lvl + adjust);
    player->init_command("tactics response parry", 1);
  } else {
    check_skill("fighting.combat.dodging", lvl);
    check_skill("fighting.combat.parry", lvl);
    player->init_command("tactics response neutral", 1);
  }
  check_skill("fighting.combat.special", lvl / 2);
  check_skill("fighting.points", lvl * 2);

  player->init_command("tactics attitude offensive", 1);

  if (bluntorsharp != USE_BLUNT) {
    if (dodgeorparry == DEFEND_PARRY)
      check_add_command("riposte", "fighting.combat.special", 15);
    if (bluntorsharp == USE_SHARP || bluntorsharp == USE_BALANCED)  {
      check_add_command("hack", "fighting.combat.special", 15);
      check_add_command("slash", "fighting.combat.special", 15);
      check_add_command("slice", "fighting.combat.special", 75);
      check_add_command("chop", "fighting.combat.special", 75);
      if (player->query_guild_ob() == "/std/guilds/warrior")
        check_add_command("behead", "fighting.combat.special", 150);
    }
    if (bluntorsharp == USE_PIERCE) {
      check_add_command("stab", "fighting.combat.special", 15);
      check_add_command("pierce", "fighting.combat.special", 75);
      if (player->query_guild_ob() == "/std/guilds/warrior")
        check_add_command("impale", "fighting.combat.special", 150);
    }
  } else {
    check_add_command("bash", "fighting.combat.special", 15);
    check_add_command("smash", "fighting.combat.special", 75);
    if (player->query_guild_ob() == "/std/guilds/warrior")
      check_add_command("crush", "fighting.combat.special", 150);
  }
  check_add_command("feint", "fighting.combat.special", 15);
  check_add_command("trip", "fighting.combat.special", 15);
  check_add_command("shove", "fighting.combat.special", 15);
  player->add_known_command("concentrate");
}

/**
 * This helper function is used by fight_in_progress() to see if one of
 * the NPC's weapons are appropriate for use with the command.
 *
 * @param weapon The object to check.
 *
 * @return 1 if the object is appropriate, 0 otherwise
 */
protected int check_weapon(object weapon, string command) {

  if(!weapon->query_attack_names())
    return 0;

  switch(command) {
  case "crush":
    command = "smash";
    break;
  case "behead":
    command = "slice";
    break;
  case "impale":
    command = "impale";
    break;
  case "riposte":
    command = "slash";
    break;
  case "feint":
    return 1;
  }

  return (member_array(command, weapon->query_attack_names()) != -1);
}

/**
 * This function is called once per combat round by the combat shadow,
 * and does most of the work for this shadow.  It controls on whom
 * the NPC is concentrating, chooses an appropriate attack, and chooses
 * an appropriate weapon for the attack.
 *
 * <p>Most of the intelligence is in the specific choice of attack, and
 * there isn't a whole lot in that.  It restricts itself to simply going
 * through the attacks stored in the specials[] array (calculated in
 * the call to good_fighter_setup()), finding which attacks are valid (based
 * on whether the opponent is holding a weapon and which weapons are wielded
 * by the NPC).  Once it has a list of valid commands, it choses from them
 * randomly.
 *
 * <p>Regardless, there is always the possibility that this function will
 * drop through and allow the combat handler to attack normally.  That
 * chance is inversely proportional to the NPC's level.
 *
 * @param attacker The current person attacking the NPC.
 */
void event_fight_in_progress(object attacker, object opponent)
{
  object ts, weapon;
  int chance;
  string cmd;

  /* I have to get to the top of the shadow stack, because the combat
   * shadow was probably added after this one. */
  ts = player;
  ts->event_fight_in_progress(attacker, opponent);

  // Make sure we have some GPs.
  if(ts->query_specific_gp("fighting") < 50)
    return;
  
  if (victim && !objectp(victim))
    victim = 0;
  if (victim && !interactive(victim))
    victim = 0;
  if (victim && environment(victim) != environment(player))
    victim = 0;
  if (victim && victim != attacker)
    return;
  if (!victim) {
    // Check the attacker is someone we're fighting
    if(member_array(attacker, ts->query_attacker_list()) != -1)
       victim = attacker;
       
    if (!victim)
      return;
  }

  // If they've been disarmed set them to wimp mode and dodge response.
  if(!sizeof(player->query_weapons()) &&
     player->query_combat_attitude() != "wimp" &&
     arg()[0] != USE_UNARMED) {
    object *shields;

    // No weapons -- go wimp.
    player->queue_command("tactics attitude wimp", 1);
    
    shields = filter(player->query_holding(), (: $1 && $1->id("shield") :));
    // No shield? Then it's unarmed parry or dodge.
    if(!sizeof(shields)) {
      if(player->query_race_ob() == "/std/races/troll") {
        if(!player->query_unarmed_parry())
          player->queue_command("tactics parry unarmed");
      } else {
        player->queue_command("tactics response dodge", 2);
      }
    }
  }

  /*
   * As the NPC gets higher level, there should be more possiblity of
   * it using a special command.  But on the other hand, it mustn't
   * ever get to the point that it will _always_ use special commands.
   * I have fixed the upper limit at a 25% chance of using a special
   * command, and that is reached at level 300.
   * Right now the probability curve is linear.  It should really
   * be a sigmoid.
   */
  chance = (player->query_level() / 12) + 5;
  if(chance > 25)
    chance = 25;

  if(sizeof(player->query_specials()))
    chance /= (sizeof(player->query_specials()) * 10);

  if(random(100) + 1 < chance) {
    string *candidates;
    int i;

    candidates = ({ });
    foreach(weapon in player->query_weapons()) {
      // Setup an array of the attack possibilities for each weapon so
      // we don't have to recalc them everytime.
      if(!weapons[weapon]) {
        weapons[weapon] = ({ });
        for (i = 0; i < sizeof(specials); i++)
          if(W_ATTACKS[specials[i]] && check_weapon(weapon, specials[i])) {
            weapons[weapon] += ({ replace(W_ATTACKS[specials[i]], "$W",
                                          weapon->query_short()) });
          }
      }

      // Add held weapons to the candidates list
      candidates += weapons[weapon];
    }

    // Setup unarmed attacks.
    if(!weapons["unarmed"]) {
      weapons["unarmed"] = ({ });
      for(i=0; i<sizeof(specials); i++)
        if(U_ATTACKS[specials[i]])
          weapons["unarmed"] += ({ U_ATTACKS[specials[i]] });
    }

    // add unarmed attacks to the candidates list
    candidates += weapons["unarmed"];
    
#ifdef DEBUG
    //    debug_printf("Commands available: %O", candidates);
#endif
    if(sizeof(candidates)) {
      cmd = replace(candidates[random(sizeof(candidates))], "$N",
                    victim->query_name());

#ifdef DEBUG
      debug_printf("Trying to perform: %O", cmd);
#endif
      player->do_command(cmd, 1);
    } else {
#ifdef DEBUG
      debug_printf("No command to perform");
#endif
    }
  }
}

/**
 * When a creator uses the 'stat' command on an NPC that is shadowed
 * by this object, this function gets called.  It returns an array
 * containing the normal stats for tho object that this is shadowing,
 * plus it tacks on some information regarding the configuration of
 * this shadow.
 *
 * @return A list of stats related to this shadow, augmented by
 * the stats of the NPC that this shadow is attached to (if any).
 */
mixed *stats()
{
  if (!player || !objectp(player))
    return ({ ({ "good fighter", "unattached" }) });
  return player->stats() + ({
    ({ "good fighter",
      ((bluntorsharp == USE_BLUNT) ? "blunt" :
        ((bluntorsharp == USE_PIERCE) ? "pierce" :
          ((bluntorsharp == USE_SHARP) ? "sharp" : "balanced"))) +
      ((dodgeorparry == DEFEND_DODGE) ? " dodger" :
        ((dodgeorparry == DEFEND_PARRY) ? " parrier" : " fighter")) }),
    ({ "managed commands", implode(specials, ", ") })
  });
}
