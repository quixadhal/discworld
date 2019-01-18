.DT
tell_creator.c
Disk World autodoc help
tell_creator.c

.SH Description
.SP 5 5

This method writes a message to a creator, it's mainly meant 
for debug messages where sending a message to a specific 
creator is smarter than sending it to players, who will only get confused.

.EP
.SP 10 5

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^tell_creator%^RESET%^
.EI
.SI 5
varargs void tell_creator(mixed cre, string fmt, mixed args ...)
.EI


