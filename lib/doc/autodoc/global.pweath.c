.DT
pweath.c
Disk World autodoc help
pweath.c

.SH Description
.SP 5 5

This method calculates an objects personal temperature index.

All temperatures in here are 20C lower so that comfort is at zero to
make calculations easier.

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/playtesters.h, /include/clothing.h and /include/weather.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_wetness%^RESET%^
.EI
.SI 5
void add_wetness(int arg)
.EI

.SI 3
* %^BOLD%^adjust_personal_temp%^RESET%^
.EI
.SI 5
void adjust_personal_temp(int i)
.EI

.SI 3
* %^BOLD%^calc_personal_temp%^RESET%^
.EI
.SI 5
int calc_personal_temp()
.EI

.SI 3
* %^BOLD%^dry_out%^RESET%^
.EI
.SI 5
void dry_out()
.EI

.SI 3
* %^BOLD%^query_personal_temp%^RESET%^
.EI
.SI 5
int query_personal_temp()
.EI

.SI 3
* %^BOLD%^query_temp_str%^RESET%^
.EI
.SI 5
string query_temp_str()
.EI

.SI 3
* %^BOLD%^query_wetness%^RESET%^
.EI
.SI 5
int query_wetness()
.EI

.SI 3
* %^BOLD%^set_personal_temp%^RESET%^
.EI
.SI 5
void set_personal_temp(int i)
.EI

.SI 3
* %^BOLD%^weather_extra_look%^RESET%^
.EI
.SI 5
string weather_extra_look()
.EI


