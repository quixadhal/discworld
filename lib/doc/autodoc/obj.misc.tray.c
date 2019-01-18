.DT
tray.c
Disk World autodoc help
tray.c

.SH Description
.SP 5 5

A small tray, to give to vendors!
This tray weighs 250 grams and can hold up to 2 kilos worth of items!
Lets hear it for.. small wooden tray!
.EP
.SP 10 5


Written by Terano.

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/wearable, /std/basic/holdable and /std/surface.
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
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
varargs void init_dynamic_arg(mapping map, mixed new_arg)
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
string long(string word, int dark)
.EI

.SI 3
* %^BOLD%^move%^RESET%^
.EI
.SI 5
varargs int move(mixed dir, string messin, string messout)
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
mapping query_static_auto_load()
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed stats()
.EI


