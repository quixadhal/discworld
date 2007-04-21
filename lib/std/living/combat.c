/**
 * This file contains all the methods and fun things actually run combat.
 * @author Ceres
 *
 *
 * TODO: Add offensive & defensive criticals -- broken limbs etc.
 *
 */
inherit "/std/living/corpse";
inherit "/std/weapon_logic";

#include <attack_messages.h>
#include <tasks.h>
#include <player.h>
#include <living.h>
#include <weapon.h>
#ifdef COMBAT_SPEED
#undef COMBAT_SPEED
#endif
#include <combat.h>

#ifdef ABS
#undef ABS
#endif
#define ABS(n) (n) < 0 ? -(n) : (n)

// 2 == verbose, 1 == brief, 0 == none.
#define DEBUG 1
#undef USE_SURRENDER

class tactics _tactics;

#ifdef USE_SURRENDER
class surrender_information _surrender;
#endif

nosave class combat_information _combat;
nosave string tmpzone; // needed for function pointer.
nosave int _callout;

object *query_attacker_list();
void stop_all_fight();
int query_distance(object);
int set_distance(object, int);
void adjust_action_defecit(int amount);
private void init_tactics();
#ifdef USE_SURRENDER
object *query_surrenderers();
#endif

/** @ignore yes */
void create() {
  ::create();

  init_tactics();
  _combat = new(class combat_information,
                in_combat : 0,
                protectors : ({ }),
                defenders : ({ }),
                attacker_list : ([ ]),
                hunting_list : ([ ]),
                specials : ({ })
                );

#ifdef USE_SURRENDER
  _surrender = new(class surrender_information,
                   to : ({ }),
                   from : ({ }),
                   refusers : ({ }));
#endif
}

private void init_tactics() {
  if(!classp(_tactics) || sizeof(_tactics) != 8) {
    _tactics = new(class tactics,
                   attitude : "neutral",
                   response : "neutral",
                   parry : "both",
                   attack : "both",
                   parry_unarmed : 0,
                   mercy : "ask",
                   focus_zone : 0,
                   ideal_distance : 0);
  }
}

#ifdef DEBUG
class combat_information query_combat() { return _combat; }
#ifdef USE_SURRENDER
class surrender_information query_surrender() { return _surrender; }
#endif
#endif

/** @ignore yes */
void combat_debug(string fmt, mixed args ...) {
  if(userp(this_object()))
    this_object()->event_inform(this_object(),
                                sprintf("%s " + fmt,
                                        this_object()->query_name(), args ...),
                                      "combat-debug", this_object());
  event(environment(this_object()), "inform",
        sprintf("%s " + fmt, this_object()->query_name(), args ...),
        "combat-debug", this_object());
}

/**
 * Decide if we will attack this round or not.  This function can be
 * overloaded if necessary. It also checks for the property "cannot attack"
 * on this object. If the property is set the object cannot attack.
 *
 * @return 1 if we want to, 0 if we don't.
 */
int query_can_attack() {
  if(this_object()->query_property(PASSED_OUT) ||
     this_object()->query_property("dead") ||
     this_object()->query_auto_loading() ||
     this_object()->query_hp() < 0 ||
     this_object()->query_casting_spell() ||
     (userp(this_object()) && !interactive(this_object())) ||
     this_object()->query_property("cannot attack"))
    return 0;

  return 1;
}

/**
 * Decide if we are able to defend ourselves this time or not.
 * This function can be overloaded if necessary. It also checks for the
 * property "cannot defend"
 * on this object. If the property is set the object cannot defend.
 *
 * @return 1 if we want to, 0 if we don't.
 */
int query_can_defend() {
  if(this_object()->query_property(PASSED_OUT) ||
     this_object()->query_property("dead") ||
     this_object()->query_auto_loading() ||
     this_object()->query_hp() < 0 ||
     this_object()->query_casting_spell() ||
     (userp(this_object()) && !interactive(this_object())) ||
     this_object()->query_property("cannot defend"))
    return 0;

  return 1;
}

/**
 * Choose an opponent and attack them.
 *
 * This method must fill in the opponent variable. If no opponent is chosen
 * the attack will be aborted.
 *
 * PRE: Attacker information has been filled in.
 * POST: opponent must be chosen or the attack will be aborted.
 *
 * @param class attack
 * @return class attack
 */
protected class attack choose_opponent(class attack att) {
  mixed *opponents, opponent;
  int i;
  
#ifdef USE_DISTANCE
  mixed tmp;
#endif

  // TODO: More advanced opponent selection.
  opponents = map(keys(_combat->attacker_list),
                  (: $1 && objectp($1) ? $1 : find_player($1) :));
  opponents = filter(opponents, (: $1 && environment($1) == environment($2) &&
                                 $1->query_attackable() :), att->attacker);
  
#ifdef USE_SURRENDER
  // Remove surrendered people from the opponent list so we don't kill
  // anyone during the surrender.
  foreach(opponent in query_surrenderers()) {
    if(userp(opponent))
      opponent = opponent->query_name();
    opponents -= ({ opponent });
  }
#endif

  // we pick the closest opponent.
  if(!sizeof(opponents))
    return att;

  // Attack whoever we're concentrating on.
  if(interactive(this_object()) &&
     objectp(att->attacker_concentrating) && 
     member_array(att->attacker_concentrating, opponents) != -1)
    opponent = att->attacker_concentrating;
  else {
#ifdef USE_DISTANCE
    opponent = opponents[0];
    foreach(tmp in opponents)
      if(_combat->attacker_list[tmp] < _combat->attacker_list[opponent])
        opponent = tmp;
#endif

    // Three different NPC algorithms. good_fighter should use better
    // ones.
    switch(random(3)) {
    case 2:
      opponent = opponents[0];
      for(i=0; i<sizeof(opponents); i++)
        if((opponent->query_hp() / 10 + random(100)) >
           (opponents[i]->query_hp() / 10 + random(100)))
          opponent = opponents[i];
      break;
    case 1:
      opponent = opponents[0];
      for(i=0; i<sizeof(opponents); i++)
        if((opponent->query_hp() / 10 + random(100)) <
           (opponents[i]->query_hp() / 10 + random(100)))
          opponent = opponents[i];
      break;
    default:
      opponent = opponents[random(sizeof(opponents))];
    }
  }

#ifdef USE_DISTANCE
  // Set the opponent and distance.
  att->distance = _combat->attacker_list[opponent];
#endif

  if(stringp(opponent))
    att->opponent = find_player(opponent);
  else
    att->opponent = opponent;

  // Just in case they disappeared somehow.
  if(environment(att->opponent) != environment(this_object()))
    return att;

  // Automatically concentrate on someone if they're the only opponent.
  if(sizeof(opponents) == 1)
    _combat->concentrating = att->opponent;

  return att;
}

/** @ignore yes */
void clear_protection(object attacker, object protector) {
  if(attacker->remove_protector(protector))
    protector->event_combat(protector,
                            "You stop protecting "+ attacker->the_short() +
                            " as "+ attacker->query_pronoun() +
                            " moves to attack "+
                            "you!\n", ({ }), 0);
  if(attacker->remove_defender(protector))
    protector->event_combat(protector, "You stop defending "+
                            attacker->the_short() + " as "+
                            attacker->query_pronoun() +" moves to attack "+
                            "you!\n", ({ }), 0);
}

/**
 * This method is used to choose who will defend this attack. Typically this
 * will be the person who was attacked, however it could be someone else in
 * some circumstances.
 *
 * PRE: attacker, attack and defender information has been set.
 * POST: defender and person_hit must be set.
 *
 * @param class attack
 * @return class attack
 */
class attack choose_defender(class attack att) {
  object *protectors, *defenders;

  // If the combat system (or a special) has already chosen a protector
  // we won't override it.
  if(!att->person_hit) {
    // People who will protect (ie. jump infront of an attack)
    protectors = filter((att->opponent)->query_protectors(),
                        (: $1 && environment($1) == environment($2) &&
                         $1->query_protect() && !pk_check($1, $2) :),
                        att->attacker);
    
    if(sizeof(protectors)) {
      protectors->attack_by(att->attacker);
      
      att->person_hit = protectors[random(sizeof(protectors))];
#if DEBUG == 1
      combat_debug("picked %s to protect", (att->person_hit)->query_name());
#endif
    }
  }
  
  // If the combat system (or a special) has already chosen a defender
  // we won't override it.
  if(!att->defender) {
    defenders = filter((att->opponent)->query_defenders(),
                       (: $1 && environment($1) == environment($2) &&
                        !pk_check($1, $2) :), att->attacker);

    // Make sure local defenders join in the fight.
    if(sizeof(defenders)) {
      defenders->attack_by(att->attacker);
      defenders = filter(defenders, (: $1->query_defend() :));
    }

    // See if we have someone to defend.
    if(sizeof(defenders)) {
      att->defender = defenders[random(sizeof(defenders))];
#if DEBUG == 1
      combat_debug("picked %s to defend", (att->defender)->query_name());
#endif
    }
  }
  
  // Do the defaults if we haven't been protected or defended.
  if(!att->person_hit)
    att->person_hit = att->opponent;
  if(!att->defender)
    att->defender = att->opponent;
  
  return att;
}

/**
 * Is this object attackable or not?
 *
 * @return 1 or 0 for true or false.
 */
int query_attackable() {
  if(this_object()->query_property(PASSED_OUT) ||
     this_object()->dont_attack_me() ||
     this_object()->attack_inhibit() ||
     this_object()->query_property("dead") ||
     this_object()->query_auto_loading() ||
     this_object()->query_hp() < 0 ||
     (userp(this_object()) && !interactive(this_object())))
    return 0;
  return 1;
}

/**
 * Is this object prepared to try to defend someone else at this time?
 *
 * @return 1 or 0 for true or false.
 */
int query_defend() {
  if(!query_attackable() || this_object()->query_casting_spell() ||
     this_object()->query_specific_gp("fighting") < 1)
    return 0;
  return ((_tactics->response == "parry" || _tactics->response == "both") &&
          _combat->action_defecit < (COMBAT_ACTION_TIME * 4));
}

/**
 * Is this object prepared to try to protect someone else at this time?
 * @return 1 or 0 for true or false.
 */
int query_protect() {
  if(!query_attackable() || this_object()->query_casting_spell() ||
     this_object()->query_specific_gp("fighting") < 1)
    return 0;
  return (_combat->action_defecit < (COMBAT_ACTION_TIME * 4));
}


/**
 * Determine which attack they'll use.
 *
 * This picks a weapon, gets the attack data and the weapon skill and
 * determines how much the attack will cost.
 *
 * PRE: attacker info, opponent and defender info have been set.
 * POST: attack weapon, attack skill, attack data, and attack_cost must
 * be set.
 *
 * @param class attack
 * @return class attack
 */
protected class attack choose_attack(class attack att) {
  object weapon, *weapons;
  string hand, *tmp;
  int perc, which_attack, i;
  class tactics tactics;
  mixed *attacks;
  string zone;
  float sz;

  tactics = att->attacker_tactics;

  if(!this_object()->query_can_attack())
    return att;

  // Depending on their tactics they will go further into defecit in order
  // to attack.
  if(!OFFENSIVE_DEFECITS[tactics->attitude])
    tactics->attitude = "neutral";

  if(att->attacker_defecit > OFFENSIVE_DEFECITS[tactics->attitude])
    return att;

  weapons = (att->attacker)->query_holding();
  tmp = (att->attacker)->query_limbs();

  // Do they have a preferred attacking hand?
  hand = (att->attacker_tactics)->attack;
  if(hand && hand != "both") {
    for(i=0; i<sizeof(tmp); i++)
      if(tmp[i] == hand + " hand")
        break;
    if(!weapons[i]) // they're going to attack unarmed.
      weapons = ({ });
    else if(weapons[i]->query_weapon()) // they'll attack with the weapon
      weapons = ({ weapons[i] });
  }

  // Make sure we have a unique list of weapons if we're going to have to
  // pick one.
  if(sizeof(weapons) > 1)
    weapons = uniq_array(filter(weapons, (: $1 && $1->query_weapon() :)));
  
  // Pick the attacking weapon from what's left by using the most appropriate
  // for distance.
  switch(sizeof(weapons)) {
  case 0:
    
    weapon = att->attacker;
    break;
  case 1:
    // Tend to attack with the weapon in the left hand.
    weapon = weapons[0];
    break;
  default:
#ifdef USE_DISTANCE
    // Pick the weapon that best fits the combat distance we're at right now.
    weapon = weapons[0];
    foreach(wtmp in weapons)
      if(ABS(REACH + weapon->query_length() - att->distance) >
         ABS(REACH + wtmp->query_length() - att->distance))
        weapon = wtmp;
#else
    weapon = weapons[random(sizeof(weapons))];
#endif
  }

  att->attack_weapon = weapon;

  // Calculate their attack percentage.

  // For the moment I've just given a simple strength bonus so that stronger
  // players hit slightly harder.
  perc = 75 + (att->attacker)->query_str() + (att->attacker)->query_dex();

  // The heavier the weapon the harder it is to hit hard with it.
  if(weapon != att->attacker)
    perc -= weapon->query_weight() / 2;

  if(perc < 25)
    perc = 25;
  
  attacks = weapon->weapon_attacks(perc, att->defender);
  
  if(!sizeof(attacks)) {
#if DEBUG == 2
    combat_debug("No attacks with %O, terminating [%d, %O].", weapon,
                 perc, att->defender);
#endif
    return att;
  }

