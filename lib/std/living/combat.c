/* stuff for the paranoia combat system... */
/* quick summary of the procedures defined in here
 * attack()            - called from heart_beat on monsters and players.
 * attack_by(object)   - Pretty obvious, we have been attacked by object
 * attack_ob(object)   - sets up the arrays for attacking object
 * stop_fight(object)  - stop fighting object
 * do_death()          - called when your hps reach 0.
 * check_stop_fight() is for internal use only ;)
 *
 * Ok guys, hit_player is obsolete...
 * The weapons themselves do all the damage and figure out how much to do
 * to the player. They then call adjust_hp(num) themselves...
 * Have fun etc.
 */
inherit "/std/weapon_logic";

static object attackee;
static object *attacker_list,
              *call_outed,
              weapon,
              protector;
static int concentrate, natural;

/* strange varible no one will ever understand...
 * Used as a level of the npc during the battle, so we odnt work it
 * out every time.
 */
static int att_level, dodging;
#define DIV_NUM 6
#define XP_DIVISION 1000
#include "money.h"

int query_dodging() { return dodging; }
int query_my_att_level() { return att_level; }
void set_my_att_level(int i) {att_level = i; }
void set_concentrate(int i) { concentrate = i; }
int query_concentrate() { return concentrate; }
object set_protector(object ob) { return (protector = ob); }
object query_protector() { return protector; }

void set_natural(int i) { natural = i; }
int query_natural() { return natural; }
void actual_death();

void create() {
  attacker_list = ({ });
  call_outed = ({ });
  weapon_logic::create();
} /* create() */

void combat_commands() {
  add_action("do_protect", "protect");
} /* combat_commands() */

/* this is called when you die ;) */
int do_death(object killed_by) {
  int i, xp, aln, tot;
  mapping damage_done;

/* stop fighting every one first... */
  damage_done = (mapping)this_object()->query_damage_done();
  if (!damage_done)
    damage_done = ([ ]);
  for (i=0;i<sizeof(attacker_list);i++) {
    attacker_list[i]->stop_fight(this_object());
    tot += damage_done[attacker_list[i]];
  }
  for (i=0;i<sizeof(call_outed);i++)
    call_outed[i]->stop_fight(this_object());
  xp = (int)this_object()->query_xp();
  if (!tot) tot = 1;
  aln = (int)this_object()->query_align();
  for (i=0;i<sizeof(attacker_list);i++) {
/*
 * calculate percantage of total xp they get based on how much damage
 * they did
 */
    attacker_list[i]->adjust_xp(((xp * damage_done[attacker_list[i]]/tot)
      * att_level) / (DIV_NUM *
      ((int)attacker_list[i]->query_my_att_level())));
    attacker_list[i]->adjust_align(aln);
  }
  if (killed_by) {
    say(killed_by->query_cap_name()+" dealt the death blow to "+
        this_object()->query_cap_name()+".\n");
    tell_object(killed_by,"You killed "+this_object()->query_cap_name()+".\n");
  }
  else
    say(this_object()->query_cap_name()+" died horribly.\n");
  if (environment())
    event(environment(), "death", attacker_list, killed_by);
  actual_death();
  return 0;
} /* do_death() */

object make_corpse() {
  object corpse,
         money,
         *arms,
         *ob;
  int i;
  corpse = clone_object("/obj/corpse");
  corpse->set_owner(this_object()->short(0), this_object());
  corpse->set_race_name(this_object()->query_race());
  corpse->start_decay();
  corpse->add_adjective(this_object()->query_adjectives());
  if (!this_object()->query_race_ob())
    if (file_size("/std/races/"+this_object()->query_race()+".c") >= 0)
      corpse->set_race_ob("/std/races/"+this_object()->query_race());
    else
      corpse->set_race_ob("/std/races/unknown");
  else
    corpse->set_race_ob(this_object()->query_race_ob());
  ob = all_inventory(this_object());

