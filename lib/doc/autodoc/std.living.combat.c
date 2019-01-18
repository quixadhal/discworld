.DT
combat.c
Disk World autodoc help
combat.c

.SH Description
.SP 5 5

This file contains all the methods and fun things actually run combat.
.EP
.SP 10 5


Written by Ceres


TODO: Add offensive & defensive criticals -- broken limbs etc.


.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/weapon_logic and /std/living/corpse.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/combat.h, /include/attack_messages.h, /include/weapon.h, /include/tasks.h, /include/player.h, /include/living.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^accepted_surrender%^RESET%^
.EI
.SI 5
void accepted_surrender(object attacker)
.EI
.SP 7 5

This method is called if surrender was accepted for the particular
person.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
attacker - the person who is surrendering
.EP

.SI 3
* %^BOLD%^add_defender%^RESET%^
.EI
.SI 5
int add_defender(object thing)
.EI
.SP 7 5

This method will add a defender to the current list of defenders
for this living object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the defender to add
.EP

.SI 3
* %^BOLD%^add_protector%^RESET%^
.EI
.SI 5
int add_protector(object thing)
.EI
.SP 7 5

This method will add a protector to the current list of protectors
for this living object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the protector to add
.EP

.SI 3
* %^BOLD%^adjust_action_defecit%^RESET%^
.EI
.SI 5
void adjust_action_defecit(int amount)
.EI

.SI 3
* %^BOLD%^attack_by%^RESET%^
.EI
.SI 5
int attack_by(object opponent)
.EI
.SP 7 5

This method is called when the living object is attacked by some opponent.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
opponent - the opponent we are attacked by
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if we cannot attack them, 1 if we can
.EP

.SI 3
* %^BOLD%^attack_ob%^RESET%^
.EI
.SI 5
int attack_ob(object opponent)
.EI
.SP 7 5

This method is called to make us attack someone else.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
opponent - the person to attack
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if we cannot attack them, 1 if we can
.EP

.SI 3
* %^BOLD%^calc_armour_protection%^RESET%^
.EI
.SI 5
class attack calc_armour_protection(class attack att)
.EI
.SP 7 5

Calculate how much of the damage is saved by the armour.

PRE: all information has been filled in except the amount of damage
stopped by armour, and which piece of armour did the stopping.
POST: The damage stopped and which armour did it is filled in.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - attack
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
class attack

.EP

.SI 3
* %^BOLD%^calc_defense_modifier%^RESET%^
.EI
.SI 5
class attack calc_defense_modifier(class attack att)
.EI
.SP 7 5

This method calculates the modifier to the defense.

It will take into account all the basic stuff that makes the defense
harder or easier.

PRE: All attack and response data has been filled in.
POST: The defensive modifier must be set.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - attack
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
class attack

.EP

.SI 3
* %^BOLD%^choose_defender%^RESET%^
.EI
.SI 5
class attack choose_defender(class attack att)
.EI
.SP 7 5

This method is used to choose who will defend this attack. Typically this
will be the person who was attacked, however it could be someone else in
some circumstances.

PRE: attacker, attack and defender information has been set.
POST: defender and person_hit must be set.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - attack
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
class attack

.EP

.SI 3
* %^BOLD%^choose_defense%^RESET%^
.EI
.SI 5
class attack choose_defense(class attack att)
.EI
.SP 7 5

Will the defender defend or not? If so, with what skill and weapon and at
what action cost?

PRE: all attack data and opponent and defender data have been set.
POST: defense type, skill and weapon must be set. If defense_action
is "none" the defender will have a big negative modifier.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - attack
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
class attack

.EP

.SI 3
* %^BOLD%^do_attack%^RESET%^
.EI
.SI 5
void do_attack()
.EI
.SP 7 5

This performs the actual attack itself.
All attack data, everything we know about the attacker and the defender
is stored in the attack class which is passed into and back by every
function at every step of the attack.

.EP

.SI 3
* %^BOLD%^do_surrender%^RESET%^
.EI
.SI 5
object do_surrender(object thing)
.EI
.SP 7 5