  // Choose an attack at random from the list of attacks. This should probably
  // be a weighted average (since each attack has a %age chance)
  which_attack = random(sizeof(attacks) / AT_ARRAY_SIZE);
  attacks = attacks[(which_attack * AT_ARRAY_SIZE)..];

  // No free hands? Can't attack with your hands then.
  if(attacks[AT_NAME] == "hands" && !(att->attacker)->query_free_limbs())
    return att;
  
  att->attack_data = attacks;
  att->attack_skill = ATTACK_SKILL + attacks[AT_SKILL];

  // Choose an attack zone. If they've chosen one then they get a -ve modifier
  // If they haven't set a preference then choose one at random.
  if((att->attacker_tactics)->focus_zone == "upper body") {
    tmp = ((att->opponent)->query_race_ob())->query_target_zones();
    zone = tmp[random(sizeof(tmp) / 2)];
  } else  if((att->attacker_tactics)->focus_zone == "lower body") {
    zone = tmp[random(sizeof(tmp) / 2) + sizeof(tmp)/2];
  } else if((att->attacker_tactics)->focus_zone &&
     (att->attacker_tactics)->focus_zone != "none") {
    // generic zone.
    zone = (att->attacker_tactics)->focus_zone;
    // map to the races zone
    zone = ((att->opponent)->query_race_ob())->map_target_zone(zone);
    att->target_zone = zone;    
  } else {
    if(att->attack_data[AT_SKILL] == "unarmed") {
      // for humanoids punches will typically be upper body while
      // kicks will typically be lower body (depending on target size).
      if(att->attack_data[AT_NAME] == "hands")
        sz = ((att->attacker)->query_height() * 2) /
          (att->opponent)->query_height();
      else if(att->attack_data[AT_NAME] == "feet")
        sz = ((att->attacker)->query_height() / 2) /
          (att->opponent)->query_height();
    } else
      sz = to_float((att->attacker)->query_height()) /
        to_float((att->opponent)->query_height());

    // The relative sizes of combatants lead to more attacks higher or lower
    // on the body.
    tmp = ((att->opponent)->query_race_ob())->query_target_zones();
    if(sizeof(tmp)) {
      if(sz > 1.5)
        zone = tmp[random(random(sizeof(tmp)))];
      if(sz < 0.75) {
        i = random(sizeof(tmp) + 10);
        if(i > sizeof(tmp)-1)
          i = sizeof(tmp) - 1;
        zone = tmp[i];
      } else {
        zone = tmp[random(sizeof(tmp))];
      }
    }
  }
  att->target_zone = zone;

  return att;
}

/**
 * Will the defender defend or not? If so, with what skill and weapon and at
 * what action cost?
 *
 * PRE: all attack data and opponent and defender data have been set.
 * POST: defense type, skill and weapon must be set. If defense_action
 * is "none" the defender will have a big negative modifier.
 *
 * @param class attack
 * @return class attack
 */
class attack choose_defense(class attack att) {
  object defender;
  string response, *hands, hand;
  object *weapons;
  class tactics tactics;
  int i, which;
#ifdef USE_DISTANCE
  int move_cost;
#endif  

  defender = att->defender;
  att->defense_action = "none";
  att->defense_skill = DODGE;
  att->defense_weapon = 0;

  tactics = att->defender_tactics;

  // Defenders (when the defender is not the opponent) have already
  // decided that they'll take on this action regardless of their
  // defecit thus only check defecits if opponent == defender.
  if(defender == att->opponent) {
    if(!defender->query_can_defend())
      return att;
    
    if(!DEFENSIVE_DEFECITS[tactics->attitude])
      tactics->attitude = "neutral";

    if(att->defender_defecit > DEFENSIVE_DEFECITS[tactics->attitude])
      return att;
  }

#ifdef USE_DISTANCE
  else if(att->distance != defender->query_distance(att->attacker) &&
            ABS(defender->query_distance(att->attacker)) >
            MOVE_DISTANCE[tactics->attitude]) {
    // If we have a defender or protector and they aren't in the right
    // position they need to move there and that will cost them some extra
    // actions. This _could_ lead to them spending all their time running
    // back and forth if they try to protect/defend too many people.

    defender->set_distance(att->attacker, att->distance);
    move_cost = MOVE_COST;
    move_cost -= defender->query_skill_bonus(MOVE_SKILL) / 50;
    if(move_cost < 0)
      move_cost = 0;

    (att->opponent)->event_combat(att->opponent, defender->one_short() +
                                  " moves to " +
                                  (att->person_hit == defender ?
                                   "protect" : "defend" ) + " you.\n",
                                  ({ }), 0);
    defender->event_combat(defender, "You move to " +
                           (att->person_hit == defender ?
                            "protect" : "defend") + " " +
                           (att->opponent)->one_short() + ".\n",
                           ({ }), 0);
    event(environment(att->opponent), "combat", defender->one_short() +
          " moves to " + (att->person_hit == defender ? "protect" : "defend") +
          " " + (att->opponent)->one_short() + ".\n",
          ({ att->opponent, defender, att->person_hit }), 1);
  }
#endif

  response = tactics->response;

  // Defenders have two choices, parry or don't, dodging isn't any use.
  if(defender != att->opponent) {
    if(response == "dodge")
      return att;
    response = "parry";
  } else if(!response || response == "neutral") // opponents get a free choice.
    response = ({"parry", "dodge"})[random(2)];

  if(response == "parry") {
    weapons = (object *)defender->query_holding();

    // Figure out which weapon index is the hand they prefer to parry with.
    hand = (att->defender_tactics)->parry;
    which = -1;
    hands = (att->defender)->query_limbs();
    for(i=0; i<sizeof(hands); i++)
      if(hands[i][0..sizeof(hand)-1] == hand)
        which = i;
    
    // If we found a preferred hand then try to parry with it, or parry
    // unarmed if they want to.
    if(which != -1) {
      if(weapons[which]) {
        att->defense_action = "parry";
        att->defense_skill = WEAPON_PARRY;
        att->defense_weapon = weapons[which];
      } else if((att->defender_tactics)->parry_unarmed) {
        att->defense_action = "parry";
        att->defense_skill = UNARMED_PARRY;
        att->defense_weapon = att->defender;
        att->defense_limb = (att->defender)->query_limbs()[which];
      }
    } else {
      // If there's no preferred hand and they have weapons they parry with
      // whichever they have a weapon in. Otherwise, they parry unarmed if
      // they want to do that, otherwise it's no defense.
      weapons -= ({ 0 });
      if(!sizeof(weapons)) {
        if((att->defender_tactics)->parry_unarmed) {
          att->defense_action = "parry";
          att->defense_skill = UNARMED_PARRY;
          att->defense_weapon = att->defender;
        }
      } else {
        att->defense_action = "parry";
        att->defense_skill = WEAPON_PARRY;
        att->defense_weapon = weapons[random(sizeof(weapons))];
      }
    }

  } else {
    att->defense_action = "dodge";
    att->defense_skill = DODGE;
    att->defense_weapon = 0;
  }

  return att;
}

/**
 * This method calculates the modifier to the attack.
 *
 * It will take into account all the basic stuff that makes the attack
 * harder or easier.
 *
 * PRE: All attack and response data has been filled in.
 * POST: The offensive modifier must be set.
 *
 * @param class attack
 * @return class attack
 */
protected class attack calc_attack_modifier(class attack att) {
  object attacker, attack_weapon, *holding;
  int mod, wep, hld, lght, mntd, hlth, brdn, dist, tact, targ, oth;
  int tmp, tmp2, limbs, dex, num, actions;
  string zone, *attackers;
  
  attacker = att->attacker;
  dex = attacker->query_dex();
  attack_weapon = att->attack_weapon;
  mod = wep = hld = lght = mntd = hlth = brdn = dist = tact = 0;

  if(attack_weapon != attacker) {
    /*
     * Find out how heavy the weapon is, how many hands they're using to hold
     * it, how many other weapons they're holding, which hand they're
     * holding their many hands they're using to hold the weapon and decrease
     * a based on that.
     */
    holding = attacker->query_holding();

    // tmp -- how many hands they're using to hold the attacking weapon
    // tmp2 -- weight factor for other held items.
    limbs = tmp2 = 0;
    for(num = 0; num < sizeof(holding); num++) {
      if(holding[num] == attack_weapon)
        limbs++;
      else if(holding[num]) {
        if((holding[num])->id("shield"))
          tmp2 += (holding[num])->query_weight() / 5;
        else
          tmp2 += (holding[num])->query_weight();
      }
    }

    // If they're holding a second weapon but aren't parrying with it
    // then their negative modifier for it is lower.
    switch((att->attacker_tactics)->response) {
    case "both":
      tmp2 = (tmp2 * 3) / 2;
      break;
    case "dodge":
      // To hold multiple weapons your dex must outweigh the weight of the
      // secondary weapons.
      if(tmp2 < dex)
        tmp2 /= 2;
    }
    
    // weapon weight.
    wep = attack_weapon->query_weight() + (tmp2 / 2);
    
    // 2/3 for one hand. 2 / 4 for two hands. 2 / 5 for three etc.
    wep /= (limbs + 1);
    wep -= (att->attacker)->query_str();
    
    // If their weapon weight factor adds up to more than their strength
    // they take a negative modifier.
    if(wep > 0 ) {
      wep = - to_int(pow(wep, 1.4));
    } else
      wep = 0;


    // If you're attacking with your off-hand (ie. not your prime hand)
    // you take a negative modifier if your dex is low.
    // (index 0 is your prime hand)
    if(dex < 14 && sizeof(holding) && holding[0] != attack_weapon)
      hld = dex - 14;
    
  } else {
    wep =  (dex * 2) - attacker->query_burden();
  }

  // light levels
  switch(attacker->check_dark(environment(attacker)->query_light())) {
  case -2:
  case 2:
    lght = -50;
    break;
  case -1:
  case 1:
    lght = -25;
    break;
  }

  // mountedness
  mntd = 0;

  // health (up to -50)
  hlth -= (50 - (attacker->query_hp() * 50) / attacker->query_max_hp());
  
  // fatigue (up to -25)
  tmp = attacker->query_specific_gp("fighting");
  if(tmp < -50)
    hlth += -25;
  else if(tmp < 0)
    hlth += (tmp / 2);

  // burden (up to -33)
  brdn -= (attacker->query_burden() / 3);

#ifdef USE_DISTANCE
  // distance from opponent & length of weapon.
  if(att->attack_weapon == att->attacker)
    dist = -3 * ABS(REACH - att->distance);
  else
    dist = -3 * ABS(REACH + (att->attack_weapon)->query_length() -
                    att->distance);
#endif

  // Bonus for offensive tactics.
  switch((att->attacker_tactics)->attitude) {
  case "insane":
    tact = 25;
    break;
  case "offensive":
    tact = 15;
    break;
  case "defensive":
    tact = -25;
    break;
  case "wimp":
    tact = -50;
    break;
  }

  switch(att->attacker_tactics->focus_zone) {
  case 0:
  case "none":
    targ = 0;
    break;
  case "upper body":
  case "lower body":
    targ -= 25;
    break;
  default:
    tmp = 0;
    foreach(zone in ((att->opponent)->query_race_ob())->query_target_zones())
      if(zone == att->target_zone)
        tmp++;
    num = sizeof(((att->opponent)->query_race_ob())->query_target_zones());
    // between -0 & -25 depending on what %age of the body the target zone
    // accounts for.
    targ -= ((num - tmp) * 25 / num);
  }

  if(targ < -25) {
    debug_printf("Targ too small (%s) %d. num: %d, tmp: %d\n",
                 (att->attacker_tactics)->focus_zone, targ, num,tmp);
    targ = -25;
  }

  // If you're further down the attacker list than the second person
  // you take a negative modifier since only the first 2-4 people
  // can really get a hit in on someone.
  attackers = filter((att->opponent)->query_attacker_list(),
                     (: $1 && environment($1) == environment(this_object()):));
  num = member_array(this_object(), attackers);
  if(num > 1)
    num += -25 * num;

  if((att->attacker_tactics)->attack == "both")
    oth += 5;
  
  mod = wep + hld + lght + mntd + hlth + brdn + dist + tact + targ + oth + num;
  att->attack_modifier += mod;

  // Calculate how much time an attack takes.
  actions += ATTACK_COST;
  
  // heavier weapons are slower to attack with.
  if(attack_weapon != att->attacker)
    actions += (sqrt(attack_weapon->query_weight()) * 3) / (limbs + 1);

  // Holding two weapons gives a slight advantage.
  if(sizeof((att->attacker)->query_weapons()) > 1)
    actions -= ATTACK_COST / 4;
  
  // more skilled opponents are more fluid in their movements.
  actions -= ((att->attacker)->query_skill_bonus(att->attack_skill) +
              (att->attacker)->query_skill_bonus(TACTICAL_SKILL)) / 50;
  
  if(actions < ATTACK_COST / 5)
    actions = ATTACK_COST / 5;
  else if(actions > ATTACK_COST * 2)
    actions = ATTACK_COST * 2;
  att->attack_cost = actions;

#if DEBUG == 1
  combat_debug("Attack %s (%s): wep: %d hld: %d lght: %d mntd: %d "
               "hlth: %d brdn: %d dist: %d tact: %d targ: %d oth: %d "
               "num: %d = %d [%d] (actions: %d %d)",
               att->attack_data[AT_NAME],
               (att->attack_weapon)->query_name(),
               wep, hld, lght, mntd, hlth, brdn, dist, tact, targ, oth, num,
               mod, att->attack_modifier, actions, att->attacker_defecit);
#endif
  return att;
}

