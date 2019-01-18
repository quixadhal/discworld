.DT
unguarded.c
Disk World autodoc help
unguarded.c

.SH Description
.SP 5 5

Cotains some simul_efuns.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/origin.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^unguarded%^RESET%^
.EI
.SI 5
mixed unguarded(function f)
.EI
.SP 7 5

This does an unguarded apply.  An unguarded apply means it chops off
the call tree at this point for security purposes.  This is needed to be
done in places where access beyond what the caller may have is needed.
For example, the board handler uses this to save the messages away as
the players and creators do not have access to save into these directories
normally.



.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
f - the fucntion pointer to evaluate unguarded.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
returns the value of the evaluated function pointer

.EP


