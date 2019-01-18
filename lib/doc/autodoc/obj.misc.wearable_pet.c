.DT
wearable_pet.c
Disk World autodoc help
wearable_pet.c

.SH Description
.SP 5 5

Note, expand_mon_string will possibly be required to be modified

.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/wearable and /obj/monster.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/armoury.h and /include/gossip.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^adjust_cond%^RESET%^
.EI
.SI 5
int adjust_cond(int i)
.EI

.SI 3
* %^BOLD%^adjust_hp%^RESET%^
.EI
.SI 5
varargs int adjust_hp(int number, object attacker, object weapon, string attack)
.EI

.SI 3
* %^BOLD%^break_me%^RESET%^
.EI
.SI 5
void break_me()
.EI
.SP 7 5

Now to add in the Living/Clothing interfaces. We want all our pets to act as 
clothing worn and take damage, but respond as a living and die if 'broken'

.EP

.SI 3
* %^BOLD%^check_anyone_here%^RESET%^
.EI
.SI 5
int check_anyone_here()
.EI

.SI 3
* %^BOLD%^cond_string%^RESET%^
.EI
.SI 5
string cond_string()
.EI

.SI 3
* %^BOLD%^expand_mon_string%^RESET%^
.EI
.SI 5
void expand_mon_string(mixed str)
.EI

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping map)
.EI

.SI 3
* %^BOLD%^init_static_arg%^RESET%^
.EI
.SI 5
void init_static_arg(mapping map)
.EI

.SI 3
* %^BOLD%^long%^RESET%^
.EI
.SI 5
string long(string str, int dark)
.EI

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^query_static_auto_load%^RESET%^
.EI
.SI 5
mixed query_static_auto_load()
.EI

.SI 3
* %^BOLD%^query_value%^RESET%^
.EI
.SI 5
int query_value()
.EI

.SI 3
* %^BOLD%^set_max_hp%^RESET%^
.EI
.SI 5
int set_max_hp(int number)
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed stats()
.EI

.SI 3
* %^BOLD%^wear_remove_function%^RESET%^
.EI
.SI 5
varargs int wear_remove_function(object pet, object thing)
.EI