/**
 * This method calculates the modifier to the defense.
 *
 * It will take into account all the basic stuff that makes the defense
 * harder or easier.
 *
 * PRE: All attack and response data has been filled in.
 * POST: The defensive modifier must be set.
 *
 * @param class attack
 * @return class attack
 */
class attack calc_defense_modifier(class attack att) {
  object defender, attacker, attack_weapon, defense_weapon, *holding;
  int mod, wep, wght, dist, brdn, hnd, lght, hlth, tact, prot, oth;
  int tmp, num, dex, limbs, actions;
  
  mod = wep = wght = dist = brdn = hnd = lght = hlth = tact = prot = oth = 0;
  defender = att->defender;
  defense_weapon = att->defense_weapon;
  dex = defender->query_dex();
  attacker = att->attacker;
  attack_weapon = att->attack_weapon;

  // Parry & Dodge are somewhat different. Obviously parrying depends on
  // using an object to block while dodge requires moving ones whole body.
  // They should each have strengths and weaknesses.
  switch(att->defense_action) {
  case "parry":
    if(defense_weapon != defender && defense_weapon) {

      holding = defender->query_holding();

      // weapon weight
      if(defense_weapon->id("shield"))
        wep = defense_weapon->query_weight() / 5;
      else
        wep = defense_weapon->query_weight() * 2;

      /*
       * Find out how many hands they're using to hold the weapon/shield
       */
      limbs = 0;
      for(num = 0; num < sizeof(holding); num++)
        if(holding[num] && holding[num] == defense_weapon)
          limbs++;

      // 2/2 for one hand. 2 / 3 for two hands. 2 / 4 for three etc.
      wep /= (limbs + 2);

      if(wep > defender->query_str())
        wep = - to_int(pow(wep - defender->query_str(), 1.3));

      // relative weight of attackers weapon to ours?
      if(attack_weapon != attacker)
        wght = 2 * (defense_weapon->query_weight() -
                    attack_weapon->query_weight());
      else
        wght = defense_weapon->query_weight() / 2;

      // Having a heavier defensive weapon doesn't help much, if at all.
      // However, having a light defensive weapon is a disadvantage.
      if(wght > 5)
        wght = 5;
      
#ifdef USE_DISTANCE
      dist = - ABS(REACH + defense_weapon->query_length() - att->distance);
#endif
    } else {
#ifdef USE_DISTANCE
      dist = - ABS(REACH - att->distance);
#endif
    }

    brdn = -(defender->query_burden() / 3);

    if(sizeof(holding)) {
      // If you're parrying with your off-hand (ie. not your prime hand)
      // you take a negative modifier.
      if(dex < 14 && holding[0] != defense_weapon)
        hnd = dex - 14;
    
      // If you're parrying with the same weapon you attack with
      // (ie. you're parrying & only holding one weapon that's a -ve.
      holding = uniq_array(filter(holding, (: $1 :)));
      if(dex  < 16 && sizeof(holding) == 1)
        hnd += dex - 16;
    }
    break;

  case "dodge":
    // All actions get the 1/3rd burden.
    brdn = -(defender->query_burden() / 3);

    // Dodgers get a dex mitigated burden modifier
    if(dex < (defender->query_burden() / 2))
      brdn -= dex - (defender->query_burden() / 2);
    
    if(att->attack_weapon != att->attacker)
      wght = ((att->attack_weapon)->query_weight() / 10);
    break;
  }

  // light levels / visibility of opponent
  if(!(att->attacker)->query_visible(defender))
    lght = -100;
  else {
    switch(defender->check_dark(environment(defender)->query_light())) {
    case -2:
    case 2:
      lght = -50;
      break;
    case -1:
    case 1:
      lght = -25;
      break;
    }
  }
  
  // mountedness

  // health (up to -25)
  hlth = - (25 - (defender->query_hp() * 25) / defender->query_max_hp());

  // fatigue (up to -25)
  tmp = defender->query_specific_gp("fighting");
  if(tmp < -50)
    hlth += -25;
  else if(tmp < 0)
    hlth += (tmp / 2);

  // Bonus for defensive tactics.
  switch((att->defender_tactics)->attitude) {
  case "insane":
    tact = -50;
    break;
  case "offensive":
    tact = -25;
    break;
  case "defensive":
    tact = 15;
    break;
  case "wimp":
    tact = 25;
    break;
  }

  // Protecting is a -15 modifier. Defending is an extra -15. This is because
  // it's easier to just jump in the way than to actually parry for someone.
  prot = 0;
  if(defender != att->opponent) {
    if(defender != att->person_hit) {
      // defending someone else with a shield is very tricky since shields
      // are setup only to protect you.
      if(defense_weapon->id("shield")) 
        prot -= 15;
      prot -= 15;
    }
    prot -= 15;
  }

  // Small bonus for response of both.
  if((att->defender_tactics)->response == "both")
    oth += 5;
  
  if(defender->query_casting_spell())
    oth -= 25;

  mod = wep + wght + dist + brdn + hnd + lght + hlth + tact + prot + oth;

  att->defense_modifier += mod;

  // Calculate how much time a parry/dodge takes.
  if(att->defense_action != "none") {
    actions = DEFENSE_COST;
    actions -= ((att->defender)->query_skill_bonus(att->defense_skill) +
      (att->attacker)->query_skill_bonus(TACTICAL_SKILL)) / 50;

    // heavier weapons are slower to parry with.
    if(att->defense_weapon && att->defense_weapon != att->defender) {
      if((att->defense_weapon)->id("shield"))
        actions += sqrt(att->defense_weapon->query_weight() / 4);
      else {
        actions += sqrt((att->defense_weapon->query_weight() * 2) /
                        (limbs + 1));

        if(sizeof((att->defender)->query_weapons()) > 1)
          actions -= DEFENSE_COST / 4;
      }
      
    } else if(att->defense_action == "dodge")
      actions += sqrt(defender->query_burden());

    if(actions < DEFENSE_COST / 5)
      actions = DEFENSE_COST / 5;
    else if(actions > DEFENSE_COST * 2)
      actions = DEFENSE_COST * 2;
    
    att->defense_cost = actions;
  }

#if DEBUG == 1
  combat_debug("Defense %s (%s): wep: %d wght: %d dist: %d "
               "brdn: %d hnd: %d lght: %d hlth: %d tact: %d prot: %d "
               "oth: %d = %d [%d] (actions: %d %d)",
               att->defense_action,
               att->defense_weapon ? (att->defense_weapon)->query_name() : "",
               wep, wght, dist, brdn, hnd, lght, hlth, tact, prot, oth,
               mod, att->defense_modifier, actions, att->defender_defecit);
#endif

  return att;
}

/****************************************************************************
 * Now we're into the damage phase.
 ****************************************************************************/

/**
 * After a successful attack (or possibly a parried one) calculate the
 * amount of damage done to the opponent.
 *
 * This method must fill in the amount of damage to be done and may adjust
 * the time taken too.
 *
 * PRE: the outcome of an attack must have been determined.
 * POST: damage must be filled in.
 */
protected class attack calc_damage(class attack att) {
  int damage, weapon_damage;
  object attacker;

  // Damage is a weighted average of the weapon damage and the
  // players skill.

  attacker = att->attacker;
  damage = att->attack_data[AT_DAMAGE];
  weapon_damage = damage;

  // This rather hampers unarmed, perhaps it should be removed.
  if(att->attack_weapon != attacker)
    damage = sqrt(damage * attacker->query_skill_bonus(att->attack_skill));

  if(damage > 3 * weapon_damage)
    damage = 3 * weapon_damage;

  // As the combat gets slower so the damage goes up to compensate otherwise
  // people heal faster than they take damage.
  damage *= COMBAT_SPEED;

  att->damage = damage;

  switch(att->result) {
  case OFFAWARD:
  case OFFWIN:
    // Critical attacks do 2x damage, exceptional attacks 1.5x
    switch(att->degree) {
    case TASKER_CRITICAL:
      att->damage *= 2;
      break;
    case TASKER_EXCEPTIONAL:
      att->damage = (att->damage * 3) / 2;
    case TASKER_MARGINAL:
      att->damage /= 2;
      break;
    }
    break;

  case DEFAWARD:
  case DEFWIN:
    // A Critical or Exceptional parry can cause the opponent to use up
    // extra combat actions since the defense pushed the attackers weapon
    // out of position or brought the defender into a better position.
    switch(att->degree) {
    case TASKER_CRITICAL:
      att->defense_cost = 0;
      break;
    case TASKER_EXCEPTIONAL:
      att->defense_cost /= 2;
      break;
    case TASKER_MARGINAL:
      att->defense_cost *= 2;
      break;
    }
    // All damage is always stopped.
    att->damage = 0;
  }

  combat_debug("Damage: %d %d",
               att->damage,
               attacker->query_skill_bonus(att->attack_skill));
  return att;
}

/**
 * Calculate how much of the damage is saved by the armour.
 *
 * PRE: all information has been filled in except the amount of damage
 * stopped by armour, and which piece of armour did the stopping.
 * POST: The damage stopped and which armour did it is filled in.
 *
 * @param class attack
 * @return class attack
 */
class attack calc_armour_protection(class attack att) {
  object person_hit;
  string armour_zone, damage_type;

  if(!att->damage || (att->result != OFFWIN && att->result != OFFAWARD))
    return att;

  person_hit = att->person_hit;

  // Specials/Criticals may change this calculation.
  armour_zone = (person_hit->query_race_ob())->
    map_armour_zone(att->target_zone);

  damage_type = att->attack_data[AT_TYPE];
  if(damage_type == "unarmed")
    damage_type = "blunt";

  att->armour_stopped = (int)person_hit->query_ac(damage_type, att->damage,
                                                  armour_zone);
  if(att->armour_stopped < 0) {
    debug_printf("armour stopped %d for %s %d %s",
                 att->armour_stopped, damage_type, att->damage, armour_zone);
    att->armour_stopped = 0;
  }
  
  return att;
}

/**
 * Do damage to the attacking & defending weapons.
 *
 * PRE: all information has been filled in
 * POST: the weapons involved have taken appropriate damage.
 *
 * @param class attack
 * @return class attack
 */
protected class attack damage_weapon(class attack att) {
  int off_damage, def_damage;

  switch(att->result) {
  case OFFAWARD:
  case OFFWIN:
    // The weapon hit the opponents armour.
    if(att->armour_stopped && att->degree)
      off_damage = att->armour_stopped / att->degree;
    // No def_damage needed since the query_ac() call damages the armour
    // directly (which is bad, but there we are).
    break;
  case DEFWIN:
  case DEFAWARD:
    // The defender successfully defended. If they parried then their
    // parrying weapon may take some damage and the attacking
    // weapon too.
    if(att->defense_action == "parry" && att->damage && att->degree) {
      off_damage = att->damage * (att->degree - 1);
      def_damage = att->damage / att->degree;
    }
    break;
  }

  if(off_damage) {
    if(att->attack_weapon == att->attacker)
      if((att->attack_weapon)->query_hp() > (off_damage / 10))
        (att->attack_weapon)->adjust_hp(- (off_damage / 10), att->defender,
                                        att->defense_weapon,
                                        "hitting someone.");
    else if(function_exists("hit_weapon", att->attack_weapon))
      (att->attack_weapon)->hit_weapon(off_damage, att->attack_data[AT_SKILL]);
    else
      (att->attack_weapon)->do_damage(att->attack_data[AT_SKILL], off_damage);
  }

  if(def_damage) {
    if(att->defense_weapon == att->defender)
      (att->defense_weapon)->adjust_hp(- def_damage, att->attacker,
                                       att->attack_weapon,
                                       "parrying an attack with a limb.");
    else if(function_exists("hit_weapon", att->defense_weapon))
      (att->defense_weapon)->hit_weapon(def_damage,
                                        att->attack_data[AT_SKILL]);
    else
      (att->defense_weapon)->do_damage(att->attack_data[AT_SKILL], def_damage);
  }

  return att;
}

/**
 * Prepare the result messages.
 *
 * This method prepares the attack and defense messages setting up the 5
 * element arrays of att_mess & def_mess. The elements are as follows:
 * 0 - the attacker, 1 - the opponent, 2 - everyone else, 3 - the defender
 * 4 - the person hit
 *
 * PRE: The attack has been done and the amount of damage done and stopped
 * has been calculated.
 * POST: The 5 element att_mess & def_mess arrays are populated.
 *
 * @param class attack
 */
protected class attack prepare_messages(class attack att) {
  object attacker, opponent, defender, person_hit;
  string *verb, def_short, margin;
  string *def_mess, *att_mess;
  int damage, armour_stopped;

  attacker = att->attacker;
  opponent = att->opponent;
  defender = att->defender;
  person_hit = att->person_hit;
  damage = att->damage;
  armour_stopped = att->armour_stopped;

