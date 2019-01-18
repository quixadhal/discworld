.DT
intermud.c
Disk World autodoc help
intermud.c

.SH Inherits
.SP 5 5
This class inherits the following classes /net/inherit/client.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/network.h, /net/intermud3/intermud.h and /include/socket_errors.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^list%^RESET%^
class list {
int ID;
mapping List;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^GetMudName%^RESET%^
.EI
.SI 5
string GetMudName(string mud)
.EI

.SI 3
* %^BOLD%^SetDestructOnClose%^RESET%^
.EI
.SI 5
int SetDestructOnClose(int x)
.EI

.SI 3
* %^BOLD%^SetSocketType%^RESET%^
.EI
.SI 5
int SetSocketType(int x)
.EI

.SI 3
* %^BOLD%^eventExternWrite%^RESET%^
.EI
.SI 5
void eventExternWrite(mixed * packet)
.EI

.SI 3
* %^BOLD%^eventWrite%^RESET%^
.EI
.SI 5
void eventWrite(int fd, mixed * packet)
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^Setup%^RESET%^
.EI
.SI 5
void Setup()
.EI

.SI 3
* %^BOLD%^eventConnectionFailure%^RESET%^
.EI
.SI 5
void eventConnectionFailure()
.EI

.SI 3
* %^BOLD%^eventRead%^RESET%^
.EI
.SI 5
void eventRead(int fd, mixed * packet)
.EI

.SI 3
* %^BOLD%^eventSocketClose%^RESET%^
.EI
.SI 5
void eventSocketClose(int fd)
.EI


