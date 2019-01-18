.DT
back_trace.c
Disk World autodoc help
back_trace.c

.SH Description
.SP 5 5

Cotains some simul_efuns.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^back_trace%^RESET%^
.EI
.SI 5
string back_trace()
.EI
.SP 7 5

This method returns out the call stack traces.  It is usualy
called in the error handling routines in the master object
but it can be used elsewhere for debuggin purposes.  You
can see the results of this function in /log/runtime and
/log/catch.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
call stack trace

.EP