  // Set attack part of the message.
  switch(att->result) {
  case OFFAWARD:
    tell_object(att->attacker,
                "%^YELLOW%^"+ replace(({ "You feel "
                "that your skill with $weapon$ has increased.", "You "
                "feel more able to use $weapon$.", "You seem to be a "
                "step closer to mastering $weapon$." })[random(3)],
                "$weapon$", (att->attack_weapon == att->attacker ?
                "unarmed combat" : (string)(att->attack_weapon)->a_short())) +
                "%^RESET%^\n" );

  case OFFWIN:
    if(person_hit == opponent) {
      att_mess = ATTACK_MESSAGE->query_message(damage - armour_stopped,
                                               att->attack_data[AT_SKILL],
                                               opponent, attacker,
                                               att->attack_data[AT_NAME],
                                               att->attack_weapon,
                                               att->target_zone);
      
      if(sizeof(att_mess) != 3) {
        att_mess = ({"You hit "+opponent->the_short()+"",
                       attacker->the_short()+" hits you",
                       attacker->the_short()+" hits "+opponent->the_short()});
      }
      
      att_mess += ({ att_mess[M_SPECTATORS], att_mess[M_SPECTATORS] });
    } else {
      att_mess = ATTACK_MESSAGE->query_message(damage - armour_stopped,
                                               att->attack_data[AT_SKILL],
                                               opponent, attacker,
                                               att->attack_data[AT_NAME],
                                               att->attack_weapon,
                                               att->target_zone);
      
      if(sizeof(att_mess) != 3) {
        att_mess = ({"You almost hit "+opponent->the_short()+"",
                       attacker->the_short()+" almost hits you",
                       attacker->the_short()+" almost hits "+
                       opponent->the_short()});
      }
      
      att_mess += ({ att_mess[M_SPECTATORS], att_mess[M_SPECTATORS] });

      att_mess[M_ATTACKER] += " but at the last second " +
        person_hit->the_short() + " leaps in and protects " +
        opponent->query_objective();
      att_mess[M_OPPONENT] += " but at the last second " +
        person_hit->the_short() + " leaps in and protects you";
      att_mess[M_SPECTATORS] += " but at the last second " +
        person_hit->the_short() + " leaps in and protects " +
        opponent->query_objective();
      if(defender == person_hit)
        att_mess[M_DEFENDER] += " but at the last second " +
          person_hit->the_short() + " leap in and protect " +
          opponent->query_objective();
      else
        att_mess[M_DEFENDER] += " but at the last second " +
          person_hit->the_short() + " leaps in and protects " +
          opponent->query_objective();

      att_mess[M_PERSON_HIT] += " but at the last second you leap in and "
        "protect " + opponent->query_objective();
    }
    break;
  case DEFAWARD:
  case DEFWIN:
    att_mess = ATTACK_MESSAGE->query_message(0,
                                             att->attack_data[AT_SKILL],
                                             opponent, attacker,
                                             att->attack_data[AT_NAME],
                                             att->attack_weapon,
                                             att->target_zone);
    if(sizeof(att_mess) != 3) {
      att_mess = ({"You try to hit "+opponent->the_short()+"",
                     attacker->the_short()+" tries to hit you",
                     attacker->the_short()+" tries to hit "+
                     opponent->the_short()});
    }
    att_mess += ({ att_mess[M_SPECTATORS], att_mess[M_SPECTATORS] });
    break;
  }

  def_mess = allocate(5);
  def_mess[M_ATTACKER] = def_mess[M_OPPONENT] = def_mess[M_SPECTATORS] =
    def_mess[M_DEFENDER] = def_mess[M_PERSON_HIT] = "";

  // Now set the response part ie. what happened after the attack.
  switch(att->result) {
  case OFFAWARD:
  case OFFWIN:
    // The attack was successful, so we add armour messages if appropriate.
    if(armour_stopped && armour_stopped > damage / 3) {
      if(objectp(att->stopped_by)) {
        def_mess[M_ATTACKER] = " but " + (att->stopped_by)->poss_short();
        def_mess[M_OPPONENT] = def_mess[M_ATTACKER];
        def_mess[M_SPECTATORS] = def_mess[M_ATTACKER];
        def_mess[M_DEFENDER] = def_mess[M_ATTACKER];
        def_mess[M_PERSON_HIT] = def_mess[M_ATTACKER];
      } else if(att->stopped_by) {
        def_mess[M_ATTACKER] = " but " + person_hit->query_possessive() +
          " " + att->stopped_by;

        if(person_hit == opponent)
          def_mess[M_OPPONENT] = " but your " + att->stopped_by;
        else
          def_mess[M_OPPONENT] = def_mess[M_ATTACKER];

        def_mess[M_SPECTATORS] = def_mess[M_ATTACKER];

        if(person_hit == defender)
          def_mess[M_DEFENDER] = " but your " + att->stopped_by;
        else
          def_mess[M_DEFENDER] = def_mess[M_ATTACKER];

        def_mess[M_PERSON_HIT] = " but your " + att->stopped_by;
      }

      // armour protected them, at least partly
      if(!damage || armour_stopped >= damage) {
        def_mess[M_ATTACKER] += " absorbs all of";
        def_mess[M_OPPONENT] += " absorbs all of";
        def_mess[M_SPECTATORS] += " absorbs all of";
        def_mess[M_DEFENDER] += " absorbs all of";
        def_mess[M_PERSON_HIT] += " absorbs all of";
      } else if(armour_stopped > (damage * 2 / 3)) {
        def_mess[M_ATTACKER] += " absorbs most of";
        def_mess[M_OPPONENT] += " absorbs most of";
        def_mess[M_SPECTATORS] += " absorbs most of";
        def_mess[M_DEFENDER] += " absorbs most of";
        def_mess[M_PERSON_HIT] += " absorbs most of";
      } else {
        def_mess[M_ATTACKER] += " absorbs some of";
        def_mess[M_OPPONENT] += " absorbs some of";
        def_mess[M_SPECTATORS] += " absorbs some of";
        def_mess[M_DEFENDER] += " absorbs some of";
        def_mess[M_PERSON_HIT] += " absorbs some of";
      }
      def_mess[M_ATTACKER] += " the blow";
      def_mess[M_OPPONENT] += " the blow";
      def_mess[M_SPECTATORS] += " the blow";
      def_mess[M_DEFENDER] += " the blow";
      def_mess[M_PERSON_HIT] += " the blow";
    }
    break;

  case DEFAWARD:
    if(att->defense_action == "parry") {
      if(att->defense_weapon && (att->defense_weapon)->query_weapon())
        verb = ({ "parry", "parries", "parrying" });
      else
        verb = ({ "block", "blocks", "blocking" });

      tell_object(att->defender,
                  "%^YELLOW%^"+ replace( ({ "You move more surely "
               "than you thought you could in $verbing$ $attacker$'s attack",
               "You just manage to $verb$ $attacker$'s attack, but you'll "
               "know better next time", "You feel better at $verbing$ as "
               "you $verb$ $attacker$'s attack" })[ random( 3 ) ],
               ({ "$verb$", verb[ 0 ], "$verbing$", verb[ 2 ], "$attacker$",
               (string)(att->attacker)->the_short() }) ) +".%^RESET%^\n" );
    } else if(att->defense_action == "dodge") {
      tell_object(att->defender,
                   "%^YELLOW%^"+ replace( ({ "You move more nimbly "
               "than you thought you could in dodging $attacker$", "You "
               "managed to predict $attacker$'s attack, letting you dodge it "
               "more easily", "You feel better at dodging as you avoid "
               "$attacker$'s attack" })[ random( 3 ) ], "$attacker$",
               (string)(att->attacker)->the_short() ) +".%^RESET%^\n" );
    }
  case DEFWIN:
    // The defense was successful so we print parry/dodge messages as
    // appropriate.
    def_mess = allocate(5);
    def_mess[M_ATTACKER] = "";
    def_mess[M_OPPONENT] = "";
    def_mess[M_SPECTATORS] = "";
    def_mess[M_DEFENDER] = "";
    def_mess[M_PERSON_HIT] = "";
    switch(att->degree) {
    case TASKER_MARGINAL:
      margin = "just ";
      break;
    case TASKER_EXCEPTIONAL:
      margin = "deftly ";
      break;
    case TASKER_MARGINAL:
      margin = "easily ";
      break;
    default:
      margin = "";
      break;
    }

    if(att->defense_action == "parry" && att->defense_weapon) {
      if(defender == opponent)
        def_mess[M_ATTACKER] = " but " + defender->query_pronoun() + " " +
          margin;
      else
        def_mess[M_ATTACKER] = " but " + defender->the_short() + " " + margin;

      if(defender == opponent)
        def_mess[M_OPPONENT] = " but you " + margin;
      else
        def_mess[M_OPPONENT] = def_mess[M_ATTACKER];

      def_mess[M_SPECTATORS] = def_mess[M_ATTACKER];
      def_mess[M_DEFENDER] = " but you " + margin;
      def_mess[M_PERSON_HIT] = def_mess[M_ATTACKER];

      if(att->defense_weapon != defender &&
         !(att->defense_weapon)->id("shield")) {
        def_mess[M_ATTACKER] += "parries";
        if(defender == opponent)
          def_mess[M_OPPONENT] += "parry";
        else
          def_mess[M_OPPONENT] += "parries";

        def_mess[M_SPECTATORS] += "parries";
        def_mess[M_DEFENDER] += "parry";
        if(att->person_hit == att->defender)
          def_mess[M_PERSON_HIT] += "parry";
        else
          def_mess[M_PERSON_HIT] += "parries";
      } else {
        def_mess[M_ATTACKER] += "blocks";
        if(defender == opponent)
          def_mess[M_OPPONENT] += "block";
        else
          def_mess[M_OPPONENT] += "blocks";

        def_mess[M_SPECTATORS] += "blocks";
        def_mess[M_DEFENDER] += "block";
        if(att->person_hit == att->defender)
          def_mess[M_PERSON_HIT] += "block";
        else
          def_mess[M_PERSON_HIT] += "blocks";
      }

      if(att->defense_weapon != defender) {
        def_short = (att->defense_weapon)->short();
      } else if(att->defense_limb) {
        def_short = att->defense_limb;
      } else
        def_short = (defender->query_race_ob())->map_target_zone("hands");


      def_mess[M_ATTACKER] += " the blow with " +
        defender->query_possessive() + " " + def_short;
      if(defender == opponent)
        def_mess[M_OPPONENT] += " the blow with your " + def_short;
      else
        def_mess[M_OPPONENT] += " the blow with " +
          defender->query_possessive() + " " + def_short;

      def_mess[M_SPECTATORS] += " the blow with " +
        defender->query_possessive() + " " + def_short;

      def_mess[M_DEFENDER] += " the blow with your " + def_short;

      if(defender == person_hit)
        def_mess[M_PERSON_HIT] += " the blow with your " + def_short;
            else
        def_mess[M_PERSON_HIT] += " the blow with " +
          defender->query_possessive() + " " + def_short;

    } else if(att->defense_action == "dodge") {
      def_mess[M_ATTACKER] = " but " + defender->query_pronoun() + " " +
        margin + "dodges out of the way";
      def_mess[M_OPPONENT] = " but you " + margin + "dodge out of the way";
      def_mess[M_SPECTATORS] = " but " + defender->query_pronoun() + " " +
        margin + "dodges out of the way";
    } else {
      def_mess[M_ATTACKER] = " but, although unable to defend, " +
        defender->query_pronoun() + " somehow avoids the attack";
      def_mess[M_OPPONENT] = " but, although unable to defend, you somehow "
        "avoid the attack";
      def_mess[M_SPECTATORS] = " but, although unable to defend, " +
        defender->query_pronoun() + " somehow avoids the attack";
    }
  }
  att->attack_messages = att_mess;
  att->defense_messages = def_mess;

  if(att->result == DEFAWARD || att->result == DEFWIN || !damage ||
     armour_stopped >= damage)
    att->verbose = 1;

  return att;
}

/**
 * Write out the attack/defense messages.
 *
 * PRE: all information has been filled in
 * POST: the appropriate messages have been written out.
 *
 * @param class attack
 * @return class attack
 */
protected class attack write_messages(class attack att) {
  int verbose;
  
  verbose = att->verbose;
  (att->attacker)->event_combat(att->attacker,
                                att->attack_messages[M_ATTACKER] +
                                att->defense_messages[M_ATTACKER] + ".\n",
                                ({ }), verbose);
  (att->opponent)->event_combat(att->opponent,
                                att->attack_messages[M_OPPONENT] +
                                att->defense_messages[M_OPPONENT] + ".\n",
                                ({ }), verbose);
  if(att->defender != att->opponent &&
     sizeof(att->attack_messages) > M_DEFENDER)
    (att->defender)->event_combat(att->defender,
                                  att->attack_messages[M_DEFENDER] +
                                  att->defense_messages[M_DEFENDER] + ".\n",
                                  ({ }), verbose);
  if(att->person_hit != att->opponent && att->person_hit != att->defender &&
     sizeof(att->attack_messages) > M_PERSON_HIT)
    (att->person_hit)->event_combat(att->person_hit,
                                    att->attack_messages[M_PERSON_HIT] +
                                    att->defense_messages[M_PERSON_HIT] +
                                    ".\n", ({ }), verbose);

  event(environment(att->attacker), "combat",
        att->attack_messages[M_SPECTATORS] +
        att->defense_messages[M_SPECTATORS] + ".\n",
        ({ att->attacker, att->opponent, att->defender, att->person_hit }),
        verbose);
  return att;
}

/**
 * Adjust the time left, combat actions and gp used by this attack.
 * This calls adjust_action_defecit in both the attacker and defender
 * as appropriate.
 *
 * @param class attack
 */