  /* this next bit is for formatting corpse inv - not combat :) */
  if(weapon) {
    if(random(2)) {
      tell_room(environment(), capitalize((string)weapon->query_short()) +
        " drops from " + (string)this_object()->query_cap_name() +
        "'s limp grasp.\n", this_object());
      tell_object(this_object(), "You limply release your " +
          capitalize((string)weapon->query_short()) + "...\n");
      ob -= ({ weapon });
      weapon->move(environment());
    } else {
      weapon->set_wield(corpse);
      corpse->set_weapon(weapon);
      /* moved with the other objects */
    }
  }
  arms = (object *)this_object()->query_armours();
  for (i=0;i<sizeof(ob);i++)
    ob[i]->move(corpse);
  corpse->set_armours(arms);
  for(i=0; i<sizeof(arms); i++)
    arms[i]->set_worn_by(corpse);

  corpse->move(environment());
  return corpse;
} /* make_corpse() */

/* shouldnt need to call this... so make it static, just in case */
static void actual_death() {
  object ob, ob2;
/* if second_life returns false... do stupid things... */
  if (!((int)this_object()->second_life())) {
    make_corpse()->move(environment());
/* dest the ones that stick around... */
    ob = first_inventory(this_object());
    while(ob) {
      ob2 = next_inventory(ob);
      ob->dest_me();
      ob = ob2;
    }
    this_object()->dest_me();
  }
} /* actual_death() */

object query_attackee() { return attackee; }

/* I will assume this is called in heart_beat... 
 * This could be used in both a paranoia mud or D&D style mud..
 * This will be called EVERY heart beat garanteed , that is when it is
 * running.... remember to get a garanteed amount of time use call_outs.
 * they are the most precise thing in mud... (which is not that hot)
 */
attack() {
  int i, dam, his_att_lvl;
  string his_name;
  object *obs, ob;

  if (!attacker_list)
   attacker_list = ({ });

  while (i<sizeof(attacker_list)) {
    if (!attacker_list[i]) /* have they died unexpecedaly? */
      attacker_list = delete(attacker_list,i,1);
    else
/*
 * check to make sure our environments are the same,
 * if not, updated them...
 * Hmmmm, Ok I will put all the ones not in the correct environ in a new
 * list.
 */
      if (environment(attacker_list[i]) != environment()) {
        object blue;

        write("You are hunting "+attacker_list[i]->short()+"\n");
        call_out("check_stop_fight",600,attacker_list[i]);
        if (!call_outed)
          call_outed = ({ attacker_list[i] });
        else
          call_outed += ({ attacker_list[i] });
        blue = attacker_list[i];
        attacker_list = delete(attacker_list,i,1);
        event(environment(), "stopped_fighting", blue);
      } else
        i++;
  }
/* check the not correct environ list to see if any one has come back.  */
  i = 0;
  while (i<sizeof(call_outed))
    if (!call_outed[i])
      call_outed = delete(call_outed,i,1);
    else if (environment() == environment(call_outed[i])) {
      attacker_list += ({ call_outed[i] });
      call_outed = delete(call_outed,i,1);
    } else
      i++;
/* not hitting anyone... do spell effects anyway... */
  if (!sizeof(attacker_list)) {
/* Do the spell effects as a seperate call in the heart_beat now.
    this_object()->do_spell_effects(0);
 */
    attackee = 0;
    return ;
  }
/*
 * choose a random person from the attacker list, as they all must be in
 * correct environment...
 */
  if (concentrate)
    attackee = attacker_list[0];
  else
    attackee = attacker_list[random(sizeof(attacker_list))];
/*
 * Check to see if they are being protected.
 * Should we use a skill here?  Hmm....  To slow probably, will make it
 * a fixed 80% chance for now.  First check to make sure that the
 * protector isnt in the attacker list
 */
  if (protector && member_array(protector, attacker_list) != -1) {
    tell_object(this_object(), protector->query_cap_name()+
                " turns against you and starts fighting you.\n");
    tell_object(protector, "You start fighting "+
                this_object()->query_cap_name()+" and stop protecting "+
                "them.\n");
    say(protector->query_cap_name()+" turns against "+
        this_object()->query_cap_name()+" and starts attacking him.\n",
        ({ protector, this_object() }));
  }
  if ((ob = (object)attackee->query_protector()) &&
      environment(ob) == environment(attackee) && random(100) >= 20) {
    tell_object(attackee, ob->query_cap_name()+
                " valiantly protects you.\n");
    tell_object(ob, "You valiantly protect "+attackee->query_cap_name()+".\n");
    say(ob->query_cap_name()+" valiantly protects "+
        attackee->query_cap_name()+".\n", ({ attackee, ob }));
    attackee = ob;
  }
  his_att_lvl = (int)attackee->query_my_att_level();
  his_name = (string)attackee->query_name();
/* if no spells to cast - do normal hitting */
/* Done in the heart_beat....
 * This was a sleeze so monsters didn't attack whilst casting.  They
 * Should not now anyway due to time effects.
  if(!this_object()->do_spell_effects(attackee)) {
 */
    attackee->attack_by(this_object());
/*
 * Ok, do the damage to the person we are attacking.  This has already
 * been wombled to get the attackee to be the right person (ie being
 * protected).
 */
    if (weapon)
      dam = (int)weapon->weapon_attack(attackee,this_object());
    if (!weapon || natural)
      dam = weapon_attack(attackee,this_object());
/*
  }
 */
  if (!attackee) {
/* This wont normally happen, cause death happens in a call_out(0) 
from the target file. Wonder if it ever will happen...hmmm
maybe if the weapon itself dests the monster? */
    tell_object(this_object(),"You killed "+his_name+"\n");
    say(this_object()->query_cap_name()+" killed "+his_name+"\n");
  }
  if (!att_level)
    att_level = 1;
  this_object()->adjust_xp(dam*his_att_lvl/att_level);
  if (environment())
    event(environment(), "fight_in_progress", attackee);
} /* attack() */