This method will be called when we surrender.  This does mean that
the opponent must have accepted our surrender plea.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing which made us surrender

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 0
.EP

.SI 3
* %^BOLD%^dont_attack_me%^RESET%^
.EI
.SI 5
int dont_attack_me()
.EI

.SI 3
* %^BOLD%^end_combat%^RESET%^
.EI
.SI 5
void end_combat()
.EI
.SP 7 5

This method is called when combat ends.  You can overload this function
to make your NPC do things at the end of combat.
You do not need to call ::end_combat() since this is just a stub function.

.EP

.SI 3
* %^BOLD%^event_surrender%^RESET%^
.EI
.SI 5
void event_surrender(object victim, object * attackers)
.EI
.SP 7 5

This method tells us what the npc shouuld do in the case that it
might possibly surrender.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
victim - the person surrendering
.EP
.SP 9 5
attacker - the person beating up the surrenderer

.EP

.SI 3
* %^BOLD%^fight_in_progress%^RESET%^
.EI
.SI 5
void fight_in_progress(object thing)
.EI
.SP 7 5

This method is called when there is a fight in progress.  It will
propogate the event onto all the objects in the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the person fighting

.EP

.SI 3
* %^BOLD%^is_fighting%^RESET%^
.EI
.SI 5
varargs int is_fighting(object ob, int actively)
.EI
.SP 7 5

This method determines whether or not the object is fighting another
object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object being tested
.EP
.SP 9 5
actively - is the opponent being actively fought at this time?
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is in combat, 0 if it is not.

.EP

.SI 3
* %^BOLD%^monitor_points%^RESET%^
.EI
.SI 5
void monitor_points()
.EI
.SP 7 5

Display the hitpoint monitor.

.EP

.SI 3
* %^BOLD%^offered_surrender%^RESET%^
.EI
.SI 5
void offered_surrender(object victim)
.EI
.SP 7 5

This method is called when someone offers to surrender to the living
object.  It will check the current tactics for surrender and then
handle the response appropriately.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
victim - the person surrendering

.EP

.SI 3
* %^BOLD%^query_action_defecit%^RESET%^
.EI
.SI 5
int query_action_defecit()
.EI

.SI 3
* %^BOLD%^query_attackable%^RESET%^
.EI
.SI 5
int query_attackable()
.EI
.SP 7 5

Is this object attackable or not?

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 or 0 for true or false.

.EP

.SI 3
* %^BOLD%^query_attacker_list%^RESET%^
.EI
.SI 5
object * query_attacker_list()
.EI
.SP 7 5

This method returns the current list of people in the attacker list
on the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current attacker array

.EP

.SI 3
* %^BOLD%^query_can_attack%^RESET%^
.EI
.SI 5
int query_can_attack()
.EI
.SP 7 5

Decide if we will attack this round or not.  This function can be
overloaded if necessary. It also checks for the property "cannot attack"
on this object. If the property is set the object cannot attack.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if we want to, 0 if we don't.

.EP

.SI 3
* %^BOLD%^query_can_defend%^RESET%^
.EI
.SI 5
int query_can_defend()
.EI
.SP 7 5

Decide if we are able to defend ourselves this time or not.
This function can be overloaded if necessary. It also checks for the
property "cannot defend"
on this object. If the property is set the object cannot defend.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if we want to, 0 if we don't.

.EP

.SI 3
* %^BOLD%^query_combat%^RESET%^
.EI
.SI 5
class combat_information query_combat()
.EI

.SI 3
* %^BOLD%^query_combat_attack%^RESET%^
.EI
.SI 5
string query_combat_attack()
.EI

.SI 3
* %^BOLD%^query_combat_attitude%^RESET%^
.EI
.SI 5
string query_combat_attitude()
.EI

.SI 3
* %^BOLD%^query_combat_distance%^RESET%^
.EI
.SI 5
string query_combat_distance()
.EI

.SI 3
* %^BOLD%^query_combat_focus%^RESET%^
.EI
.SI 5
string query_combat_focus()
.EI