protected void adjust_actions(class attack att) {
  int tmp;

  // Reduce the attackers time left.
  if((att->attacker)->query_time_left() > 0)
    (att->attacker)->adjust_time_left(-DEFAULT_TIME);

  // Reduce their actions.
  (att->attacker)->adjust_action_defecit(att->attack_cost);
  (att->opponent)->adjust_action_defecit(att->defense_cost);

  // Extra cost for defenders
  if(att->defender != att->opponent)
    (att->defender)->adjust_action_defecit(DEFENSE_COST);

  // Extra cost for protectors
  if(att->person_hit != att->opponent)
    (att->person_hit)->adjust_action_defecit(DEFENSE_COST);

  // Reduce their GP
  tmp = ATTACK_GP[(att->attacker_tactics)->attitude];
  (att->attacker)->adjust_gp(-tmp);

  if(att->defender != att->opponent) {
    tmp = DEFENDER_GP[(att->defender_tactics)->attitude];
    (att->defender)->adjust_gp(-tmp);
  } else if(att->defense_action != "none") {
    tmp = DEFENSE_GP[(att->defender_tactics)->attitude];
    (att->opponent)->adjust_gp(-tmp);
  }

  if(att->person_hit != att->opponent) {
    tmp = DEFENDER_GP[(att->person_hit)->query_combat_attitude()];
    (att->person_hit)->adjust_gp(-tmp);
  }
}

/**
 * This function is called as part of the post attack processing and performs
 * the checks to see if someone can move closer or further away from their
 * opponent.
 *
 * @param class attack
 * @return class attack
 * @see after_attack()
 */
protected class attack perform_movement(class attack att) {
  int ideal, move_cost, closer;

  // Figure out if they will attempt to move-in or move-away from their
  // opponent.
  ideal = DISTANCES[(att->attacker_tactics)->ideal_distance];

  // If the difference between the actual and ideal distance is greater than
  // the acceptable difference defined for our attitude we'll attempt to
  // move in or out to compensate.
  if(ABS(ideal - att->distance) >
     MOVE_DISTANCE[(att->attacker_tactics)->attitude]) {

    if(att->distance > ideal)
      closer = 1;
    move_cost = MOVE_COST;
    move_cost -= (att->attacker)->query_skill_bonus(MOVE_SKILL) / 30;

    if(move_cost > 0)
      att->attack_cost += MOVE_COST;
    
    switch(TASKER->compare_skills(att->attacker, MOVE_SKILL,
                                  att->defender, MOVE_SKILL,
                                  att->attack_modifier -
                                  att->defense_modifier,
                                  TM_CONTINUOUS, TM_CONTINUOUS, 0)) {
    case OFFAWARD:
      tell_object(att->attacker,
                  "%^YELLOW%^You feel that your tactical skill has increased."
                  "%^RESET%^\n");
    case OFFWIN:
      if(closer) {
        (att->attacker)->event_combat(att->attacker, "You move in closer to " +
                                      (att->opponent)->one_short() + ".\n",
                                      ({ }), 0);
        (att->opponent)->event_combat(att->opponent,
                                      (att->attacker)->one_short() +
                                      " moves in closer to you.\n", ({ }), 0);

        event(environment(att->attacker), "combat",
              (att->attacker)->one_short() +
              " moves in closer to " + (att->opponent)->one_short() + ".\n",
              ({ att->attacker, att->opponent }), 1);

        set_distance(att->opponent, query_distance(att->opponent) -
                     DISTANCE_STEP);
        (att->opponent)->set_distance(att->attacker,
                                      query_distance(att->opponent));
      } else {
        (att->attacker)->event_combat(att->attacker,
                                      "You move further away from " +
                                      (att->opponent)->one_short() + ".\n",
                                      ({ }), 0);
        (att->opponent)->event_combat(att->opponent,
                                     (att->attacker)->one_short() +
                                     " moves further away from you.\n",
                                     ({ }), 0);
        event(environment(att->attacker), "combat",
              (att->attacker)->one_short() + " moves further away from " +
              (att->opponent)->one_short() + ".\n",
              ({ att->attacker, att->opponent }), 1);
        set_distance(att->opponent, query_distance(att->opponent) +
                     DISTANCE_STEP);
        (att->opponent)->set_distance(att->attacker,
                                      query_distance(att->opponent));
      }
      break;
    case DEFAWARD:
      tell_object(att->opponent,
                  "%^YELOW%^You feel that your tactical skill has increased."
                  "%^RESET%^\n");
    case DEFWIN:
      if(closer) {
        (att->attacker)->event_combat(att->attacker,
                                      "You try to move in closer to " +
                                      (att->opponent)->one_short() +
                                      " but cannot find "
                                      "an opportunity.\n", ({ }), 1);
        (att->opponent)->event_combat(att->opponent,
                                      (att->attacker)->one_short() +
                                      " tries to move in closer to you "
                                      "but you don't give " +
                                      (att->attacker)->query_objective() +
                                      " an opporunity.\n", ({ }), 1);
        event(environment(att->attacker), (att->attacker)->one_short() +
              " tries to move in closer to " +
              (att->opponent)->one_short() + " but " +
              (att->opponent)->query_pronoun() + " doesn't give " +
              (att->attacker)->query_objective() + " an opportunity.\n",
              ({ att->attacker, att->opponent }), 1);
      } else {
        (att->attacker)->event_combat(att->attacker,
                                      "You try to move further away from  " +
                                      (att->opponent)->one_short() +
                                      " but cannot find an opportunity.\n",
                                      ({ }), 1);
        (att->opponent)->event_combat(att->opponent,
                                      (att->attacker)->one_short() +
                                      " tries to move further away from you "
                                      "but you don't give " +
                                      (att->attacker)->query_objective() +
                                      " an opporunity.\n", ({ }) , 1);
        event(environment(att->attacker), "combat",
              (att->attacker)->one_short() +
              " tries to move further away from " +
              (att->opponent)->one_short() + " but " +
              (att->opponent)->query_pronoun() + " doesn't give " +
              (att->attacker)->query_objective() + " an opportunity.\n",
              ({ att->attacker, att->opponent }), 1);
      }
    }
  }

  return att;
}

/**
 * This function is called after it's all over and done (just before the
 * costs are charged). It can be used to do any special outcomes that
 * we may decide on.
 * Note that if you override this function in a special be absolutely certain
 * you know what you're doing if you return R_ABORT or R_DONE. Use of either
 * of these two could cause really nasty side-effects.
 *
 * @param class attack
 * @return class attack
 */
protected class attack after_attack(class attack att) {
  string *locations;
  int total, count, lperc, hperc;

#ifdef USE_DISTANCE
  if((att->result == OFFWIN || att->result == OFFAWARD) &&
     (att->attacker_tactics)->ideal_distance &&
     environment(att->attacker) == environment(att->opponent))
    att = perform_movement(att);
#endif

  adjust_actions(att);
  if((att->damage - att->armour_stopped) <= 0)
    return att;

  // Do weapon special attacks.
  if((environment(att->person_hit) == environment(att->attacker)) &&
     (att->result == OFFWIN || att->result == OFFAWARD)) {
    (att->attack_weapon)->attack_function(att->attack_data[AT_NAME],
                                          att->damage - att->armour_stopped,
                                          att->person_hit, att->attacker);
  }

  // Determine special injuries if enough damage was done.

  locations = (att->defender->query_race_ob())->query_target_zones();

  total = sizeof(locations);
  tmpzone = att->target_zone;
  locations = filter(locations, (: $1 == tmpzone :));
  count = sizeof(locations);
  lperc = (count * 100) / total;

  if(att->degree == TASKER_CRITICAL) {
    hperc = ((att->damage - att->armour_stopped) * 100) /
      (att->opponent)->query_max_hp();

    if(lperc < hperc) {
      switch(att->target_zone) {
      case "head":
        // Unconscious
        break;
      case "chest":
      case "stomach":
        break;
      case "arm":
        // broken arm.
        break;
      case "leg":
        // broken leg.
        break;
      }
    }
  }
  return att;
}

/**
 * Go through the list of the attacker & opponents specials and
 * call any that have registered for this stage.
 */
protected mixed *call_special(int stage, class attack att) {
  class combat_special *tmp;
  mixed *result;
  int i;

  if(!this_object()->query_no_specials()) {
    tmp = att->attacker_specials;
    for(i=0; i<sizeof(tmp); i++) {
      if((tmp[i]->type & T_OFFENSIVE) && (tmp[i]->events & stage)) {
        if(functionp(tmp[i]->callback))
          result = evaluate(tmp[i]->callback, stage, att, tmp[i]->data);
        else if(arrayp(tmp[i]->callback)) {
          if(!tmp[i]->callback[0] || !tmp[i]->callback[1]) {
            (att->attacker)->remove_special(tmp[i]->id);
          } else 
            result = call_other(tmp[i]->callback[0], tmp[i]->callback[1],
                                stage, att, tmp[i]->data);
        }
        
        if(sizeof(result)) {
          if(!classp(result[1])) {
            debug_printf("Error, call_special received invalid return "
                         "data. %O when calling %O stage %d\n", result,
                         tmp[i]->callback, stage);
            result[0] = R_REMOVE_ME | R_ABORT;
          }
                         
          if(result[0] & R_REMOVE_ME) {
            (att->attacker)->remove_special(tmp[i]->id);
          } else {
            // Set the data to be the new data in both the current attack
            // data & in the original info.
            tmp[i]->data = result[2];
            (att->attacker)->set_special_data(tmp[i]->id, result[2]);
          }
          
          if(result[0] & R_ABORT)
            return result;
          
          if((result[0] & R_DONE) || (result[0] & R_CONTINUE))
            break;
        }
      }
    }
  }

  if(att->defender && !(att->defender)->query_no_specials()) {
    tmp = att->defender_specials;
    for(i=0; i<sizeof(tmp); i++) {
      if((tmp[i]->type & T_DEFENSIVE) && (tmp[i]->events & stage)) {
        if(functionp(tmp[i]->callback))
          result = evaluate(tmp[i]->callback, stage, att, tmp[i]->data);
        else if(arrayp(tmp[i]->callback)) {
          if(!tmp[i]->callback[0] || !tmp[i]->callback[1]) {
            (att->defender)->remove_special(tmp[i]->id);
          } else 
            result = call_other(tmp[i]->callback[0], tmp[i]->callback[1],
                                stage, att, tmp[i]->data);
        }
        
        if(sizeof(result)) {
          if(!classp(result[1])) {
            debug_printf("Error, call_special received invalid return "
                         "data. %O when calling %O stage %d", result,
                         tmp[i]->callback, stage);
            result[0] = R_REMOVE_ME | R_ABORT;
          }

          if(result[0] & R_REMOVE_ME) {
            (att->defender)->remove_special(tmp[i]->id);
          } else {
            // Set the data to be the new data.
            (att->defender)->set_special_data(tmp[i]->id, result[2]);
          }
          
          if(result[0] & R_ABORT)
            return result;

          // Done or continue we stop processing specials.
          // R_PASSTHRU means try the next special.
          if((result[0] & R_DONE) || (result[0] & R_CONTINUE))
            break;
        }
      }
    }
  }
  return result;
}

/**
 * This performs the actual attack itself.
 * All attack data, everything we know about the attacker and the defender
 * is stored in the attack class which is passed into and back by every
 * function at every step of the attack.
 */
