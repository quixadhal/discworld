.DT
combat.h
Disk World autodoc help
combat.h

.SH Description
.SP 5 5

This class stores all the info about a combat special.

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^attack%^RESET%^
class attack {
object attacker;
class tactics attacker_tactics;
class combat_special * attacker_specials;
object attacker_concentrating;
object attack_weapon;
string attack_action;
string attack_skill;
mixed * attack_data;
int attacker_defecit;
int attack_cost;
int attack_modifier;
object attacker_last_opponent;
object attacker_last_weapon;
string attacker_last_action;
int attacker_last_result;
object opponent;
class combat_special * defender_specials;
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
string * attack_messages;
string * defense_messages;
int verbose;
int repeat_count;
int repeat;
}

.EI
.SP 7 5

This class stores all the information about a given attack. One of these is
created for every attack. It contains attacker and defender information and
is passed in to and back from every special attack.

.EP

.SI 3
* %^BOLD%^combat_information%^RESET%^
class combat_information {
int hbc;
int mbc;
int in_combat;
int special_id;
object * protectors;
object * defenders;
mapping attacker_list;
mapping hunting_list;
object concentrating;
object last_opponent;
object last_weapon;
string last_action;
int last_result;
int action_defecit;
class combat_special * specials;
}

.EI
.SP 7 5

This holds the combat information for this player/npc.

.EP

.SI 3
* %^BOLD%^combat_special%^RESET%^
class combat_special {
int id;
int type;
int events;
mixed callback;
mixed data;
}

.EI

.SI 3
* %^BOLD%^surrender_information%^RESET%^
class surrender_information {
object * refusers;
object * to;
object * from;
}

.EI

.SI 3
* %^BOLD%^tactics%^RESET%^
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

.EI
.SP 7 5

This holds the combat information for this player/npc.

.EP


.SH Defines
.SI 3
* %^BOLD%^ACTIONS_PER_HB%^RESET%^
.EI

.SI 3
* %^BOLD%^ATTACK_COST%^RESET%^
.EI

.SI 3
* %^BOLD%^ATTACK_GP%^RESET%^
.EI
.SP 7 5

How many GPs an attack or defense costs.
This compliments the combat actions by gradually wearing down the
combatant.

.EP

.SI 3
* %^BOLD%^ATTACK_SKILL%^RESET%^
.EI

.SI 3
* %^BOLD%^BALANCE_MOD%^RESET%^
.EI
.SP 7 5

A balance modifier which sways things towards the attacker or the
defender.

Using a negative number makes combat a little less dangerous by tipping
the balance in favour of the defender, a positive number does the opposite.

.EP

.SI 3
* %^BOLD%^COMBAT_ACTION_TIME%^RESET%^
.EI
.SP 7 5

Number of actions per heartbeat, actions used for an attack or defense, and
number of player actions an action uses.

.EP

.SI 3
* %^BOLD%^COMBAT_SPEED%^RESET%^
.EI
.SP 7 5

The speed combat runs at

.EP

.SI 3
* %^BOLD%^DEFENDER_GP%^RESET%^
.EI

.SI 3
* %^BOLD%^DEFENSE_COST%^RESET%^
.EI

.SI 3
* %^BOLD%^DEFENSE_GP%^RESET%^
.EI

.SI 3
* %^BOLD%^DEFENSIVE_DEFECITS%^RESET%^
.EI
.SP 7 5

How far into action defecit someone will go and still try to defend.
Generally speaking people will go farther into defecit in order to
defend themselves than they will to attack.

The defensive numbers are higher than the offensive ones because
people are prone to avoiding pain.

.EP

.SI 3
* %^BOLD%^DISTANCE_STEP%^RESET%^
.EI
.SP 7 5

The size of a distance "step" (in size units)

.EP

.SI 3
* %^BOLD%^DISTANCES%^RESET%^
.EI
.SP 7 5

Map size units to distance strings.

.EP

.SI 3
* %^BOLD%^DODGE%^RESET%^
.EI

.SI 3
* %^BOLD%^E_AFTER_ATTACK%^RESET%^
.EI

.SI 3
* %^BOLD%^E_ARMOUR_CALCULATION%^RESET%^
.EI