.SI 3
* %^BOLD%^query_combat_mercy%^RESET%^
.EI
.SI 5
string query_combat_mercy()
.EI

.SI 3
* %^BOLD%^query_combat_parry%^RESET%^
.EI
.SI 5
string query_combat_parry()
.EI

.SI 3
* %^BOLD%^query_combat_response%^RESET%^
.EI
.SI 5
string query_combat_response()
.EI

.SI 3
* %^BOLD%^query_concentrating%^RESET%^
.EI
.SI 5
object query_concentrating()
.EI

.SI 3
* %^BOLD%^query_defend%^RESET%^
.EI
.SI 5
int query_defend()
.EI
.SP 7 5

Is this object prepared to try to defend someone else at this time?

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 or 0 for true or false.

.EP

.SI 3
* %^BOLD%^query_defenders%^RESET%^
.EI
.SI 5
object * query_defenders()
.EI
.SP 7 5

This method returns the current array of defenders on the living
object.  This is the people who are protecting us, so if we are hit
make them attack the hitter.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current defenders array
.EP

.SI 3
* %^BOLD%^query_distance%^RESET%^
.EI
.SI 5
int query_distance(object opponent)
.EI
.SP 7 5

Query the distance from this object to the given opponent.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
opponent - the object to check

.EP

.SI 3
* %^BOLD%^query_fighting%^RESET%^
.EI
.SI 5
int query_fighting()
.EI
.SP 7 5

Is the player currently fighting anyone.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for yes, 0 for no.

.EP

.SI 3
* %^BOLD%^query_last_action%^RESET%^
.EI
.SI 5
string query_last_action()
.EI
.SP 7 5

This method returns the last (combat) action performed.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
string the action.

.EP

.SI 3
* %^BOLD%^query_last_opponent%^RESET%^
.EI
.SI 5
object query_last_opponent()
.EI
.SP 7 5

This method returns the last known opponent we attacked or defended
against.  It is kept as up to date as the system can manage and so will
include anyone who is currently attacking us or who we are currently
attacking.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
object the opponent.

.EP

.SI 3
* %^BOLD%^query_last_result%^RESET%^
.EI
.SI 5
int query_last_result()
.EI
.SP 7 5

This method returns the result of the last combat round.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
int the result.

.EP

.SI 3
* %^BOLD%^query_last_weapon%^RESET%^
.EI
.SI 5
object query_last_weapon()
.EI
.SP 7 5

This method returns the last weapon we used for an attack or defense.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
object the weapon.

.EP

.SI 3
* %^BOLD%^query_protect%^RESET%^
.EI
.SI 5
int query_protect()
.EI
.SP 7 5

Is this object prepared to try to protect someone else at this time?
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 or 0 for true or false.

.EP

.SI 3
* %^BOLD%^query_protectors%^RESET%^
.EI
.SI 5
object * query_protectors()
.EI
.SP 7 5

This method returns the current array of protectors on the living
object.  This is the people who are protecting us, so if we are hit
make them attack the hitter.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current protectors array
.EP

.SI 3
* %^BOLD%^query_specials%^RESET%^
.EI
.SI 5
class combat_special * query_specials()
.EI
.SP 7 5

List the specials currently registered for this npc/player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of comba_special classes.

.EP

.SI 3
* %^BOLD%^query_surrender%^RESET%^
.EI
.SI 5
class surrender_information query_surrender()
.EI

.SI 3
* %^BOLD%^query_surrenderers%^RESET%^
.EI
.SI 5
object * query_surrenderers()
.EI
.SP 7 5

This method returns the current list of people surrendering to us.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of people surrendering
.EP

.SI 3
* %^BOLD%^query_tactics%^RESET%^
.EI
.SI 5
class tactics query_tactics()
.EI
.SP 7 5

This method returns the current tactics set using the tactics class.
.EP

.SI 3
* %^BOLD%^query_unarmed_parry%^RESET%^
.EI
.SI 5
int query_unarmed_parry()
.EI

.SI 3
* %^BOLD%^recalc_hunting_list%^RESET%^
.EI
.SI 5
void recalc_hunting_list()
.EI

