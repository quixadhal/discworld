.DT
out_auth.c
Disk World autodoc help
out_auth.c

.SH Inherits
.SP 5 5
This class inherits the following classes /net/inherit/client.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/network.h and /include/socket_errors.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^auth%^RESET%^
class auth {
object Object;
string Function;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_auth%^RESET%^
.EI
.SI 5
void query_auth(string func)
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^eventRead%^RESET%^
.EI
.SI 5
void eventRead(int fd, string mess)
.EI

.SI 3
* %^BOLD%^eventSocketClose%^RESET%^
.EI
.SI 5
void eventSocketClose(int fd)
.EI