.SI 3
* %^BOLD%^E_ATTACK_MODIFIER%^RESET%^
.EI

.SI 3
* %^BOLD%^E_ATTACK_SELECTION%^RESET%^
.EI

.SI 3
* %^BOLD%^E_DAMAGE_CALCULATION%^RESET%^
.EI

.SI 3
* %^BOLD%^E_DEFENDER_SELECTION%^RESET%^
.EI

.SI 3
* %^BOLD%^E_DEFENSE_MODIFIER%^RESET%^
.EI

.SI 3
* %^BOLD%^E_DEFENSE_SELECTION%^RESET%^
.EI

.SI 3
* %^BOLD%^E_OPPONENT_SELECTION%^RESET%^
.EI
.SP 7 5

Events specials can register for

.EP

.SI 3
* %^BOLD%^E_WEAPON_DAMAGE%^RESET%^
.EI

.SI 3
* %^BOLD%^E_WRITE_MESSAGES%^RESET%^
.EI

.SI 3
* %^BOLD%^HUNTING_TIME%^RESET%^
.EI
.SP 7 5

Time until we stop hunting someone.

.EP

.SI 3
* %^BOLD%^INITIAL_DISTANCE%^RESET%^
.EI
.SP 7 5

The initial distance value (in size units)

.EP

.SI 3
* %^BOLD%^M_ATTACKER%^RESET%^
.EI
.SP 7 5

Indices of the attack_messages and defense_messages arrays.

.EP

.SI 3
* %^BOLD%^M_DEFENDER%^RESET%^
.EI

.SI 3
* %^BOLD%^M_OPPONENT%^RESET%^
.EI

.SI 3
* %^BOLD%^M_PERSON_HIT%^RESET%^
.EI

.SI 3
* %^BOLD%^M_SPECTATORS%^RESET%^
.EI

.SI 3
* %^BOLD%^MAX_ACTION_DEFECIT%^RESET%^
.EI
.SP 7 5

Min & Max combat action defecits.

.EP

.SI 3
* %^BOLD%^MIN_ACTION_DEFECIT%^RESET%^
.EI

.SI 3
* %^BOLD%^MOVE_COST%^RESET%^
.EI

.SI 3
* %^BOLD%^MOVE_DISTANCE%^RESET%^
.EI
.SP 7 5

How far from ideal distance someone can be before they attempt to
change their combat distance.

.EP

.SI 3
* %^BOLD%^MOVE_SKILL%^RESET%^
.EI

.SI 3
* %^BOLD%^OFFENSIVE_DEFECITS%^RESET%^
.EI
.SP 7 5

How far into action defecit someone will go and still try to attack

.EP

.SI 3
* %^BOLD%^R_ABORT%^RESET%^
.EI

.SI 3
* %^BOLD%^R_CONTINUE%^RESET%^
.EI

.SI 3
* %^BOLD%^R_DONE%^RESET%^
.EI

.SI 3
* %^BOLD%^R_PASSTHRU%^RESET%^
.EI
.SP 7 5

Return fields for a special

.EP

.SI 3
* %^BOLD%^R_REMOVE_ME%^RESET%^
.EI

.SI 3
* %^BOLD%^REACH%^RESET%^
.EI
.SP 7 5

This is the reach of an object, should be more dynamic than this

.EP

.SI 3
* %^BOLD%^SPECIAL_UNARMED%^RESET%^
.EI

.SI 3
* %^BOLD%^SPECIAL_WEAPON%^RESET%^
.EI

.SI 3
* %^BOLD%^T_CONTINUOUS%^RESET%^
.EI

.SI 3
* %^BOLD%^T_DEFENSIVE%^RESET%^
.EI

.SI 3
* %^BOLD%^T_OFFENSIVE%^RESET%^
.EI
.SP 7 5

Types of special

.EP

.SI 3
* %^BOLD%^TACTICAL_SKILL%^RESET%^
.EI

.SI 3
* %^BOLD%^UNARMED_PARRY%^RESET%^
.EI

.SI 3
* %^BOLD%^WEAPON_PARRY%^RESET%^
.EI
.SP 7 5

Standard skills for attacks and defenses

.EP

