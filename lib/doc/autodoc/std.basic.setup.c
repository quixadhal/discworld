.DT
setup.c
Disk World autodoc help
setup.c

.SH Description
.SP 5 5

This handles the setup code for rooms and objects.  It simplifies the whole
system and makes it nice and generic.
.EP
.SP 10 5


Written by Pinkfish

Started May 16th

.EP

.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^dec_setup%^RESET%^
.EI
.SI 5
void dec_setup()
.EI
.SP 7 5

This method decrements the setup variable.

.EP

.SI 3
* %^BOLD%^do_reset%^RESET%^
.EI
.SI 5
void do_reset()
.EI
.SP 7 5

This method checks to see if the reset is possible.

.EP

.SI 3
* %^BOLD%^do_setup%^RESET%^
.EI
.SI 5
void do_setup()
.EI
.SP 7 5

This method checks to see if the setup is possible.

.EP

.SI 3
* %^BOLD%^inc_setup%^RESET%^
.EI
.SI 5
void inc_setup()
.EI
.SP 7 5

This method increments the setup variable.

.EP


