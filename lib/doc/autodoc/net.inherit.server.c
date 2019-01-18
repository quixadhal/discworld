.DT
server.c
Disk World autodoc help
server.c

.SH Includes
.SP 5 5
This class includes the following files /include/server.h, /include/network.h and /include/socket_errors.h.
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
int eventCreateSocket(int port)
.EI

.SI 3
* %^BOLD%^eventServerAbortCallback%^RESET%^
.EI
.SI 5
void eventServerAbortCallback(int fd)
.EI

.SI 3
* %^BOLD%^eventWrite%^RESET%^
.EI
.SI 5
varargs void eventWrite(int fd, mixed val, int close, int type, int pos, int size)
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^eventClose%^RESET%^
.EI
.SI 5
void eventClose(class server sock)
.EI

.SI 3
* %^BOLD%^eventNewConnection%^RESET%^
.EI
.SI 5
void eventNewConnection(int fd)
.EI

.SI 3
* %^BOLD%^eventRead%^RESET%^
.EI
.SI 5
void eventRead(int fd, mixed val)
.EI

.SI 3
* %^BOLD%^eventServerListenCallback%^RESET%^
.EI
.SI 5
void eventServerListenCallback(int fd)
.EI

.SI 3
* %^BOLD%^eventServerReadCallback%^RESET%^
.EI
.SI 5
void eventServerReadCallback(int fd, mixed val)
.EI

.SI 3
* %^BOLD%^eventServerWriteCallback%^RESET%^
.EI
.SI 5
void eventServerWriteCallback(int fd)
.EI

.SI 3
* %^BOLD%^eventSocketClosed%^RESET%^
.EI
.SI 5
void eventSocketClosed(int fd)
.EI

.SI 3
* %^BOLD%^eventSocketError%^RESET%^
.EI
.SI 5
void eventSocketError(string str, int x)
.EI