void do_attack() {
  class tasker_result result;
  class attack att;
  mixed *sp_result;
  int modifier, tmp;

  // This is required since some people have the old tactics settings :(
  init_tactics();
  if(_callout)
    _callout = remove_call_out("announce_intent");

  att = new(class attack,
            attacker : this_object(),
            attacker_tactics : this_object()->query_tactics(),
            attacker_specials : this_object()->query_specials(),
            attacker_concentrating : this_object()->query_concentrating(),
            attacker_defecit : this_object()->query_action_defecit(),
            attacker_last_opponent : this_object()->query_last_opponent(),
            attacker_last_weapon : this_object()->query_last_weapon(),
            attacker_last_action : this_object()->query_last_action(),
            attacker_last_result : this_object()->query_last_result(),
            repeat : 0
            );

  // choose opponent
  sp_result = call_special(E_OPPONENT_SELECTION, att);
  if(!sp_result || sp_result[0] & R_CONTINUE || sp_result[0] & R_PASSTHRU)
    att = this_object()->choose_opponent(att);
  else if(!(sp_result[0] & R_ABORT) && classp(sp_result[1]))
    att = (class attack)sp_result[1];
  else
    return;

  if(!(att->opponent) || !(att->opponent)->attack_by(att->attacker)) {
    combat_debug("No opponent chosen, aborting attack");
    return;
  }


  event(environment(att->attacker), "fight_in_progress", att->opponent);
  clear_protection(att->attacker, att->opponent);
  _combat->last_opponent = att->opponent;
  
  // This loop causes us to allow a defender to defend and if the defender
  // fails & isn't the opponent the opponent also gets a chance to defend.
  do {
    // Now lets figure out who will do the defense, the person being attacked
    // or someone/thing that wants to defend them.
    sp_result = call_special(E_DEFENDER_SELECTION, att);
    if(!sp_result || sp_result[0] & R_CONTINUE || sp_result[0] & R_PASSTHRU)
      att = (att->opponent)->choose_defender(att);
    else if(!(sp_result[0] & R_ABORT) && classp(sp_result[1]))
      att = (class attack)sp_result[1];
    else
      return;

    att->defender_tactics = (att->defender)->query_tactics();
    att->defender_concentrating = (att->defender)->query_concentrating();
    att->defender_last_opponent = (att->defender)->query_last_opponent();
    att->defender_last_action = (att->defender)->query_last_action();
    att->defender_last_result = (att->defender)->query_last_result();
    att->defender_defecit = (att->defender)->query_action_defecit();
    att->defender_specials = (att->defender)->query_specials();

    clear_protection(att->attacker, att->defender);

    // get weapon and attack data
    sp_result = call_special(E_ATTACK_SELECTION, att);
    if(!sp_result || sp_result[0] & R_CONTINUE || sp_result[0] & R_PASSTHRU)
      att = this_object()->choose_attack(att);
    else if(!(sp_result[0] & R_ABORT) && classp(sp_result[1]))
      att = (class attack)sp_result[1];
    else
      return;

    if(!att->attack_weapon || !sizeof(att->attack_data) || !att->attack_skill){
      combat_debug("Missing attack weapon, attack data or attack skill, "
                   "aborting attack.");
      return;
    }

    // set the 'last' data.
    _combat->last_action = att->attack_data[AT_NAME];
    _combat->last_weapon = att->attack_weapon;

    // will opponent defend? If so determine defense skill & weapon.
    sp_result = call_special(E_DEFENSE_SELECTION, att);
    if(!sp_result || sp_result[0] & R_CONTINUE || sp_result[0] & R_PASSTHRU)
      att = (att->defender)->choose_defense(att);
    else if(!(sp_result[0] & R_ABORT) && classp(sp_result[1]))
      att = (class attack)sp_result[1];
    else
      return;

    // Set the 'last' data.
    (att->opponent)->set_last_opponent(att->attacker);
    if(att->opponent != att->defender)
      (att->defender)->set_last_opponent(att->attacker);
    (att->defender)->set_last_action(att->defense_action);
    (att->defender)->set_last_weapon(att->defense_weapon);
    if(att->person_hit != att->opponent && att->person_hit != att->defender)
      (att->person_hit)->set_last_opponent(att->attacker);
    
    // determine attack modifier
    sp_result = call_special(E_ATTACK_MODIFIER, att);
    if(!sp_result || sp_result[0] & R_CONTINUE || sp_result[0] & R_PASSTHRU) {
      att = this_object()->calc_attack_modifier(att);
    } else if(!(sp_result[0] & R_ABORT) && classp(sp_result[1])) {
      att = (class attack)sp_result[1];
    } else
      return;

    // determine defense modifier
    // If they aren't defending they just get a big fat minus.
    if(!att->defense_action || att->defense_action == "none")
      att->defense_modifier += -1000;
    else {
      sp_result = call_special(E_DEFENSE_MODIFIER, att);
      if(!sp_result || sp_result[0] & R_CONTINUE || sp_result[0] & R_PASSTHRU)
        att = (att->defender)->calc_defense_modifier(att);
      else if(!(sp_result[0] & R_ABORT) && classp(sp_result[1]))
        att = (class attack)sp_result[1];
      else
        return;
    }

    tmp = att->attack_modifier - att->defense_modifier + BALANCE_MOD;
    modifier = tmp;
    // This flattens the modifier out a bit preventing crazy modifiers
    // against extreme skills.
    if(modifier > 25)
      modifier = sqrt(modifier * 25);
    else if(modifier < -25)
      modifier = -sqrt(-modifier * 25);
    
    // compare_skills
    result = TASKER->compare_skills(att->attacker, att->attack_skill,
                                    att->defender, att->defense_skill,
                                    modifier,
                                    TM_CONTINUOUS, TM_CONTINUOUS, 1);
    att->result = result->result;
    att->degree = result->degree;

    // Set the last_result data.
    _combat->last_result = att->result;
    (att->defender)->set_last_result(att->result);

    // If they got hit while being defended then reduce the defenders actions
    // and let them defend themselves.
    if((att->result == OFFWIN || att->result == OFFAWARD) &&
       att->defender != att->opponent && !att->repeat) {
      // charge the defender
      (att->defender)->adjust_action_defecit(att->defense_cost);
      (att->defender)->adjust_gp(-(DEFENSE_GP[att->defender_tactics->attitude]));

      // mark them to defend themselves.
      att->defender = att->opponent;
      att->repeat = 1;
    } else
      att->repeat = 0;
    
  } while(att->repeat);

  // calculate damage
  sp_result = call_special(E_DAMAGE_CALCULATION, att);
  if(!sp_result || sp_result[0] & R_CONTINUE || sp_result[0] & R_PASSTHRU)
    att = this_object()->calc_damage(att);
  else if(!(sp_result[0] & R_ABORT) && classp(sp_result[1]))
    att = (class attack)sp_result[1];
  else
    return;

  // see how much damage was stopped by armour.
  sp_result = call_special(E_ARMOUR_CALCULATION, att);
  if(!sp_result || sp_result[0] & R_CONTINUE || sp_result[0] & R_PASSTHRU)
    att = (att->opponent)->calc_armour_protection(att);
  else if(!(sp_result[0] & R_ABORT) && classp(sp_result[1]))
    att = (class attack)sp_result[1];
  else
    return;

#if DEBUG == 1
  combat_debug("Outcome: mod: %d, res: %d, deg: %d dam: %d arm: %d",
               att->attack_modifier - att->defense_modifier + BALANCE_MOD,
               att->result, att->degree, att->damage, att->armour_stopped);
#endif

  // We'll only damage people if the attack succeeded.
  if(att->result == OFFWIN || att->result == OFFAWARD) {
    // and which armour stopped the attack.
    att->stopped_by = (att->person_hit)->query_stopped();

#if DEBUG == 2
    combat_debug("hit: %s, weapon: %s damage: %d, armour: %d, reducing hp by "
                 "%d to %d",
                 (att->person_hit)->query_name(),
                 (att->attack_weapon)->query_name(),
                 att->damage,
                 att->armour_stopped,
                 (att->damage - att->armour_stopped),
                 (att->person_hit)->query_hp());
#endif
  }

  // Prepare the offense/defense messages.
  att = this_object()->prepare_messages(att);
  if(sizeof(att->attack_messages) != 5 || sizeof(att->defense_messages) != 5) {
    debug_printf("No attack or defense messages.\n");
    return;
  }
    
  // Write out the offense/defense messages.
  sp_result = call_special(E_WRITE_MESSAGES, att);
  if(!sp_result || sp_result[0] & R_CONTINUE || sp_result[0] & R_PASSTHRU)
    att = this_object()->write_messages(att);
  else if(!(sp_result[0] & R_ABORT) && classp(sp_result[1]))
    att = (class attack)sp_result[1];
  else
    return;

  // Damage the person hit
  if(att->damage - att->armour_stopped > 0)
    (att->person_hit)->adjust_hp(-(att->damage - att->armour_stopped),
                                 att->attacker, att->attack_weapon,
                                 _combat->last_action);

  // damage the weapon.
  sp_result = call_special(E_WEAPON_DAMAGE, att);
  if(!sp_result || sp_result[0] & R_CONTINUE || sp_result[0] & R_PASSTHRU)
    att = this_object()->damage_weapon(att);
  else if(!(sp_result[0] & R_ABORT) && classp(sp_result[1]))
    att = (class attack)sp_result[1];
  else
    return;

  // Do post attack stuff.
  sp_result = call_special(E_AFTER_ATTACK, att);
  if(!sp_result || sp_result[0] & R_CONTINUE || sp_result[0] & R_PASSTHRU)
    att = this_object()->after_attack(att);
  else if(!(sp_result[0] & R_ABORT) && classp(sp_result[1]))
    att = (class attack)sp_result[1];
  else
    return;
}

/*
 * Figure out who should be in our attacker list and who should be in our
 * hunting list.
 */
void recalc_hunting_list() {
  mixed ob;
  object tmp;

  tmp = 0;
  foreach(ob in keys(_combat->attacker_list)) {
    if(stringp(ob) && find_player(ob))
      tmp = find_player(ob);
    else if(ob && objectp(ob))
      tmp = ob;

    if(!tmp) {
      // This removes the zeros left by dested objects.
      _combat->attacker_list = filter_mapping(_combat->attacker_list,(: $1 :));
    } else if(tmp->query_property("dead") ||
              !environment(tmp) ||
               base_name(environment(tmp)) == "/room/rubbish") {
      map_delete(_combat->attacker_list, ob);
    } else if(environment(tmp) != environment() ||
              !tmp->query_visible(this_object())) {
      // Add to hunting list, remove from attacker list.
      _combat->hunting_list[ob] = time();
      map_delete(_combat->attacker_list, ob);
    }
  }

  tmp = 0;
  foreach(ob in keys(_combat->hunting_list)) {
    if(stringp(ob) && find_player(ob))
      tmp = find_player(ob);
    else if(ob && objectp(ob))
      tmp = ob;

    if(!tmp) {
      // This removes the zeros left by dested objects.
      _combat->hunting_list = filter_mapping(_combat->hunting_list, (: $1 :));
      map_delete(_combat->hunting_list, ob);
    } else if(tmp->query_property("dead") ||
              base_name(environment(tmp)) == "/room/rubbish") {
      map_delete(_combat->hunting_list, ob);
    } else if(environment(tmp) == environment() &&
              tmp->query_visible(this_object())) {
      // If they are here and we can see them move them from the hunted
      // list to the attacker list.
      _combat->attacker_list[ob] = INITIAL_DISTANCE;
      map_delete(_combat->hunting_list, ob);
    } else if(_combat->hunting_list[ob] < time() - HUNTING_TIME) {
      // If we've been hunting them too long stop hunting them.
      this_object()->event_combat(this_object(), "You stop hunting " +
                                  tmp->one_short() + ".\n", ({ }), 1);
      map_delete(_combat->hunting_list, ob);
    }
  }
}

/**
 * Display the hitpoint monitor.
 */
void monitor_points() {
   int hp;
   int max;
   string colour;

#ifdef UNUSED
   this_object()->clear_gp_info();
   if ( (int)this_object()->adjust_gp( -1 ) < 0 )
     return;
#endif

   hp = (int)this_object()->query_hp();
   if(hp < 0)
     hp = 0;
   max = (int)this_object()->query_max_hp();
   switch ( ( hp * 100 ) / max) {
      case 50 .. 100 :
         colour = "%^GREEN%^";
         break;
      case 20 .. 49 :
         colour = "%^YELLOW%^";
         break;
      default :
         colour = "%^RED%^";
   }
   tell_object( this_object(), colour +"Hp: "+ hp +" (" + max + ") Gp: "+
         (int)this_object()->query_gp() +" (" + this_object()->query_max_gp() +
         ") Xp: " + this_object()->query_xp() + "%^RESET%^\n" );
}

/** @ignore yes */
void announce_intent(object opponent) {
  int difficulty = 50;
  object *things, ob;

  if(environment(opponent) != environment(this_object()))
    return;
  
  switch(this_object()->check_dark((int)environment(this_object())->query_light())) {
   case -2:
   case 2:
     difficulty *= 4;
     break;
   case -1:
   case 1:
     difficulty *= 2;
   }

   things = ({ });
   foreach(ob in filter(all_inventory(environment(this_object())),
                        (: $1 &&living($1) :))) {
     switch(TASKER->perform_task(ob, "other.perception", difficulty, TM_FREE)){
     case AWARD:
       tell_object(ob, "%^YELLOW%^You feel very perceptive."
                   "%^RESET%^\n");
     case SUCCEED:
       if(interactive(ob) && !ob->query_verbose("combat"))
         things += ({ ob });
       break;
     case FAIL:
       things += ({ ob });
     }
   }
   
   opponent->add_message((string)this_object()->one_short() +
                      " $V$0=moves,move$V$ aggressively towards you!\n",
                      ({ }));
   event(environment(opponent), "see", (string)this_object()->one_short() +
          " $V$0=moves,move$V$ aggressively towards "+
          (string)opponent->one_short() +"!\n", this_object(),
          ({ opponent, this_object() }) + things);
}

/**
 * This method is called when combat starts.  You can overload this function
 * to make your NPC do things at the start of combat.
 * You do not need to call ::start_combat() since this is just a stub function.
 *
 * @param opponent The first opponent to be attacked.
 */
void start_combat(object opponent) { return; }

/**
 * This method is called when combat ends.  You can overload this function
 * to make your NPC do things at the end of combat.
 * You do not need to call ::end_combat() since this is just a stub function.
 */
void end_combat() { return; }

/** @ignore yes */
void heart_beat() {
  int i;

  // are they in combat?
  if(!_combat->in_combat)
    return;

  recalc_hunting_list();

  if(!sizeof(keys(_combat->attacker_list)) &&
     !sizeof(keys(_combat->hunting_list))) {
    // Not in combat so remove any non-continuous specials.
    for(i=0; i<sizeof(_combat->specials); i++)
      if(!((_combat->specials[i])->type & T_CONTINUOUS))
        _combat->specials -= ({ (_combat->specials)[i] });

    // mark as not in combat
    _combat->in_combat = 0;
    adjust_action_defecit(-MAX_ACTION_DEFECIT);
    end_combat();
    return;
  }

  // Are they in a fit state to fight & do they have anyone to fight?
  if(!query_attackable() || !sizeof(keys(_combat->attacker_list)))
    return;

  // They get part of their combat actions here (assuming they aren't
  // trying to do something else as well) and part later.
  if((!userp(this_object()) ||
     this_object()->query_time_left() == ROUND_TIME) &&
     !this_object()->query_casting_spell()) {
    adjust_action_defecit(-(ACTIONS_PER_HB / (COMBAT_SPEED + 1)));
  }
  
  // Used to control combat speed, especially during testing.
  if(_combat->hbc++ % COMBAT_SPEED)
    return;
  _combat->hbc = 0;

  if(this_object()->query_monitor() &&
     this_object()->query_monitor() <= ++(_combat->mbc)) {
    monitor_points();
    _combat->mbc = 0;
  }

  do_attack();

  if(!this_object()->query_casting_spell()) {
    adjust_action_defecit(-(ACTIONS_PER_HB / (COMBAT_SPEED + 1)));

    // Give a bonus attack if they have no action defecit.
    if(_combat->action_defecit < (OFFENSIVE_DEFECITS[_tactics->attitude] / 2)) {
#if DEBUG == 1
      combat_debug("Adding bonus attack for %s", this_object()->query_name());
      debug_printf("Bonus attack for %s [%d, %d]",
                   this_object()->query_name(), _combat->action_defecit,
                   OFFENSIVE_DEFECITS[_tactics->attitude]);
#endif
      call_out("do_attack", 1);
    }
  }
}

