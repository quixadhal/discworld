.DT
banishment.c
Disk World autodoc help
banishment.c

.SH Description
.SP 5 5

This is the standard punishment file for banishing someone from a
city.
.EP
.SP 10 5


Written by Pinkfish

Started Tue Sep 19 14:37:28 PDT 2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/inherit/punishment_inherit.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/nomic_system.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_banish_handler%^RESET%^
.EI
.SI 5
string query_banish_handler()
.EI
.SP 7 5

This method returns the banish handler.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the banish handler

.EP

.SI 3
* %^BOLD%^set_banish_handler%^RESET%^
.EI
.SI 5
void set_banish_handler(string hand)
.EI
.SP 7 5

This method sets the banish handler.  The banish handler will be called
with two methods, one is:
.EP
.SP 7 5
banish_person(string name, int days)
.EP
.SP 7 5
banish_person_suspend(string name)
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
handler - the banish handler

.EP


