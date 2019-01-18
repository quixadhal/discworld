.DT
telnet.c
Disk World autodoc help
telnet.c

.SH Includes
.SP 5 5
This class includes the following files /include/socket.h and /include/socket_errors.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^char%^RESET%^
.EI
.SI 5
int char(string str)
.EI

.SI 3
* %^BOLD%^connect%^RESET%^
.EI
.SI 5
int connect(string str)
.EI

.SI 3
* %^BOLD%^connected%^RESET%^
.EI
.SI 5
int connected()
.EI

.SI 3
* %^BOLD%^disconnect%^RESET%^
.EI
.SI 5
int disconnect(string str)
.EI

.SI 3
* %^BOLD%^disconnected%^RESET%^
.EI
.SI 5
void disconnected()
.EI

.SI 3
* %^BOLD%^line%^RESET%^
.EI
.SI 5
int line(string str)
.EI

.SI 3
* %^BOLD%^my_socket_write%^RESET%^
.EI
.SI 5
void my_socket_write(int fd, string message)
.EI

.SI 3
* %^BOLD%^query_connected%^RESET%^
.EI
.SI 5
int query_connected()
.EI

.SI 3
* %^BOLD%^receive_data%^RESET%^
.EI
.SI 5
void receive_data(int rec_fd, string msg)
.EI

.SI 3
* %^BOLD%^send%^RESET%^
.EI
.SI 5
int send(string str)
.EI

.SI 3
* %^BOLD%^set_callback%^RESET%^
.EI
.SI 5
void set_callback(string arg)
.EI

.SI 3
* %^BOLD%^set_verbosity%^RESET%^
.EI
.SI 5
void set_verbosity(int v)
.EI

.SI 3
* %^BOLD%^socket_shutdown%^RESET%^
.EI
.SI 5
void socket_shutdown(int fd)
.EI

.SI 3
* %^BOLD%^write_data%^RESET%^
.EI
.SI 5
void write_data(int fd)
.EI


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^init_tel_neg%^RESET%^
.EI
.SI 5
void init_tel_neg()
.EI


