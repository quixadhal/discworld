.DT
grinder.c
Disk World autodoc help
grinder.c

.SH Description
.SP 5 5

To set a maximum amount of an object that can be ground at once.
If left with the default value of 0, there is no maximum amount.

.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/container.
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
* %^BOLD%^do_empty%^RESET%^
.EI
.SI 5
int do_empty(object * dest, string me, string him, string prep)
.EI

.SI 3
* %^BOLD%^do_grind%^RESET%^
.EI
.SI 5
int do_grind(string what)
.EI

.SI 3
* %^BOLD%^long_status%^RESET%^
.EI
.SI 5
string long_status()
.EI

.SI 3
* %^BOLD%^query_max_grindable%^RESET%^
.EI
.SI 5
int query_max_grindable()
.EI
.SP 7 5

Returns the maximum amount grindable at once

.EP

.SI 3
* %^BOLD%^set_max_grindable%^RESET%^
.EI
.SI 5
void set_max_grindable(int i)
.EI

.SI 3
* %^BOLD%^test_add%^RESET%^
.EI
.SI 5
int test_add(object ob, int flag)
.EI