.SI 3
* %^BOLD%^refused_surrender%^RESET%^
.EI
.SI 5
void refused_surrender(object attacker)
.EI
.SP 7 5

This method is called if surrender was refused for the particular
person.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
attacker - the person who is surrendering
.EP

.SI 3
* %^BOLD%^register_special%^RESET%^
.EI
.SI 5
int register_special(int type, int events, mixed callback, mixed data)
.EI
.SP 7 5

Register a combat special.

The combat system maintains a list of combat specials. These specials can
be used to override or alter any facet of a combat attack (or defense).


A special must register what type of special it is:
.EP
.SP 7 5
T_OFFENSIVE - an offensive special
.EP
.SP 7 5
T_DEFENSIVE - a defensive special
.EP
.SP 7 5
T_CONTINUOUS - can be used with one of the above to indicate that the special is continuous and not a one-time event.
.EP
.SP 7 5


It also indicates which stages of combat it wants to be notified of:
.EP
.SP 7 5
E_OPPONENT_SELECTION - selection of an opponent
.EP
.SP 7 5
E_DEFENDER_SELECTION - selection of the defender
.EP
.SP 7 5
E_ATTACK_SELECTION - selecting which attack the attacker will perform
.EP
.SP 7 5
E_DEFENSE_SELECTION - selection of the method of defense
.EP
.SP 7 5
E_ATTACK_MODIFIER - calculating the modifier for attack (how easy/hard it is)
.EP
.SP 7 5
E_DEFENSE_MODIFIER - doing the same for the defense
.EP
.SP 7 5
E_DAMAGE_CALCULATION - calculating how much damage the attack can do
.EP
.SP 7 5
E_ARMOUR_CALCULATION - calculating how much damage the armour will stop
.EP
.SP 7 5
E_WEAPON_DAMAGE - performing damage to the weapons involved
.EP
.SP 7 5
E_WRITE_MESSAGES - writing out the success/failure messages
.EP
.SP 7 5
E_AFTER_ATTACK - any post-attack cleanup.
.EP
.SP 7 5


The special provides the combat system with a callback function to be called and any additional data the special wants tracked by the combat system. 

Then, when combat reaches one of the stages requested it calls the callback function. This function is passed the stage we're at, all the data for this attack and also the extra data the special requested. 

The callback can then modify any aspect of the attack data (it could change the attack skill, the defensive weapon, the defense modifier, anything at all) and then return back into combat. 

The return tells the combat system what to do next. The options are:
.EP
.SP 7 5
R_CONTINUE - continue as though the special hadn't done anything.
.EP
.SP 7 5
R_DONE - treat this step as completed.
.EP
.SP 7 5
R_ABORT - abort the attack
.EP
.SP 7 5
R_REMOVE_ME - do one of the above and then remove the special. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - Is this an offensive or defensive special.
.EP
.SP 9 5
events - A bitmap of the events this special is interested in.
.EP
.SP 9 5
the - function to be called. A mixed array containing an object & a
function string.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the id of the special.

.EP

.SI 3
* %^BOLD%^remove_attacker_list%^RESET%^
.EI
.SI 5
void remove_attacker_list(object ob)
.EI
.SP 7 5

This method removes someone from an attacker/hunting list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to be removed.

.EP

.SI 3
* %^BOLD%^remove_defender%^RESET%^
.EI
.SI 5
int remove_defender(object defender)
.EI
.SP 7 5

This method will remove a defender to the current list of defenders
for this living object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the defender to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure.

.EP

.SI 3
* %^BOLD%^remove_protector%^RESET%^
.EI
.SI 5
int remove_protector(object protector)
.EI
.SP 7 5

This method will remove a protector to the current list of protectors
for this living object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the protector to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure.

.EP

.SI 3
* %^BOLD%^remove_special%^RESET%^
.EI
.SI 5
int remove_special(int id)
.EI
.SP 7 5

Remove a combat special.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - The id of the special
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure

.EP

.SI 3
* %^BOLD%^remove_surrenderer%^RESET%^
.EI
.SI 5
void remove_surrenderer(object victim)
.EI
.SP 7 5

