#ifndef __SYS_COMBAT
#define __SYS_COMBAT

#include <player.h>

class surrender_information {
   object *refusers;
   object *to;
   object *from;
}

/**
 * This class stores all the info about a combat special.
 *
 * @param id A unique ID given to this special
 * @param type The type of special (offensive, defensive, continuous)
 * @param events The list of events this special wants to be notified for
 * @param callback The callback data.
 * @param data Any additional data the special registered.
 */
class combat_special {
  int id;
  int type;
  int events;
  mixed callback;
  mixed data; // this can be used to store anything the special wants to store.
}

/**
 * This holds the combat information for this player/npc.
 */
class combat_information {
  int hbc;
  int mbc;                      // Monitor count
  int in_combat;
  int special_id;
  object *protectors;
  object *defenders;
  mapping attacker_list;
  mapping hunting_list;
  object concentrating;
  object last_opponent;
  object last_weapon;
  string last_action;
  int last_result;
  int action_defecit;
  class combat_special *specials;
}

class tactics {
  string attitude;
  string response;
  string parry;
  string attack;
  int parry_unarmed;
  string mercy;
  string focus_zone;
  string ideal_distance;
}

/**
 * This class stores all the information about a given attack. One of these is
 * created for every attack. It contains attacker and defender information and
 * is passed in to and back from every special attack.
 *
 * @param attacker the attacking object
 * @param attacker_tactics a class for the attackers tactics
 * @param attacker_specials a list of specials registered for the attacker
 * @param attacker_concentrating who the atacker is concentrating on
 * @param attack_weapon the attacking weapon
 * @param attack_action the attackers action
 * @param attack_skill the skill being used for the attack
 * @param attack_data the attack data for the weapon
 * @param attacker_defecit how far into action defecit the attacker is
 * @param attack_cost the action cost for the attack
 * @param attack_modifier the difficulty modifier for the attack
 * @param attacker_last_opponent the last opponent the attacker fought
 * @param attacker_last_weapon the last weapon the attacker used
 * @param attacker_last_action the last combat action by the attacker
 * @param attacker_last_result the result of the last action
 *
 * @param opponent the opponent object (the person who was attacked and who
 *                                      will take any damage)
 * @param defender_specials a list of specials registered for the defender
 * @param defender The defending object (the person who will perform the
 *                                       defense against the attack)
 * @paran distance length units between attacker and the opponent
 * @param person_hit Someone who will jump in the way of the attack
 * @param defender_tactics a class for the defenders tactics
 * @param defender_concentrating who the defender is concentrating on
 * @param defense_weapon the defending weapon
 * @param defense_limb if parrying unarmed which limb are they using
 * @param defense_action the defenders action
 * @param defense_skill the skill being used for the defense
 * @param defender_defecit how far into action defecit the defender is
 * @param defense_cost the action cost for the defense
 * @param defense_modifier the difficulty modifier for the defense
 * @param defender_last_opponent the last person the defender fought
 * @param defender_last_weapon the last weapon the defender used
 * @param defender_last_action the last combat action by the defender
 * @param defender_last_result the result of the last action
 *
 * @param damage how much damage will be done
 * @param armour_stopped how much damage was stopped by armour
 * @param stopped_by which armour stopped the most damage
 * @param target_zone which part of the opponents body is being hit
 * @param result the outcome (OFFAWARD, OFFWIN, DEFAWARD, DEFWIN)
 * @param degree the degree of the outcome (TASKER_CRITICAL,
 *                                          TASKER_EXCEPTIONAL,
 *                                          TASKER_NORMAL,
 *                                          TASKER_MARGINAL)
 *
 * @param attack_messages messages to be shown about the attack
 * @param defense_messages messages to be shown about the defense
 * @param verbose should the messages be considered verbose (ie. only to be
 *        displayed to people with verbose combat turned on).
 * @param repeat should we go through the attack loop again?
 *
 */
class attack {
  object attacker;
  class tactics attacker_tactics;
  class combat_special *attacker_specials;
  object attacker_concentrating;
  object attack_weapon;
  string attack_action;
  string attack_skill;
  mixed *attack_data;
  int attacker_defecit;
  int attack_cost;
  int attack_modifier;
  object attacker_last_opponent;
  object attacker_last_weapon;
  string attacker_last_action;
  int attacker_last_result;

  object opponent;
  class combat_special *defender_specials;
  object person_hit;
  object defender;
  int distance;
  class tactics defender_tactics;
  object defender_concentrating;
  object defense_weapon;
  string defense_limb;
  string defense_action;
  string defense_skill;
  int defender_defecit;
  int defense_cost;
  int defender_cost;
  int protector_cost;
  int defense_modifier;
  object defender_last_opponent;
  object defender_last_weapon;
  string defender_last_action;
  int defender_last_result;

  int damage;
  int armour_stopped;
  mixed stopped_by;
  string target_zone;
  int result;
  int degree;
  object corpse;
  
  string *attack_messages;
  string *defense_messages;
  int verbose;

  int repeat_count;
  int repeat;
}

/**
 * Standard skills for attacks and defenses
 */
