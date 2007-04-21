.DT
lantern.c
DW_wibble autodoc help
lantern.c

.SH Description
.SP 5 5

This method returns the maximum amount of fuel for the object.

.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/fuel_handler.h.
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
int do_extinguish()
.EI

.SI 3
* %^BOLD%^do_light%^RESET%^
.EI
.SI 5
int do_light()
.EI

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping arg)
.EI

.SI 3
* %^BOLD%^out_of_fuel%^RESET%^
.EI
.SI 5
void out_of_fuel()
.EI

.SI 3
* %^BOLD%^pretty_plural%^RESET%^
.EI
.SI 5
string pretty_plural(object thing)
.EI

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^query_fuel_left%^RESET%^
.EI
.SI 5
int query_fuel_left()
.EI

.SI 3
* %^BOLD%^query_max_fuel%^RESET%^
.EI
.SI 5
int query_max_fuel()
.EI

.SI 3
* %^BOLD%^set_fuel_left%^RESET%^
.EI
.SI 5
void set_fuel_left(int i)
.EI

.SI 3
* %^BOLD%^short%^RESET%^
.EI
.SI 5
string short(int i)
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


