.DT
baggage.c
Disk World autodoc help
baggage.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/close_lock_container, /std/basic/holdable, /std/basic/condition and /std/container.
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
* %^BOLD%^check_breakages%^RESET%^
.EI
.SI 5
void check_breakages()
.EI
.SP 7 5

This method checks to see if any of the things contained in the
container should be broken.
.EP

.SI 3
* %^BOLD%^ensure_open%^RESET%^
.EI
.SI 5
int ensure_open()
.EI
.SP 7 5

This method returns true if the object is open and prints a message
about the open status of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is open, 0 if not

.EP


