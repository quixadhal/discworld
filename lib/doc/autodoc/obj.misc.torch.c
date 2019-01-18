.DT
torch.c
Disk World autodoc help
torch.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/holdable and /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h and /include/fuel_handler.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^consume_fuel%^RESET%^
.EI
.SI 5
void consume_fuel()
.EI

.SI 3
* %^BOLD%^do_dowse%^RESET%^
.EI
.SI 5
int do_dowse()
.EI

.SI 3
* %^BOLD%^do_extinguish%^RESET%^
.EI
.SI 5
int do_extinguish(object *, string,  string,  string)
.EI

.SI 3
* %^BOLD%^do_light%^RESET%^
.EI
.SI 5
int do_light()
.EI

.SI 3
* %^BOLD%^do_warning%^RESET%^
.EI
.SI 5
void do_warning()
.EI

.SI 3
* %^BOLD%^finish_it%^RESET%^
.EI
.SI 5
int finish_it()
.EI

.SI 3
* %^BOLD%^held_this_item%^RESET%^
.EI
.SI 5
int held_this_item(int type, object holder, mixed arg)
.EI

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping arg)
.EI

.SI 3
* %^BOLD%^init_static_arg%^RESET%^
.EI
.SI 5
void init_static_arg(mapping arg)
.EI

.SI 3
* %^BOLD%^move%^RESET%^
.EI
.SI 5
varargs int move(mixed ob, string mess1, string mess2)
.EI

.SI 3
* %^BOLD%^out_of_fuel%^RESET%^
.EI
.SI 5
void out_of_fuel()
.EI

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mixed query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^query_fuel%^RESET%^
.EI
.SI 5
int query_fuel()
.EI

.SI 3
* %^BOLD%^query_lit%^RESET%^
.EI
.SI 5
int query_lit()
.EI

.SI 3
* %^BOLD%^query_static_auto_load%^RESET%^
.EI
.SI 5
mapping query_static_auto_load()
.EI

.SI 3
* %^BOLD%^query_torch%^RESET%^
.EI
.SI 5
int query_torch()
.EI

.SI 3
* %^BOLD%^self_light%^RESET%^
.EI
.SI 5
void self_light()
.EI

.SI 3
* %^BOLD%^set_fuel%^RESET%^
.EI
.SI 5
void set_fuel(int f)
.EI

.SI 3
* %^BOLD%^setup_shorts%^RESET%^
.EI
.SI 5
void setup_shorts()
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