This method removes a person surrendering from our current list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
victim - the person to remove
.EP

.SI 3
* %^BOLD%^reset_defenders%^RESET%^
.EI
.SI 5
void reset_defenders()
.EI
.SP 7 5

This method resets the defender array back to being nothing.

.EP

.SI 3
* %^BOLD%^reset_protectors%^RESET%^
.EI
.SI 5
void reset_protectors()
.EI
.SP 7 5

This method resets the protector array back to being nothing.

.EP

.SI 3
* %^BOLD%^set_combat_attack%^RESET%^
.EI
.SI 5
void set_combat_attack(string attack)
.EI

.SI 3
* %^BOLD%^set_combat_attitude%^RESET%^
.EI
.SI 5
void set_combat_attitude(string attitude)
.EI

.SI 3
* %^BOLD%^set_combat_distance%^RESET%^
.EI
.SI 5
void set_combat_distance(string distance)
.EI

.SI 3
* %^BOLD%^set_combat_focus%^RESET%^
.EI
.SI 5
void set_combat_focus(string focus)
.EI

.SI 3
* %^BOLD%^set_combat_mercy%^RESET%^
.EI
.SI 5
void set_combat_mercy(string mercy)
.EI

.SI 3
* %^BOLD%^set_combat_parry%^RESET%^
.EI
.SI 5
void set_combat_parry(string parry)
.EI

.SI 3
* %^BOLD%^set_combat_response%^RESET%^
.EI
.SI 5
void set_combat_response(string response)
.EI

.SI 3
* %^BOLD%^set_concentrating%^RESET%^
.EI
.SI 5
int set_concentrating(object thing)
.EI

.SI 3
* %^BOLD%^set_distance%^RESET%^
.EI
.SI 5
int set_distance(object opponent, int distance)
.EI
.SP 7 5

Set the distance from this object to the given opponent.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
opponent - the object to set the distance to.
.EP
.SP 9 5
distance - the new distance.

.EP

.SI 3
* %^BOLD%^set_special_data%^RESET%^
.EI
.SI 5
int set_special_data(int id, mixed data)
.EI
.SP 7 5

Update the user data for a specific special.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - The id of the special
.EP
.SP 9 5
data - The data to be set
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
True or false for success or failure

.EP

.SI 3
* %^BOLD%^set_specials%^RESET%^
.EI
.SI 5
void set_specials(class combat_special * specials)
.EI
.SP 7 5

Directly set the special list. Use with care!
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - combat_special *

.EP

.SI 3
* %^BOLD%^set_tactics%^RESET%^
.EI
.SI 5
void set_tactics(class tactics new_tactics)
.EI
.SP 7 5

This sets the current attitude to use in combat.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
attitude - the new combat attitude
.EP

.SI 3
* %^BOLD%^set_unarmed_parry%^RESET%^
.EI
.SI 5
void set_unarmed_parry(int parry)
.EI

.SI 3
* %^BOLD%^start_combat%^RESET%^
.EI
.SI 5
void start_combat(object opponent)
.EI
.SP 7 5

This method is called when combat starts.  You can overload this function
to make your NPC do things at the start of combat.
You do not need to call ::start_combat() since this is just a stub function.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
opponent - The first opponent to be attacked.

.EP

.SI 3
* %^BOLD%^stop_all_fight%^RESET%^
.EI
.SI 5
void stop_all_fight()
.EI

.SI 3
* %^BOLD%^stop_fight%^RESET%^
.EI
.SI 5
void stop_fight(object opponent)
.EI

.SI 3
* %^BOLD%^stop_hunting%^RESET%^
.EI
.SI 5
void stop_hunting(object opponent)
.EI

.SI 3
* %^BOLD%^stopped_fighting%^RESET%^
.EI
.SI 5
void stopped_fighting(object thing)
.EI
.SP 7 5

This method is called when the fight has stopped.  It propogates the
stopped fighting event onto all the objects in the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing which stopped fighting?

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^adjust_actions%^RESET%^
.EI
.SI 5
void adjust_actions(class attack att)
.EI
.SP 7 5

