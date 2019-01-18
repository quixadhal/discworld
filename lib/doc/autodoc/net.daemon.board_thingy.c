.DT
board_thingy.c
Disk World autodoc help
board_thingy.c

.SH Inherits
.SP 5 5
This class inherits the following classes /net/inherit/server.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/localtime.h, /include/board.h, /include/network.h and /include/socket_errors.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^eventRead%^RESET%^
.EI
.SI 5
void eventRead(int fd, string str)
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^eventNewConnection%^RESET%^
.EI
.SI 5
void eventNewConnection(int fd)
.EI

.SI 3
* %^BOLD%^eventSocketClosed%^RESET%^
.EI
.SI 5
void eventSocketClosed(int fd)
.EI