/**
 * Register a combat special.
 *
 * The combat system maintains a list of combat specials. These specials can
 * be used to override or alter any facet of a combat attack (or defense).
 * <p>
 * A special must register what type of special it is:<br>
 * T_OFFENSIVE - an offensive special<br>
 * T_DEFENSIVE - a defensive special<br>
 * T_CONTINUOUS - can be used with one of the above to indicate that the
 *                special is continuous and not a one-time event.<br>
 * <p>
 * It also indicates which stages of combat it wants to be notified of:<br>
 * E_OPPONENT_SELECTION - selection of an opponent<br>
 * E_DEFENDER_SELECTION - selection of the defender<br>
 * E_ATTACK_SELECTION - selecting which attack the attacker will perform<br>
 * E_DEFENSE_SELECTION - selection of the method of defense<br>
 * E_ATTACK_MODIFIER - calculating the modifier for attack (how easy/hard it
 * is)<br>
 * E_DEFENSE_MODIFIER - doing the same for the defense<br>
 * E_DAMAGE_CALCULATION - calculating how much damage the attack can do<br>
 * E_ARMOUR_CALCULATION - calculating how much damage the armour will stop<br>
 * E_WEAPON_DAMAGE - performing damage to the weapons involved<br>
 * E_WRITE_MESSAGES - writing out the success/failure messages<br>
 * E_AFTER_ATTACK - any post-attack cleanup.<br>
 * <p>
 * The special provides the combat system with a callback function to be
 * called and any additional data the special wants tracked by the combat
 * system.
 * <p>
 * Then, when combat reaches one of the stages requested it calls the callback
 * function. This function is passed the stage we're at, all the data for this
 * attack and also the extra data the special requested.
 * <p>
 * The callback can then modify any aspect of the attack data (it could change
 * the attack skill, the defensive weapon, the defense modifier, anything at
 * all) and then return back into combat.
 * <p>
 * The return tells the combat system what to do next. The options are:<br>
 * R_CONTINUE - continue as though the special hadn't done anything.<br>
 * R_DONE - treat this step as completed.<br>
 * R_ABORT - abort the attack<br>
 * R_REMOVE_ME - do one of the above and then remove the special.
 * 
 * @param type Is this an offensive or defensive special.
 * @param events A bitmap of the events this special is interested in.
 * @param the function to be called. A mixed array containing an object & a
 * function string.
 * @return the id of the special.
 *
 * @example
 *
 * This example registers a special that causes the player to be unable to
 * defend themselves for three rounds.
 *
 * id = player->register_special(T_DEFENSIVE | T_CONTINUOUS,
 *                               E_DEFENSE_SELECTION | E_DAMAGE_CALCULATION,
 *                               ({ base_name(this_object()), "callback" }),
 *                               ({ player, 0}));
 *
 * mixed *callback(int stage, class attack att, mixed data) {
 *   // If it's not our player defending we won't do anything.
 *   if(att->defender != data[1])
 *     return ({ R_CONTINUE, att, data });
 *
 *    switch(stage) {
 *    case E_DEFENSE_SELECTION: // We won't let them defend themselves!
 *      att->defense_action = "none";
 *      return ({ R_DONE, att, data });
 *    case E_DAMAGE_CALCULATION:
 *      // If they managed to defend themself, turn it into a failure.
 *      if(att->result == DEFWIN || att->result == DEFAWARD)
 *        att->result = OFFWIN;
 *
 *      // By using R_CONTINUE they take 500 hits _plus_ the normal damage.
 *      att->damage = 500;
 *
 *      // The R_REMOVE_ME causes our special to disappear once we've hurt
 *      // them three times.
 *      if(data[1] > 3)
 *        return ({ R_CONTINUE & R_REMOVE_ME, att, data });
 *
 *      data[1]++;
 *      return ({ R_CONTINUE, att, data });
 *    }
 * }
 */
int register_special(int type, int events, mixed callback, mixed data) {
  _combat->specials += ({ new(class combat_special,
                              id : ++(_combat->special_id),
                              type : type,
                              events : events,
                              callback : callback,
                              data : data) });
  return _combat->special_id;
}

/**
 * Remove a combat special.
 *
 * @param id The id of the special
 * @return 1 for success, 0 for failure
 */
int remove_special(int id) {
  int i;

  for(i=0; i<sizeof(_combat->specials); i++) {
    if((_combat->specials[i])->id == id) {
      _combat->specials -= ({ (_combat->specials)[i] });
      return 1;
    }
  }

  debug_printf("Failed to remove special %d", id);
  return 0;
}

/**
 * List the specials currently registered for this npc/player.
 * @return an array of comba_special classes.
 */
class combat_special *query_specials() {
  return copy(_combat->specials);
}

/**
 * Directly set the special list. Use with care!
 * @param class combat_special *
 */
void set_specials(class combat_special *specials) {
  _combat->specials = specials;
}

/**
 * Update the user data for a specific special.
 * @param id The id of the special
 * @param data The data to be set
 * @return True or false for success or failure
 */
int set_special_data(int id, mixed data) {
  int i;

  for(i=0; i<sizeof(_combat->specials); i++) {
    if((_combat->specials[i])->id == id) {
      _combat->specials[i]->data = data;
      return 1;
    }
  }

  return 0;
}

void adjust_action_defecit(int amount) {
  _combat->action_defecit += amount;
  if(_combat->action_defecit > MAX_ACTION_DEFECIT)
    _combat->action_defecit = MAX_ACTION_DEFECIT;
  else if(_combat->action_defecit < MIN_ACTION_DEFECIT)
    _combat->action_defecit = MIN_ACTION_DEFECIT;
}

int query_action_defecit() { return _combat->action_defecit; }

/**
 * This method returns the current tactics set using the tactics class.
 * @see set_tactics()
 * @index tactics
 */
class tactics query_tactics() {
  init_tactics();
  return copy(_tactics);
}

/**
 * This sets the current attitude to use in combat.
 * @param attitude the new combat attitude
 * @see query_tactics()
 */
void set_tactics(class tactics new_tactics) { _tactics = new_tactics; }

string query_combat_attitude() { return _tactics->attitude; }
void set_combat_attitude(string attitude) { _tactics->attitude = attitude; }
string query_combat_response() { return _tactics->response; }
void set_combat_response(string response) { _tactics->response = response; }
string query_combat_attack() { return _tactics->attack; }
void set_combat_attack(string attack) { _tactics->attack = attack; }
string query_combat_parry() { return _tactics->parry; }
void set_combat_parry(string parry) { _tactics->parry = parry; }
int query_unarmed_parry() { return _tactics->parry_unarmed; }
void set_unarmed_parry(int parry) {  _tactics->parry_unarmed = parry; }
string query_combat_mercy() { return _tactics->mercy; }
void set_combat_mercy(string mercy) { _tactics->mercy = mercy; }
string query_combat_focus() { return _tactics->focus_zone; }
void set_combat_focus(string focus) { _tactics->focus_zone = focus; }
string query_combat_distance() { return _tactics->ideal_distance; }
void set_combat_distance(string distance) { _tactics->ideal_distance = distance; }

/**
 * This method returns the current array of protectors on the living
 * object.  This is the people who are protecting us, so if we are hit
 * make them attack the hitter.
 * @return the current protectors array
 * @see add_protector()
 * @see remove_protector()
 */
object *query_protectors() {
  _combat->protectors -= ({ 0 });
   return copy(_combat->protectors + ({ }));
}

/**
 * This method will add a protector to the current list of protectors
 * for this living object.
 * @param thing the protector to add
 * @see remove_protector()
 * @see query_protectors()
 */
int add_protector( object thing ) {
  if(thing == this_object() ||
     thing->query_property("dead") ||
     member_array(this_object(), thing->query_protectors() +
                  thing->query_defenders()) != -1 ||
     member_array(thing, query_attacker_list())  != -1)
    return 0;
  
  if(member_array(thing, _combat->protectors) == -1)
    _combat->protectors += ({ thing });
  return 1;
}

/**
 * This method will remove a protector to the current list of protectors
 * for this living object.
 * @param thing the protector to remove
 * @return 1 for success, 0 for failure.
 *
 * @see add_protector()
 * @see query_protectors()
 */
int remove_protector(object protector) {
  if(member_array(protector, _combat->protectors) == -1)
    return 0;
  _combat->protectors -= ({ protector });
  return 1;
}

/**
 * This method resets the protector array back to being nothing.
 */
void reset_protectors() {
  _combat->protectors = ({ });
}

/**
 * This method returns the current array of defenders on the living
 * object.  This is the people who are protecting us, so if we are hit
 * make them attack the hitter.
 * @return the current defenders array
 * @see add_protector()
 * @see remove_protector()
 */
object *query_defenders() {
  _combat->defenders -= ({ 0 });
   return copy(_combat->defenders + ({ }));
}

/**
 * This method will add a defender to the current list of defenders
 * for this living object.
 * @param thing the defender to add
 * @see remove_defender()
 * @see query_defenders()
 */
int add_defender( object thing ) {
  if(thing == this_object() ||
     thing->query_property("dead") ||
     member_array(this_object(), thing->query_defenders() +
                  thing->query_protectors()) != -1 ||
     member_array(thing, query_attacker_list())  != -1)
    return 0;

  if(member_array(thing, _combat->defenders) == -1)
    _combat->defenders += ({ thing });
  return 1;
}

/**
 * This method will remove a defender to the current list of defenders
 * for this living object.
 * @param thing the defender to remove
 * @return 1 for success, 0 for failure.
 *
 * @see add_defender()
 * @see query_defenders()
 */
int remove_defender(object defender) {
  if(member_array(defender, _combat->defenders) == -1)
    return 0;
  _combat->defenders -= ({ defender });
  return 1;
}

/**
 * This method resets the defender array back to being nothing.
 */
void reset_defenders() {
  _combat->defenders = ({ });
}

#ifdef USE_DISTANCE
/**
 * Set the distance from this object to the given opponent.
 *
 * @param opponent the object to set the distance to.
 * @param distance the new distance.
 */
int set_distance(object opponent, int distance) {
  if(userp(opponent)) {
    if(!_combat->attacker_list[opponent->query_name()])
      return 0;
    _combat->attacker_list[opponent->query_name()] = distance;
  } else {
    if(!_combat->attacker_list[opponent])
      return 0;
    _combat->attacker_list[opponent] = distance;
  }

  return 1;
}

/**
 * Query the distance from this object to the given opponent.
 *
 * @param opponent the object to check
 */
int query_distance(object opponent) {
  if(userp(opponent)) {
    if(!_combat->attacker_list[opponent->query_name()])
      return 0;
    return _combat->attacker_list[opponent->query_name()];
  } else {
    if(!_combat->attacker_list[opponent])
      return 0;
    return _combat->attacker_list[opponent];
  }
}
#endif

/**
 * This method returns the current list of people in the attacker list
 * on the object.
 * @return the current attacker array
 */
object *query_attacker_list() {
  return uniq_array(map(keys(_combat->attacker_list) +
                        keys(_combat->hunting_list),
                        (: $1 && objectp($1) ? $1 : find_player($1) :)) -
                    ({ 0 }));
}

/**
 * This method removes someone from an attacker/hunting list.
 * @param ob the object to be removed.
 */
void remove_attacker_list(object ob) {
  if(userp(ob)) {
    map_delete(_combat->attacker_list, ob->query_name());
    map_delete(_combat->hunting_list, ob->query_name());
  } else {
    map_delete(_combat->attacker_list, ob);
    map_delete(_combat->hunting_list, ob);
  }
}

/**
 * This method is called when the living object is attacked by some opponent.
 * @param opponent the opponent we are attacked by
 * @return 0 if we cannot attack them, 1 if we can
 * @see query_attacker_list()
 * @see attack_ob()
 */
int attack_by(object opponent) {
  int starting;

  if(!objectp(opponent) ||
     opponent == this_object() ||
     !this_object()->query_attackable() ||
     pk_check(this_object(), opponent))
    return 0;

  // We'll stop protecting them when they attack us.
  if(member_array(opponent, _combat->protectors) != -1)
    _combat->protectors -= ({ opponent });
  if(member_array(opponent, _combat->defenders) != -1)
    _combat->defenders -= ({ opponent });

  if(!sizeof(query_attacker_list()))
    starting = 1;
  
#ifdef USE_DISTANCE
  if(userp(opponent)) {
    if(!_combat->attacker_list[opponent->query_name()])
      _combat->attacker_list[opponent->query_name()] =
        opponent->query_distance(this_object());
  } else if(!_combat->attacker_list[opponent])
    _combat->attacker_list[opponent] = opponent->query_distance(this_object());
#else
  if(userp(opponent)) {
    if(!_combat->attacker_list[opponent->query_name()])
      _combat->attacker_list[opponent->query_name()] = 1;
  } else if(!_combat->attacker_list[opponent])
    _combat->attacker_list[opponent] = 1;
#endif

  if(starting) {
    _combat->action_defecit = (MAX_ACTION_DEFECIT - MIN_ACTION_DEFECIT) / 3;
    start_combat(opponent);
  }

  _combat->in_combat = 1;

  return 1;
}

/**
 * This method is called to make us attack someone else.
 * @param opponent the person to attack
 * @return 0 if we cannot attack them, 1 if we can
 * @see query_attacker_list()
 * @see attack_by()
 */
