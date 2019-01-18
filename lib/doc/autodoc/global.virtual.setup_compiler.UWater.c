.DT
UWater.c
Disk World autodoc help
UWater.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/uwater.c.
.EP

.SH Includes
.SP 5 5
This class includes the following files /global/virtual/setup_compiler/Room.inc.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^AddFuncsToMixed%^RESET%^
.EI
.SI 5
int AddFuncsToMixed(mixed * array, object obj, mixed func)
.EI

.SI 3
* %^BOLD%^FindObject%^RESET%^
.EI
.SI 5
object FindObject(mixed obj)
.EI

.SI 3
* %^BOLD%^QueryResetObjects%^RESET%^
.EI
.SI 5
int QueryResetObjects()
.EI

.SI 3
* %^BOLD%^RunFuncsOnObjects%^RESET%^
.EI
.SI 5
int RunFuncsOnObjects(mixed * array, object arg)
.EI

.SI 3
* %^BOLD%^add_clone_on_reset%^RESET%^
.EI
.SI 5
void add_clone_on_reset(string fname)
.EI

.SI 3
* %^BOLD%^add_death_func%^RESET%^
.EI
.SI 5
void add_death_func(mixed ob, string func)
.EI

.SI 3
* %^BOLD%^add_dest_func%^RESET%^
.EI
.SI 5
void add_dest_func(mixed ob, string func)
.EI

.SI 3
* %^BOLD%^add_enter_func%^RESET%^
.EI
.SI 5
void add_enter_func(mixed ob, string func)
.EI

.SI 3
* %^BOLD%^add_exit_func%^RESET%^
.EI
.SI 5
void add_exit_func(mixed ob, string func)
.EI

.SI 3
* %^BOLD%^add_reset_func%^RESET%^
.EI
.SI 5
void add_reset_func(mixed ob, string func)
.EI

.SI 3
* %^BOLD%^event_death%^RESET%^
.EI
.SI 5
int event_death(object ob)
.EI

.SI 3
* %^BOLD%^event_enter%^RESET%^
.EI
.SI 5
int event_enter(object ob)
.EI

.SI 3
* %^BOLD%^event_exit%^RESET%^
.EI
.SI 5
void event_exit(object ob)
.EI


