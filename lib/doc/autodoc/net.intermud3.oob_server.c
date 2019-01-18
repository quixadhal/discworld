.DT
oob_server.c
Disk World autodoc help
oob_server.c

.SH Inherits
.SP 5 5
This class inherits the following classes /net/inherit/server.
.EP

.SH Includes
.SP 5 5
This class includes the following files /net/intermud3/oob_server.h, /include/network.h and /include/socket_errors.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^connection%^RESET%^
class connection {
string MudName;
mixed Buffer;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^Setup%^RESET%^
.EI
.SI 5
void Setup()
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^eventRead%^RESET%^
.EI
.SI 5
void eventRead(int fd, mixed * packet)
.EI

.SI 3
* %^BOLD%^eventSocketClosed%^RESET%^
.EI
.SI 5
void eventSocketClosed(int fd)
.EI

.SI 3
* %^BOLD%^eventTimeout%^RESET%^
.EI
.SI 5
void eventTimeout(int fd)
.EI


