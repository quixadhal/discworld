.DT
boring_stick.c
Disk World autodoc help
boring_stick.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/held.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_flick%^RESET%^
.EI
.SI 5
int do_flick()
.EI

.SI 3
* %^BOLD%^do_press%^RESET%^
.EI
.SI 5
int do_press()
.EI

.SI 3
* %^BOLD%^do_push%^RESET%^
.EI
.SI 5
int do_push()
.EI

.SI 3
* %^BOLD%^do_switch%^RESET%^
.EI
.SI 5
int do_switch(mixed * indirect_obs, string indir_match, string dir_match, mixed * args, string pattern)
.EI

.SI 3
* %^BOLD%^do_turn%^RESET%^
.EI
.SI 5
int do_turn(mixed * indirect_obs, string indir_match, string dir_match, mixed * args, string pattern)
.EI

.SI 3
* %^BOLD%^extra_look%^RESET%^
.EI
.SI 5
string extra_look()
.EI

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping map)
.EI

.SI 3
* %^BOLD%^move%^RESET%^
.EI
.SI 5
varargs int move(mixed dest, string str1, string str2)
.EI

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^set_state%^RESET%^
.EI
.SI 5
void set_state(int new_state)
.EI