int attack_ob(object opponent) {
  int starting, new_opponent;

  if(!objectp(opponent) ||
     opponent == this_object() ||
     !opponent->query_attackable() ||
     !this_object()->query_attackable() ||
     pk_check(this_object(), opponent))
    return 0;

  // They stop protecting you when you attack them.
  if(member_array(opponent, _combat->protectors) != -1)
    _combat->protectors -= ({ opponent });
  if(member_array(opponent, _combat->defenders) != -1)
    _combat->defenders -= ({ opponent });

  if(!sizeof(query_attacker_list()))
    starting = 1;
  else if(member_array(opponent, query_attacker_list()) == -1)
    new_opponent = 1;
  
#ifdef USE_DISTANCE
  if(userp(opponent)) {
    if(!_combat->attacker_list[opponent->query_name()])
      _combat->attacker_list[opponent->query_name()] = INITIAL_DISTANCE;
  } else {
    if(!_combat->attacker_list[opponent])
      _combat->attacker_list[opponent] = INITIAL_DISTANCE;
  }
#else
  if(userp(opponent)) {
    if(!_combat->attacker_list[opponent->query_name()])
      _combat->attacker_list[opponent->query_name()] = 1;
  } else {
    if(!_combat->attacker_list[opponent])
      _combat->attacker_list[opponent] = 1;
  }
#endif
  if((new_opponent || starting) && 
     opponent->query_visible(this_object()))
    _callout = call_out("announce_intent", 0, opponent);

  this_object()->remove_hide_invis("hiding");

  if(starting) {
    _combat->action_defecit = (MAX_ACTION_DEFECIT - MIN_ACTION_DEFECIT) / 3;
    start_combat(opponent);
  }

  _combat->in_combat = 1;

  return 1;
}

void stop_fight(object opponent) {
  class combat_special tmp;
#if DEBUG == 2
  combat_debug("stop_fight called against %s", opponent->query_name());
#endif

  if(!objectp(opponent))
    return;

  if(userp(opponent)) {
    map_delete(_combat->attacker_list, opponent->query_name());
    map_delete(_combat->hunting_list, opponent->query_name());
  } else {
    map_delete(_combat->attacker_list, opponent);
    map_delete(_combat->hunting_list, opponent);
  }

#ifdef USE_SURRENDER
  _surrender->from -= ({ 0, opponent });
  _surrender->refusers -= ({ 0, opponent });
  _surrender->to -= ({ 0, opponent });
#endif

  if(!sizeof(query_attacker_list())) {
    // Not in combat so remove any non-continuous specials.
    foreach(tmp in _combat->specials)
      if(!(tmp->type & T_CONTINUOUS))
        remove_special(tmp->id);

    end_combat();
  }
}

void stop_all_fight() {
  class combat_special tmp;

#if DEBUG == 2
  combat_debug("stop_all_fight called");
#endif

  _combat->attacker_list = ([ ]);
  _combat->hunting_list = ([ ]);
  _combat->in_combat = 0;

#ifdef USE_SURRENDER  _surrender->from = ({ });
  _surrender->refusers = ({ });
  _surrender->to = ({ });
#endif

  // Not in combat so remove any non-continuous specials.
  foreach(tmp in _combat->specials)
    if(!(tmp->type & T_CONTINUOUS))
      remove_special(tmp->id);

  end_combat();
}

void stop_hunting(object opponent) {
  if(userp(opponent))
    map_delete(_combat->hunting_list, opponent->query_name());
  else
    map_delete(_combat->hunting_list, opponent);

  if(!sizeof(query_attacker_list()))
    end_combat();
}

#ifdef USE_SURRENDER
/**
 * This method will be called when we surrender.  This does mean that
 * the opponent must have accepted our surrender plea.
 * @return always returns 0
 * @param thing the thing which made us surrender
 */
object do_surrender(object thing) {
   mixed *attackers;
   mixed att;

   if(this_object()->query_property("dead"))
      return 0;

   death_helper(thing, 0);

   catch(DEATH->someone_surrendered(this_object()));
   attackers = query_attacker_list();
   foreach(att in attackers) {
     att->stop_fight(this_object());
   }
   stop_all_fight();

   return 0;
}

int dont_attack_me() {
  if(sizeof(_surrender->to))
    return 1;
  else
    return 0;
}

/**
 * This method tells us what the npc shouuld do in the case that it
 * might possibly surrender.
 * @param victim the person surrendering
 * @param attacker the person beating up the surrenderer
 */
void event_surrender(object victim, object *attackers) {
  object *offer;

  offer = ({ });
  if (this_object() == victim) {
    _surrender->refusers -= ({ 0 });
    _surrender->to -= ({ 0 });
    offer = filter(attackers, (: member_array($1, _surrender->refusers +
                                              _surrender->to) == -1 :));
    if(sizeof(offer)) {
      event(environment(), "combat",
            sprintf("%s kneels down and surrenders to %s.\n",
                    this_object()->one_short(), query_multiple_short(offer)),
            ({ this_object() }) + _surrender->to + _surrender->refusers, 0);

      this_object()->event_combat(this_object(),
                                sprintf("%s kneel down and surrender to %s.\n",
                                        this_object()->one_short(),
                                        query_multiple_short(offer)), ({ }), 0);
      offer->offered_surrender(victim);
      _surrender->to += offer;
    }
  }
}

/**
 * This method returns the current list of people surrendering to us.
 * @return the list of people surrendering
 * @see remove_surrenderer()
 */
object *query_surrenderers() {
  _surrender->from -= ({ 0 });
  return copy(_surrender->from);
}

/**
 * This method removes a person surrendering from our current list.
 * @param victim the person to remove
 * @see query_surrenderers()
 */
void remove_surrenderer(object victim) {
  _surrender->from -= ({ 0, victim });
  return;
}

/**
 * This method is called when someone offers to surrender to the living
 * object.  It will check the current tactics for surrender and then
 * handle the response appropriately.
 * @param victim the person surrendering
 */
void offered_surrender(object victim) {
  string mercy = this_object()->query_combat_mercy();

  switch(mercy) {
  case "ask":
    if (interactive(this_object())) {
      _surrender->from -= ({ 0, victim });
      _surrender->from += ({ victim });
      this_object()->event_combat(this_object(),
                                  victim->one_short() + " has surrendered to "
                                  "you.  Either \"accept " +
                                  victim->query_name() + "\" or \"reject " +
                                  victim->query_name() + ".\n", ({ }), 0);
    } else {
      if (this_object()->query_accept_surrender(victim))
        victim->accepted_surrender(this_object());
      else
        victim->refused_surrender(this_object());
    }
    break;
  case "always":
    this_object()->event_combat(this_object(),
                                "You accept the surrender of " +
                                victim->one_short() + ".\n", ({ }), 0);
    victim->accepted_surrender(this_object());
    break;
  default:
    this_object()->event_combat(this_object(),
                                sprintf("You refuse the surrender of %s.\n",
                                        victim->one_short()), ({ }), 0);
    victim->refused_surrender(this_object());
  }
}

/**
 * This method is called if surrender was refused for the particular
 * person.
 * @param attacker the person who is surrendering
 * @see accepted_surrender()
 */
void refused_surrender(object attacker) {
  _surrender->to -= ({ 0 });
  if (member_array(attacker, _surrender->to) == -1)
    return;
  _surrender->to -= ({ attacker });
  _surrender->refusers -= ({ 0 });
  _surrender->refusers += ({ attacker });

  this_object()->event_combat(this_object(), attacker->one_short() +
                              " refused your surrender\n", ({ }), 0);
}

/**
 * This method is called if surrender was accepted for the particular
 * person.
 * @param attacker the person who is surrendering
 * @see refused_surrender()
 */
void accepted_surrender(object attacker) {
  _surrender->to -= ({ 0 });
  if(member_array(attacker, _surrender->to) == -1) {
    return;
  }
  _surrender->to -= ({ attacker });

  this_object()->event_combat(this_object(), attacker->one_short() +
                              " accepts your surrender.\n", ({ }));
  this_object()->do_surrender(attacker);
}
#endif

/**
 * This method is called when the fight has stopped.  It propogates the
 * stopped fighting event onto all the objects in the room.
 * @param thing the thing which stopped fighting?
 */
void stopped_fighting(object thing) {
  event( environment(), "stopped_fighting", thing );
}

/**
 * This method is called when there is a fight in progress.  It will
 * propogate the event onto all the objects in the room.
 * @param thing the person fighting
 */
void fight_in_progress(object thing) {
  event(environment(), "fight_in_progress", thing );
}

/* Stuff from the old combat shadow */
int set_concentrating(object thing) {
  if(userp(thing)) {
    if(!_combat->attacker_list[thing->query_name()])
      return 0;
#ifdef USE_DISTANCE
    _combat->attacker_list[thing->query_name()] =
      thing->query_distance(this_object());
#else
    _combat->attacker_list[thing->query_name()] = 1;
#endif
  } else {
    if(!_combat->attacker_list[thing])
      return 0;
#ifdef USE_DISTANCE
    _combat->attacker_list[thing] = thing->query_distance(this_object());
#else
    _combat->attacker_list[thing] = 1;
#endif
  }
  _combat->concentrating = thing;

  return 1;
}

object query_concentrating() {
  object thing;

  thing = _combat->concentrating;
  if(!thing || !objectp(thing))
    return 0;

  if(userp(thing) && !_combat->attacker_list[thing->query_name()])
    return 0;
  else if(!_combat->attacker_list[thing])
    return 0;

  return thing;
}

/** @ignore yes
 * This is here for backward compatibility only.  Use query_last_opponent.
 */
object query_attacker() {
  return _combat->last_opponent;
}

/**
 * This method returns the last known opponent we attacked or defended
 * against.  It is kept as up to date as the system can manage and so will
 * include anyone who is currently attacking us or who we are currently
 * attacking.
 *
 * @return object the opponent.
 */
object query_last_opponent() {
  return _combat->last_opponent;
}

/** @ignore yes
 * This is _internal_ to the combat system and should not be used doing so
 * may have odd side-effects.
 */
void set_last_opponent(object last) { _combat->last_opponent = last; }

/**
 * This method returns the last weapon we used for an attack or defense.
 *
 * @return object the weapon.
 */
object query_last_weapon() {
  return _combat->last_weapon;
}

/** @ignore yes
 * This is _internal_ to the combat system and should not be used doing so
 * may have odd side-effects.
 */
void set_last_weapon(object last) { _combat->last_weapon = last; }

/**
 * This method returns the last (combat) action performed.
 *
 * @return string the action.
 */
string query_last_action() { return _combat->last_action; }

/** @ignore yes
 * This is _internal_ to the combat system and should not be used doing so
 * may have odd side-effects.
 */
void set_last_action(string last) { _combat->last_action = last; }

/**
 * This method returns the result of the last combat round.
 *
 * @return int the result.
 */
int query_last_result() { return _combat->last_result; }

/** @ignore yes
 * This is _internal_ to the combat system and should not be used doing so
 * may have odd side-effects.
 */
void set_last_result(int last) { _combat->last_result = last; }

/**
 * This method determines whether or not the object is fighting another
 * object.
 * @param ob the object being tested
 * @param actively is the opponent being actively fought at this time?
 * @return 1 if it is in combat, 0 if it is not.
 */
varargs int is_fighting(object ob, int actively) {
  
  if(!objectp(ob))
    return 0;

  // Only return those things we are actively fighting right now. Things
  // we're hunting or cannot see are excluded.
  if(actively)
    return userp(ob) ? _combat->attacker_list[ob->query_name()] :
      _combat->attacker_list[ob];
    
  return userp(ob) ? _combat->attacker_list[ob->query_name()] ||
    _combat->hunting_list[ob->query_name()] :
    _combat->attacker_list[ob] || _combat->hunting_list[ob];
}

/**
 * Is the player currently fighting anyone.
 *
 * @return 1 for yes, 0 for no.
 */
int query_fighting() {
  if(!environment() || base_name(environment()) == "/room/rubbish")
    return 0;
  
  // Are any of our opponents here? If so we're fighting.
  if(sizeof(filter(query_attacker_list(),
                   (: environment($1) == environment(this_object()) :))))
    return 1;
  
  // Are any of the people here fighting us? If so we're fighting
  if(sizeof(filter(all_inventory(environment(this_object())),
                   (: $1 && living($1) && $1->is_fighting(this_object(), 1) :))))
    return 1;
  
  return 0;
}

/** @ignore yes */
mixed stats() {
  int i;
  int j;
  object *weapons;
  mixed ret;

  init_tactics();

  ret = ({
    ({ "attitude", _tactics->attitude }),
      ({ "response", _tactics->response }),
      ({ "mercy", _tactics->mercy }),
      ({ "parry", _tactics->parry }),
      ({ "attack", _tactics->attack }),
      ({ "unarmed parry", ({ "no", "yes" })[ _tactics->parry_unarmed ] }),
      ({ "attack zone", _tactics->focus_zone ? _tactics->focus_zone :
        "none" }),
      ({ "distance", _tactics->ideal_distance ? _tactics->ideal_distance :
        "none" }),
      
      });
  weapons = (object *)this_object()->query_weapons();
  if(!sizeof(weapons))
    return ret + weapon_logic::stats();

  for ( i = 0; i < sizeof( weapons ); i++ )
    ret += ({ ({ "weapon #"+ i, weapons[ i ]->short() }) }) +
      (mixed)weapons[ j ]->weapon_stats();

  return ret + weapon_logic::stats();
}
