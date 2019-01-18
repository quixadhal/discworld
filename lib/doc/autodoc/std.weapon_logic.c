.DT
weapon_logic.c
Disk World autodoc help
weapon_logic.c

.SH Description
.SP 5 5

This class has alkl the stuff for anything which can damage something
else, this is included into living and weapons.
.EP
.SP 10 5


Written by Pinkfish.

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/weapon.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_attack%^RESET%^
.EI
.SI 5
varargs int add_attack(string a_name, int chance, int * damage, string type, string skill, mixed func, mixed bogus_1, mixed bogus_2)
.EI
.SP 7 5

This method adds an attack onto the object.  The name of the attack
must be unique on each object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
a_name - attack name
.EP
.SP 9 5
change - the chance of it occuring
.EP
.SP 9 5
damage - the damage it will do
.EP
.SP 9 5
type - the type of the attack
.EP
.SP 9 5
skill - the skill used by the attack
.EP
.SP 9 5
func - the special function
.EP
.SP 9 5
bogus_1 - errrr.
.EP
.SP 9 5
bogus_2 - frog.
.EP

.SI 3
* %^BOLD%^add_attack_message%^RESET%^
.EI
.SI 5
void add_attack_message(string name, string type, string * data)
.EI
.SP 7 5

This method adds a special attack message set onto the weapon.  The type
and name are around this way to correspond
to the parameters to add_attack.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the attack (0 for none)
.EP
.SP 9 5
type - the type of attack
.EP
.SP 9 5
data - the attack data
.EP

.SI 3
* %^BOLD%^attack_function%^RESET%^
.EI
.SI 5
void attack_function(string a_name, int damage, object attack_ob, object attack_by)
.EI

.SI 3
* %^BOLD%^calc_attack%^RESET%^
.EI
.SI 5
int calc_attack(int number, int percent)
.EI

.SI 3
* %^BOLD%^modify_damage%^RESET%^
.EI
.SI 5
int modify_damage(int damage, string attack_name)
.EI

.SI 3
* %^BOLD%^query_attack_data%^RESET%^
.EI
.SI 5
mixed * query_attack_data()
.EI

.SI 3
* %^BOLD%^query_attack_message%^RESET%^
.EI
.SI 5
mixed * query_attack_message(string name, string type)
.EI
.SP 7 5

This method returns the attack message associated with the
type and name.  The type and name are around this way to correspond
to the parameters to add_attack.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the attack
.EP
.SP 9 5
type - the type of the attack
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the attack message array, 0 for none

.EP

.SI 3
* %^BOLD%^query_attack_names%^RESET%^
.EI
.SI 5
string * query_attack_names()
.EI

.SI 3
* %^BOLD%^query_attack_types%^RESET%^
.EI
.SI 5
string * query_attack_types()
.EI

.SI 3
* %^BOLD%^query_special_messages%^RESET%^
.EI
.SI 5
mapping query_special_messages()
.EI

.SI 3
* %^BOLD%^query_weapon_type%^RESET%^
.EI
.SI 5
string query_weapon_type()
.EI
.SP 7 5

This method attempts to work out what type of weapon this is.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the weapon type

.EP

.SI 3
* %^BOLD%^remove_attack%^RESET%^
.EI
.SI 5
void remove_attack(string a_name)
.EI
.SP 7 5

This method removes the attack of the given name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
a_name - the name of the attack to remove
.EP

.SI 3
* %^BOLD%^weapon_attacks%^RESET%^
.EI
.SI 5
mixed * weapon_attacks(int percent, object target)
.EI
.SP 7 5

This function returns 0 or more attacks for this weapon against the
given target.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
percent - the attack percentage
.EP
.SP 9 5
target - who is being hit

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of 0 or more attacks. This array is indexed by the
AT_ macros defined in combat.h

.EP


