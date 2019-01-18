.DT
wearable_corpse.c
Disk World autodoc help
wearable_corpse.c

.SH Description
.SP 5 5

Wearable corpse for wearable pets!

To use this, you need to put a make_corpse function in your pet that
clones this object and sets it up as per a standard corpse.

You should also call: set_type, set_max_cond and set_cond on it!
a
.EP
.SP 10 5


Written by Terano

Started 24/11/02

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/wearable and /obj/corpse.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping map)
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
mixed * stats()
.EI


