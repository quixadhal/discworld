.DT
find_match.c
Disk World autodoc help
find_match.c

.SH Description
.SP 5 5

The find_match simul_efun.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /secure/simul_efun/obj_parser.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/obj_parser.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^filter_in_me_or_environment%^RESET%^
.EI
.SI 5
object * filter_in_me_or_environment(object * obs, object player)
.EI
.SP 7 5

This method returns the array of objects that are only inside the
player in question, or in their environment.  It excludes any items
in other peoples inventorys.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects to filer
.EP
.SP 9 5
player - the player to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the filtered objects

.EP

.SI 3
* %^BOLD%^is_in_me_or_environment%^RESET%^
.EI
.SI 5
int is_in_me_or_environment(object thing, object person)
.EI
.SP 7 5

This method checks to see if the specified object is in the player
or the environment.  If it has no environment it is considered to
be here.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to test
.EP
.SP 9 5
player - the player to check against
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is, 0 if not

.EP


