.DT
playtester.c
Disk World autodoc help
playtester.c

.SH Description
.SP 5 5

This is the playtester player object.  It gives the playtesters the
bonus extra commands needed by happy playtesters.
.EP
.SP 10 5


Written by Pinkfish

Started Tue Jun 25 11:27:05 PDT 2002

.EP



.SH See also
.SP 5 5
/global/player.c
.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /global/player.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/command.h, /include/player_handler.h, /include/player.h, /include/login_handler.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^playtester_data%^RESET%^
class playtester_data {
int protect;
int hp;
string log_file;
int log_death;
int log_damage;
int turn_off;
int protection_counter;
int room_protection;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^advancement_restriction%^RESET%^
.EI
.SI 5
int advancement_restriction()
.EI

.SI 3
* %^BOLD%^disable_damage_log%^RESET%^
.EI
.SI 5
void disable_damage_log()
.EI
.SP 7 5

This method disables logging of PT fake damage.

.EP

.SI 3
* %^BOLD%^disable_death_log%^RESET%^
.EI
.SI 5
void disable_death_log()
.EI
.SP 7 5

This method disables logging of PT fake deaths.

.EP

.SI 3
* %^BOLD%^disable_personal_pt_protection%^RESET%^
.EI
.SI 5
int disable_personal_pt_protection()
.EI
.SP 7 5

This method resets the protect flag so that adjust_hp and
do_death are not masked, i.e. the player can be hurt in the
normal(*) way.

(*) Having "normal" ways to hurt people is quite sick...  But it's fnu!

.EP

.SI 3
* %^BOLD%^disable_pt_protection%^RESET%^
.EI
.SI 5
int disable_pt_protection()
.EI
.SP 7 5

This method sets the PT as no longer being under room protection.  Room protection does not 
expire and lasts until it is switched off.  It also over-rides personal protection.  
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure.

.EP

.SI 3
* %^BOLD%^enable_damage_log%^RESET%^
.EI
.SI 5
void enable_damage_log()
.EI
.SP 7 5

This method enables logging of PT fake damage.

.EP

.SI 3
* %^BOLD%^enable_death_log%^RESET%^
.EI
.SI 5
void enable_death_log()
.EI
.SP 7 5

This method enables logging of PT fake deaths.

.EP

.SI 3
* %^BOLD%^enable_personal_pt_protection%^RESET%^
.EI
.SI 5
int enable_personal_pt_protection()
.EI
.SP 7 5

This method sets the protect flag so that adjust_hp and
do_death are masked.

.EP

.SI 3
* %^BOLD%^enable_pt_protection%^RESET%^
.EI
.SI 5
int enable_pt_protection()
.EI
.SP 7 5

This method sets the PT as being under room protection.  Room protection does not 
expire and lasts until it is switched off.  It also over-rides personal protection.  
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure.

.EP

.SI 3
* %^BOLD%^pt_adjust_hp%^RESET%^
.EI
.SI 5
varargs int pt_adjust_hp(int hp, object attacker)
.EI
.SP 7 5

This method adjusts the fake hitpoints.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
hp - The value to add to fake hitpoints
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
hp after adjustment

.EP

.SI 3
* %^BOLD%^pt_do_death%^RESET%^
.EI
.SI 5
object pt_do_death()
.EI
.SP 7 5

This method replaces the normal do_death() when testing.  It gives
players a message about how it would have hurt if it had been for real.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - The object that killed us.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
A corpse

.EP

.SI 3
* %^BOLD%^pt_query_hp%^RESET%^
.EI
.SI 5
int pt_query_hp()
.EI
.SP 7 5

This method returns the number of fake hitpoints.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The number of fake hitpoints

.EP

.SI 3
* %^BOLD%^pt_set_hp%^RESET%^
.EI
.SI 5
void pt_set_hp(int hp)
.EI
.SP 7 5

This method sets the fake hitpoints.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
hp - The new value of fake hitpoints

.EP

.SI 3
* %^BOLD%^query_contractable%^RESET%^
.EI
.SI 5
int query_contractable()
.EI

.SI 3
* %^BOLD%^query_in_pt_arena%^RESET%^
.EI
.SI 5
int query_in_pt_arena()
.EI

.SI 3
* %^BOLD%^query_log_damage%^RESET%^
.EI
.SI 5
int query_log_damage()
.EI
.SP 7 5

This method returns the status of the LOG_DAMAGE flag, i.e. whether
PT fake damage is logged or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Status of LOG_DAMAGE flag

.EP

.SI 3
* %^BOLD%^query_log_death%^RESET%^
.EI
.SI 5
int query_log_death()
.EI
.SP 7 5

This method returns the status of the LOG_DEATH flag, i.e. whether
PT fake deaths are logged or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Status of LOG_DEATH flag

.EP

.SI 3
* %^BOLD%^query_playtester%^RESET%^
.EI
.SI 5
int query_playtester()
.EI
.SP 7 5

This method tells us if the object is a playtester or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the object is a creator, 0 if not.

.EP

.SI 3
* %^BOLD%^query_pt_area%^RESET%^
.EI
.SI 5
int query_pt_area(object ob)
.EI

.SI 3
* %^BOLD%^query_pt_log_file%^RESET%^
.EI
.SI 5
string query_pt_log_file()
.EI
.SP 7 5

This method returns the log file for logging of PT fake deaths and damage.
The default is /d/playtesters/log/pain.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The filename of the log file.

.EP

.SI 3
* %^BOLD%^query_pt_protection%^RESET%^
.EI
.SI 5
int query_pt_protection()
.EI
.SP 7 5

This method returns the status of the protect flag.  2 means that they have 
their room protection enabled, 1 means that they have their personal protection
enabled, 0 means that they are not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The value of the protect flag

.EP

.SI 3
* %^BOLD%^set_pt_log_file%^RESET%^
.EI
.SI 5
void set_pt_log_file(string str)
.EI
.SP 7 5

This method sets the log file for logging of PT fake deaths and damage.
The default is /d/playtesters/log/pain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - The filename of the log file.

.EP