Adjust the time left, combat actions and gp used by this attack.
This calls adjust_action_defecit in both the attacker and defender
as appropriate.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - attack

.EP

.SI 3
* %^BOLD%^after_attack%^RESET%^
.EI
.SI 5
class attack after_attack(class attack att)
.EI
.SP 7 5

This function is called after it's all over and done (just before the
costs are charged). It can be used to do any special outcomes that
we may decide on.
Note that if you override this function in a special be absolutely certain
you know what you're doing if you return R_ABORT or R_DONE. Use of either
of these two could cause really nasty side-effects.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - attack
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
class attack

.EP

.SI 3
* %^BOLD%^calc_attack_modifier%^RESET%^
.EI
.SI 5
class attack calc_attack_modifier(class attack att)
.EI
.SP 7 5

This method calculates the modifier to the attack.

It will take into account all the basic stuff that makes the attack
harder or easier.

PRE: All attack and response data has been filled in.
POST: The offensive modifier must be set.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - attack
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
class attack

.EP

.SI 3
* %^BOLD%^calc_damage%^RESET%^
.EI
.SI 5
class attack calc_damage(class attack att)
.EI
.SP 7 5

After a successful attack (or possibly a parried one) calculate the
amount of damage done to the opponent.

This method must fill in the amount of damage to be done and may adjust
the time taken too.

PRE: the outcome of an attack must have been determined.
POST: damage must be filled in.

.EP

.SI 3
* %^BOLD%^call_special%^RESET%^
.EI
.SI 5
mixed * call_special(int stage, class attack att)
.EI
.SP 7 5

Go through the list of the attacker & opponents specials and
call any that have registered for this stage.

.EP

.SI 3
* %^BOLD%^choose_attack%^RESET%^
.EI
.SI 5
class attack choose_attack(class attack att)
.EI
.SP 7 5

Determine which attack they'll use.

This picks a weapon, gets the attack data and the weapon skill and
determines how much the attack will cost.

PRE: attacker info, opponent and defender info have been set.
POST: attack weapon, attack skill, attack data, and attack_cost must
be set.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - attack
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
class attack

.EP

.SI 3
* %^BOLD%^choose_opponent%^RESET%^
.EI
.SI 5
class attack choose_opponent(class attack att)
.EI
.SP 7 5

Choose an opponent and attack them.

This method must fill in the opponent variable. If no opponent is chosen
the attack will be aborted.

PRE: Attacker information has been filled in.
POST: opponent must be chosen or the attack will be aborted.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - attack
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
class attack

.EP

.SI 3
* %^BOLD%^damage_weapon%^RESET%^
.EI
.SI 5
class attack damage_weapon(class attack att)
.EI
.SP 7 5

Do damage to the attacking & defending weapons.

PRE: all information has been filled in
POST: the weapons involved have taken appropriate damage.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - attack
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
class attack

.EP

.SI 3
* %^BOLD%^perform_movement%^RESET%^
.EI
.SI 5
class attack perform_movement(class attack att)
.EI
.SP 7 5

This function is called as part of the post attack processing and performs
the checks to see if someone can move closer or further away from their
opponent.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - attack
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
class attack
.EP

.SI 3
* %^BOLD%^prepare_messages%^RESET%^
.EI
.SI 5
class attack prepare_messages(class attack att)
.EI
.SP 7 5

Prepare the result messages.

This method prepares the attack and defense messages setting up the 5
element arrays of att_mess & def_mess. The elements are as follows:
0 - the attacker, 1 - the opponent, 2 - everyone else, 3 - the defender
4 - the person hit

PRE: The attack has been done and the amount of damage done and stopped
has been calculated.
POST: The 5 element att_mess & def_mess arrays are populated.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - attack

.EP

.SI 3
* %^BOLD%^write_messages%^RESET%^
.EI
.SI 5
class attack write_messages(class attack att)
.EI
.SP 7 5

Write out the attack/defense messages.

PRE: all information has been filled in
POST: the appropriate messages have been written out.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
class - attack
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
class attack

.EP


