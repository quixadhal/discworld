.DT
weapon.c
Disk World autodoc help
weapon.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/holdable, /std/basic/condition, /std/weapon_logic and /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/virtual.h, /include/weapon.h, /include/move_failures.h, /include/shops/bank.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_immune%^RESET%^
.EI
.SI 5
int add_immune(string name)
.EI

.SI 3
* %^BOLD%^break_me%^RESET%^
.EI
.SI 5
void break_me()
.EI

.SI 3
* %^BOLD%^hit_weapon%^RESET%^
.EI
.SI 5
void hit_weapon(int amount, string type)
.EI

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping map,  object)
.EI

.SI 3
* %^BOLD%^init_static_arg%^RESET%^
.EI
.SI 5
void init_static_arg(mapping map)
.EI

.SI 3
* %^BOLD%^int_query_static_auto_load%^RESET%^
.EI
.SI 5
mapping int_query_static_auto_load()
.EI

.SI 3
* %^BOLD%^modify_damage%^RESET%^
.EI
.SI 5
int modify_damage(int val, string name)
.EI

.SI 3
* %^BOLD%^new_weapon%^RESET%^
.EI
.SI 5
void new_weapon(int new_condition)
.EI
.SP 7 5

This method setups the weapon with the new condition.  It sets the
maximum and lowest conditions to the specified condition.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_condition - the condition value of the weapon

.EP

.SI 3
* %^BOLD%^player_wield%^RESET%^
.EI
.SI 5
void player_wield(int pos)
.EI

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^query_full_value%^RESET%^
.EI
.SI 5
int query_full_value()
.EI

.SI 3
* %^BOLD%^query_money%^RESET%^
.EI
.SI 5
int query_money(string type)
.EI

.SI 3
* %^BOLD%^query_money_array%^RESET%^
.EI
.SI 5
mixed * query_money_array()
.EI

.SI 3
* %^BOLD%^query_static_auto_load%^RESET%^
.EI
.SI 5
mapping query_static_auto_load()
.EI

.SI 3
* %^BOLD%^query_value%^RESET%^
.EI
.SI 5
int query_value()
.EI

.SI 3
* %^BOLD%^query_weapon%^RESET%^
.EI
.SI 5
int query_weapon()
.EI
.SP 7 5

This method return true if it is a weapon.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1

.EP

.SI 3
* %^BOLD%^remove_immune%^RESET%^
.EI
.SI 5
int remove_immune(string name)
.EI

.SI 3
* %^BOLD%^replace_me%^RESET%^
.EI
.SI 5
void replace_me()
.EI

.SI 3
* %^BOLD%^set_wield_func%^RESET%^
.EI
.SI 5
void set_wield_func(string func, mixed ob)
.EI
.SP 7 5

This method sets the wield function of the object.  This will be
called when the object is wielded and unwielded.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to call
.EP
.SP 9 5
ob - the object to call the method on

.EP

.SI 3
* %^BOLD%^short%^RESET%^
.EI
.SI 5
string short(int dark)
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


