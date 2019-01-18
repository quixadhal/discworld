.DT
armour_logic.c
Disk World autodoc help
armour_logic.c

.SH Includes
.SP 5 5
This class includes the following files /include/weapon_old.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_ac%^RESET%^
.EI
.SI 5
int add_ac(string name, string type, mixed a_c)
.EI

.SI 3
* %^BOLD%^calc_string%^RESET%^
.EI
.SI 5
string calc_string(mixed b)
.EI

.SI 3
* %^BOLD%^calc_value%^RESET%^
.EI
.SI 5
int calc_value(mixed arr)
.EI

.SI 3
* %^BOLD%^query_ac%^RESET%^
.EI
.SI 5
varargs int query_ac(string type, int dam, string zone)
.EI

.SI 3
* %^BOLD%^query_armour_class%^RESET%^
.EI
.SI 5
mapping query_armour_class()
.EI

.SI 3
* %^BOLD%^remove_ac%^RESET%^
.EI
.SI 5
int remove_ac(string name)
.EI

.SI 3
* %^BOLD%^set_ac%^RESET%^
.EI
.SI 5
void set_ac(mixed * bing)
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