#define WEAPON_PARRY "fighting.combat.parry.melee"
#define UNARMED_PARRY "fighting.combat.parry.melee"
#define DODGE "fighting.combat.dodging.melee"
#define ATTACK_SKILL "fighting.combat.melee."
#define MOVE_SKILL "fighting.combat.special.tactics"
#define TACTICAL_SKILL "fighting.combat.special.tactics"
#define SPECIAL_WEAPON "fighting.combat.special.weapon"
#define SPECIAL_UNARMED "fighting.combat.special.unarmed"

/**
 * The speed combat runs at
 */
#undef COMBAT_SPEED
#define COMBAT_SPEED 1

/**
 * Number of actions per heartbeat, actions used for an attack or defense, and
 * number of player actions an action uses.
 */
#define COMBAT_ACTION_TIME (environment() ? environment()->attack_speed() : 15)
#define ACTIONS_PER_HB DEFAULT_TIME
#define ATTACK_COST (COMBAT_ACTION_TIME * 4) / 3
#define DEFENSE_COST (COMBAT_ACTION_TIME * 2) / 3
#define MOVE_COST (COMBAT_ACTION_TIME / 4)

/**
 * How many GPs an attack or defense costs.
 * This compliments the combat actions by gradually wearing down the
 * combatant.
 */
#define ATTACK_GP ([ "insane" : 1, \
                     "offensive" : 0, \
                     "neutral" : 0,\
                     "defensive" : 1, \
                     "wimp" : 2 ])

#define DEFENSE_GP ([ "insane" : 2, \
                     "offensive" : 1, \
                     "neutral" : 0,\
                     "defensive" : 0, \
                     "wimp" : 1 ])

#define DEFENDER_GP ([ "insane" : 4, \
                       "offensive" : 2, \
                       "neutral" : 1,\
                       "defensive" : 2, \
                       "wimp" : 3 ])

/**
 * A balance modifier which sways things towards the attacker or the
 * defender.
 *
 * Using a negative number makes combat a little less dangerous by tipping
 * the balance in favour of the defender, a positive number does the opposite.
 */
//#define BALANCE_MOD (random(random(100)) - 50)
#define BALANCE_MOD (random(random(75)) - 25)

/**
 * Time until we stop hunting someone.
 */
#define HUNTING_TIME 300

/**
 * The initial distance value (in size units)
 */
#define INITIAL_DISTANCE 45

/**
 * The size of a distance "step" (in size units)
 */
#define DISTANCE_STEP 9

/**
 * This is the reach of an object, should be more dynamic than this
 */
#define REACH 18

/**
 * Map size units to distance strings.
 */
#define DISTANCES ([ "ranged" : 45, "long" : 36, "medium" : 27, "close" : 18,\
                     "hand-to-hand" : 9 ])

/**
 * Types of special
 */
#define T_OFFENSIVE 1
#define T_DEFENSIVE 2
#define T_CONTINUOUS 4

/**
 * Events specials can register for
 */
#define E_OPPONENT_SELECTION 1
#define E_DEFENDER_SELECTION 2
#define E_ATTACK_SELECTION 4
#define E_DEFENSE_SELECTION 8
#define E_ATTACK_MODIFIER 16
#define E_DEFENSE_MODIFIER 32
#define E_DAMAGE_CALCULATION 64
#define E_ARMOUR_CALCULATION 128
#define E_WEAPON_DAMAGE 256
#define E_WRITE_MESSAGES 512
#define E_AFTER_ATTACK 1024

/**
 * Return fields for a special
 */
#define R_PASSTHRU 1
#define R_CONTINUE 2
#define R_DONE 4
#define R_ABORT 8
#define R_REMOVE_ME 16

/**
 * Min & Max combat action defecits.
 */
#define MAX_ACTION_DEFECIT 100
#define MIN_ACTION_DEFECIT 0

/**
 * Indices of the attack_messages and defense_messages arrays.
 */
#define M_ATTACKER 0
#define M_OPPONENT 1
#define M_SPECTATORS 2
#define M_DEFENDER 3
#define M_PERSON_HIT 4

/**
 * How far into action defecit someone will go and still try to attack
 */
#define OFFENSIVE_DEFECITS ([ "insane" : (COMBAT_ACTION_TIME * 2), \
                              "offensive" : (COMBAT_ACTION_TIME), \
                              "neutral" : (COMBAT_ACTION_TIME * 2 / 3),\
                              "defensive" : (COMBAT_ACTION_TIME / 2), \
                              "wimp" : (COMBAT_ACTION_TIME / 4) ])

/**
 * How far into action defecit someone will go and still try to defend.
 * Generally speaking people will go farther into defecit in order to
 * defend themselves than they will to attack.
 *
 * The defensive numbers are higher than the offensive ones because
 * people are prone to avoiding pain.
 */
#define DEFENSIVE_DEFECITS (["insane" : (COMBAT_ACTION_TIME / 2), \
                             "offensive" : (COMBAT_ACTION_TIME), \
                             "neutral" : (COMBAT_ACTION_TIME * 3 / 2), \
                             "defensive" : (COMBAT_ACTION_TIME * 2), \
                             "wimp" : (COMBAT_ACTION_TIME * 3) ])

/**
 * How far from ideal distance someone can be before they attempt to
 * change their combat distance.
 */
#define MOVE_DISTANCE ([ "insane" : 36, "offensive" : 27, "neutral" : 9,\
                         "defensive" : 0, "wimp" : 0 ])
#endif