void attack_by(object ob) {
  if (ob == this_object())
    return ;
  if(userp(ob) && userp(this_object())) return;
/* One of 2 PK disablers */
  if (!attacker_list)
    attacker_list = ({ });
  if (!sizeof(attacker_list)) {
    att_level = (int)this_object()->query_level()+15;
    att_level += (int)this_object()->query_xp()/XP_DIVISION;
    if (att_level <= 0)
      att_level = 1;
    dodging = (int)this_object()->query_skill_bonus("fighting.combat.dodging.melee");
  }
  if (member_array(ob, attacker_list) == -1 &&
      member_array(ob, call_outed) == -1) {
    attacker_list += ({ ob });
  }
  set_heart_beat(1);
} /* attack_by() */

void attack_ob(object ob) {
  if (ob == this_object())
    return ;
  if(userp(ob) && userp(this_object())) return ;
/* This is one of 2 PK disablers. */
  if (!attacker_list)
    attacker_list = ({ });
  if (!sizeof(attacker_list)) {
    att_level = (int)this_object()->query_level()+15;
    att_level += (int)this_object()->query_xp()/XP_DIVISION;
    if (att_level <= 0)
      att_level = 1;
    dodging = (int)this_object()->query_skill_bonus("fighting.combat.dodging.melee");
  }
  if (member_array(ob, attacker_list) == -1 &&
      member_array(ob, call_outed) == -1) {
    attacker_list += ({ ob });
  }
  if (ob)
    ob->attack_by(this_object());
} /* attack_ob() */

void check_stop_fight(object ob) {
  int i;

  if ((i=member_array(ob,call_outed)) != -1)
    call_outed = delete(call_outed,i,1);
} /* check_stop_fight() */

int stop_fight(object ob) {
  int i;
  if ((i=member_array(ob,call_outed)) != -1) {
    tell_object(this_object(), "You stop hunting "+call_outed[i]->short()+".\n");
    call_outed = delete(call_outed, i, 1);
  } else {
    event(environment(), "stopped_fighting", ob);
  }
  if ((i=member_array(ob,attacker_list)) != -1)
    attacker_list = delete(attacker_list,i,1);
} /* stop_fight() */

mixed *query_attacker_list() { return attacker_list; }
mixed *query_call_outed() { return call_outed; }

