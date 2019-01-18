.DT
smtp.c
Disk World autodoc help
smtp.c

.SH Inherits
.SP 5 5
This class inherits the following classes /net/inherit/client.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/mail.h, /include/network.h, /net/obj/smtp.h, /include/socket_errors.h and /include/mime.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^eventSendMail%^RESET%^
.EI
.SI 5
void eventSendMail(mixed to, string from, string mess)
.EI

.SI 3
* %^BOLD%^eventWrite%^RESET%^
.EI
.SI 5
void eventWrite(string mess)
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^eventForward%^RESET%^
.EI
.SI 5
void eventForward(string who)
.EI

.SI 3
* %^BOLD%^eventOKToSendMail%^RESET%^
.EI
.SI 5
void eventOKToSendMail()
.EI

.SI 3
* %^BOLD%^eventRead%^RESET%^
.EI
.SI 5
void eventRead(int fd, string mess)
.EI

.SI 3
* %^BOLD%^eventSuccess%^RESET%^
.EI
.SI 5
void eventSuccess()
.EI


