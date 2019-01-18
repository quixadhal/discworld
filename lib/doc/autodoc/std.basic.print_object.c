.DT
print_object.c
Disk World autodoc help
print_object.c

.SH Description
.SP 5 5

This class prints out an object in a useful format.
Mostly used for debugging.  The resuls of this can mostly be
acheived using sprintf("%O", ob). 
.EP
.SP 10 5

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^print_object%^RESET%^
.EI
.SI 5
varargs string print_object(mixed ob, int off)
.EI
.SP 7 5

This method returns the value of the variable in 
useful format.  This can mostly be emulated with the
use of sprintf("%O", ob).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the variable to print out info for
.EP
.SP 9 5
off - the current depth offset
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a nice string

.EP


