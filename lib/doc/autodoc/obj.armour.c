.DT
armour.c
Disk World autodoc help
armour.c

.SH Description
.SP 5 5

This function is called on an object and causes it to be worn
if it can be by it's environment(), i.e the player carrying it.

.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/wearable, /std/object and /std/armour_logic.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/virtual.h and /include/move_failures.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^long%^RESET%^
.EI
.SI 5
string long(string word, int dark)
.EI

.SI 3
* %^BOLD%^player_wear%^RESET%^
.EI
.SI 5
void player_wear()
.EI

.SI 3
* %^BOLD%^query_ac%^RESET%^
.EI
.SI 5
varargs int query_ac(string type, int amount, string zone)
.EI

.SI 3
* %^BOLD%^query_armour%^RESET%^
.EI
.SI 5
int query_armour()
.EI

.SI 3
* %^BOLD%^query_full_value%^RESET%^
.EI
.SI 5
int query_full_value()
.EI

.SI 3
* %^BOLD%^query_value%^RESET%^
.EI
.SI 5
int query_value()
.EI

.SI 3
* %^BOLD%^replace_me%^RESET%^
.EI
.SI 5
void replace_me()
.EI

.SI 3
* %^BOLD%^setup_armour%^RESET%^
.EI
.SI 5
void setup_armour(int number)
.EI


