.DT
health.c
Disk World autodoc help
health.c

.SH Description
.SP 5 5

Returns the current value of the callingdeath variable.

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/drinks.h and /include/living.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^adjust_gp%^RESET%^
.EI
.SI 5
int adjust_gp(int number)
.EI

.SI 3
* %^BOLD%^adjust_hp%^RESET%^
.EI
.SI 5
varargs int adjust_hp(int number, object attacker, object weapon, string attack)
.EI

.SI 3
* %^BOLD%^adjust_volume%^RESET%^
.EI
.SI 5
int adjust_volume(int type, int amount)
.EI

.SI 3
* %^BOLD%^adjust_xp%^RESET%^
.EI
.SI 5
varargs int adjust_xp(int number, int shared)
.EI

.SI 3
* %^BOLD%^clear_gp_info%^RESET%^
.EI
.SI 5
void clear_gp_info()
.EI

.SI 3
* %^BOLD%^health_string%^RESET%^
.EI
.SI 5
varargs string health_string(int flag, int ref health_level)
.EI

.SI 3
* %^BOLD%^query_callingdeath%^RESET%^
.EI
.SI 5
int query_callingdeath()
.EI

.SI 3
* %^BOLD%^query_counts%^RESET%^
.EI
.SI 5
mapping query_counts()
.EI

.SI 3
* %^BOLD%^query_drink_info%^RESET%^
.EI
.SI 5
int * query_drink_info()
.EI

.SI 3
* %^BOLD%^query_gp%^RESET%^
.EI
.SI 5
int query_gp()
.EI

.SI 3
* %^BOLD%^query_hp%^RESET%^
.EI
.SI 5
int query_hp()
.EI

.SI 3
* %^BOLD%^query_max_gp%^RESET%^
.EI
.SI 5
int query_max_gp()
.EI

.SI 3
* %^BOLD%^query_max_hp%^RESET%^
.EI
.SI 5
int query_max_hp()
.EI

.SI 3
* %^BOLD%^query_specific_gp%^RESET%^
.EI
.SI 5
int query_specific_gp(string gp_type)
.EI

.SI 3
* %^BOLD%^query_surrender%^RESET%^
.EI
.SI 5
int query_surrender()
.EI

.SI 3
* %^BOLD%^query_volume%^RESET%^
.EI
.SI 5
int query_volume(int type)
.EI

.SI 3
* %^BOLD%^query_wimpy%^RESET%^
.EI
.SI 5
int query_wimpy()
.EI

.SI 3
* %^BOLD%^query_xp%^RESET%^
.EI
.SI 5
int query_xp()
.EI

.SI 3
* %^BOLD%^reset_callingdeath%^RESET%^
.EI
.SI 5
void reset_callingdeath()
.EI

.SI 3
* %^BOLD%^set_gp%^RESET%^
.EI
.SI 5
int set_gp(int number)
.EI

.SI 3
* %^BOLD%^set_hp%^RESET%^
.EI
.SI 5
varargs int set_hp(int number, object attacker)
.EI

.SI 3
* %^BOLD%^set_max_gp%^RESET%^
.EI
.SI 5
int set_max_gp(int number)
.EI

.SI 3
* %^BOLD%^set_max_hp%^RESET%^
.EI
.SI 5
int set_max_hp(int number)
.EI

.SI 3
* %^BOLD%^set_surrender%^RESET%^
.EI
.SI 5
int set_surrender(int number)
.EI

.SI 3
* %^BOLD%^set_wimpy%^RESET%^
.EI
.SI 5
int set_wimpy(int number)
.EI

.SI 3
* %^BOLD%^update_volumes%^RESET%^
.EI
.SI 5
void update_volumes()
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^check_surrender%^RESET%^
.EI
.SI 5
int check_surrender()
.EI

.SI 3
* %^BOLD%^check_wimpy%^RESET%^
.EI
.SI 5
int check_wimpy()
.EI


