.DT
client.c
Disk World autodoc help
client.c

.SH Includes
.SP 5 5
This class includes the following files /include/client.h, /include/network.h and /include/socket_errors.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^SetDestructOnClose%^RESET%^
.EI
.SI 5
int SetDestructOnClose(int x)
.EI

.SI 3
* %^BOLD%^SetRead%^RESET%^
.EI
.SI 5
function SetRead(function f)
.EI

.SI 3
* %^BOLD%^SetSocketType%^RESET%^
.EI
.SI 5
int SetSocketType(int type)
.EI

.SI 3
* %^BOLD%^eventCreateSocket%^RESET%^
.EI
.SI 5
int eventCreateSocket(string host, int port)
.EI

.SI 3
* %^BOLD%^eventWrite%^RESET%^
.EI
.SI 5
void eventWrite(int fd, mixed val)
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^eventAbortCallback%^RESET%^
.EI
.SI 5
void eventAbortCallback(int fd)
.EI

.SI 3
* %^BOLD%^eventClose%^RESET%^
.EI
.SI 5
void eventClose(class client sock)
.EI

.SI 3
* %^BOLD%^eventRead%^RESET%^
.EI
.SI 5
void eventRead(int fd, mixed val)
.EI

.SI 3
* %^BOLD%^eventReadCallback%^RESET%^
.EI
.SI 5
void eventReadCallback(int fd, mixed val)
.EI

.SI 3
* %^BOLD%^eventSocketClose%^RESET%^
.EI
.SI 5
void eventSocketClose(int fd)
.EI

.SI 3
* %^BOLD%^eventSocketError%^RESET%^
.EI
.SI 5
void eventSocketError(string str, int x)
.EI

.SI 3
* %^BOLD%^eventWriteCallback%^RESET%^
.EI
.SI 5
void eventWriteCallback(int fd)
.EI


