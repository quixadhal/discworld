.DT
present.c
Disk World autodoc help
present.c

.SH Description
.SP 5 5

This method sets the colour of the present.
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/baggage.
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
* %^BOLD%^do_unwrap%^RESET%^
.EI
.SI 5
int do_unwrap()
.EI
.SP 7 5

This method unwraps the wrapping paper and sets up all the exiting
bits which pop out of it.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_wrap%^RESET%^
.EI
.SI 5
int do_wrap(object * things)
.EI
.SP 7 5

This is the main entry point and wraps up the given objects into the
wrapping paper.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
things - the things to wrap up
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^long_status%^RESET%^
.EI
.SI 5
string long_status()
.EI

.SI 3
* %^BOLD%^query_closed%^RESET%^
.EI
.SI 5
int query_closed()
.EI

.SI 3
* %^BOLD%^set_colour%^RESET%^
.EI
.SI 5
void set_colour(string word)
.EI

.SI 3
* %^BOLD%^set_type%^RESET%^
.EI
.SI 5
void set_type(string word)
.EI
.SP 7 5

This method sets the type of the present.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the type of the present

.EP


