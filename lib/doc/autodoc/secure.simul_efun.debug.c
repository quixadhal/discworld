.DT
debug.c
Disk World autodoc help
debug.c

.SH Description
.SP 5 5

Debugging simul efuns.

.EP
.SP 10 5


Written by Ceres

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^debug_printf%^RESET%^
.EI
.SI 5
void debug_printf(string fmt, mixed args ...)
.EI
.SP 7 5

This method will generate a debug inform for any creators in the room
where it is generated.  It will always attempt to identify the room
that the event should be sent to.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
all - printf style parameters
.EP

.SI 3
* %^BOLD%^tell_creator%^RESET%^
.EI
.SI 5
varargs void tell_creator(mixed cres, string fmt, mixed args ...)
.EI
.SP 7 5

This method writes a message to a creator, it's mainly meant 
for debug messages where sending a message to a specific 
creator is smarter than sending it to players, who will only get confused.

Typically you should use debug_printf() instead of this.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cre - the name or object to send the message to
.EP
.SP 9 5
fmt - the format string
.EP
.SP 9 5
args - the arguments to the sprintf

.EP