int wield(string str, int none) {
  int i;
  object *obs;
  mapping failed;
  mixed tmp;
  string mess, *bits;

  failed = ([ ]);
  if (!str) {
    notify_fail("Usage: wield <weapon>\n");
    return 0;
  }
  if(str == "hands") {
    if(weapon)
    {
      write("You lower your " + (string)weapon->short() + ".\n");
      weapon->set_wield(0);
    }
    write("You are now ready for hand to hand combat.\n");
    weapon = 0;
    return 1;
  }
  obs = find_match(str, this_object());
  if (!sizeof(obs)) {
    notify_fail("Cannot find "+str+".\n");
    return 0;
  }
  for (i=0;i<sizeof(obs);i++)
    if (obs[i]->query_weapon()) {
      if (weapon && (!(tmp = (mixed)weapon->set_wield(0)) || stringp(tmp))) {
        if (!stringp(tmp))
          mess = "You cannot wield $N.\n";
        else
          mess = tmp;
        if (!failed[mess])
          failed[mess] = ({ obs[i] });
        else
          failed[mess] += ({ obs[i] });
        continue;
      }
      if (!(tmp = obs[i]->set_wield(this_object()))) {
        mess = "$N is not a weapon.\n";
        if (!failed[mess])
          failed[mess] = ({ obs[i] });
        else
          failed[mess] += ({ obs[i] });
        continue;
      }
      if (stringp(tmp)) {
        mess = tmp;
        if (!failed[mess])
          failed[mess] = ({ obs[i] });
        else
          failed[mess] += ({ obs[i] });
        continue;
      }
      weapon = obs[i];
      if (this_object() != this_player()) return 1;
/* Need to print the failures first... */
      bits = keys(failed);
      for (i=0;i<sizeof(bits);i++)
        write(replace(bits[i], "$N", query_multiple_short(failed[bits[i]])));
      if (!none) {
        say(this_object()->query_cap_name()+" wields "+
            weapon->query_short()+".\n");
        if (this_player() == this_object()) write("You wield "+
          weapon->query_short()+".\n");
      }
      return 1;
    }
  mess = "";
  bits = keys(failed);
  for (i=0;i<sizeof(bits);i++)
    mess += replace(bits[i], "$N", query_multiple_short(failed[bits[i]]));
  notify_fail(mess);
  return 0;
} /* wield() */

int unwield_weapon(object ob) {
  if (ob == weapon) {
    weapon = 0;
    return 1;
  }
  return 0;
} /* unwield_weapon() */

object query_weapon() { return weapon; }
void set_weapon(object weap) { weapon = weap; }

/*
 * Sets up protection matrixes.
 */
int do_protect(string str) {
  object *obs,
         *ok;
  int i;

  if (!str) {
    notify_fail("Syntax: protect <objects>\n");
    return 0;
  }
  obs = find_match(str, environment());
  if(sizeof(obs)) obs -= ({ this_player() });
  if (!sizeof(obs)) {
    notify_fail("Protect what?\n");
    return 0;
  }
  ok = ({ });
  for (i=0;i<sizeof(obs);i++)
    if (obs[i]->set_protector(this_object()))
      ok += ({ obs[i] });
  if (!sizeof(ok)) {
    notify_fail(query_multiple_short(obs)+
                (sizeof(obs)==1?" doesnt":" don't")+" want your protection.\n");
    return 0;
  }
  if (sizeof(obs) != sizeof(ok))
    write("You fail to protect "+query_multiple_short(obs - ok)+".\n");
  write("You protect "+query_multiple_short(ok)+".\n");
  say(this_object()->query_cap_name()+" protects "+query_multiple_short(ok)+
      ".\n", ok);
  for (i=0;i<sizeof(ok);i++)
    tell_object(ok[i], this_object()->query_cap_name()+" protects "+
                query_multiple_short(({ "you" }) + (ok - ok[i..i]))+".\n");
  return 1;
} /* do_protect() */

/* the below line is used by weapon_logic to see how much damage the
 * weapon does.  Is useed like this so that contition and other strange
 * things can be taken into account ;)
 */
int modify_damage(int i) { return i; }

mixed stats() {
  mixed fish;

  if(weapon)
    return ({ ({ "Weapon", weapon->short() }) }) +
      (mixed)weapon->weapon_stats();
  else
    return weapon_logic::stats();
} /* stats() */
