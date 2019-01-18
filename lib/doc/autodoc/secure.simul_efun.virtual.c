.DT
virtual.c
Disk World autodoc help
virtual.c

.SH Description
.SP 5 5

Cotains some simul_efuns.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/virtual.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^clone_object%^RESET%^
.EI
.SI 5
object clone_object(string str, mixed * args ...)
.EI
.SP 7 5

This method creates a new object.  This is overridden to handle the
virtual object stuff.
See the documentation on the efun clone_object for more information.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the object to clone
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the newly cloned object

.EP


