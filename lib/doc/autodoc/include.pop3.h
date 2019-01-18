.DT
pop3.h
Disk World autodoc help
pop3.h

.SH Classes

.SI 3
* %^BOLD%^pop_session%^RESET%^
class pop_session {
int fd;
int state;
int time;
string command;
string user_name;
int * deleted;
int num_messages;
class mail_header * headers;
string * messages;
int * sizes;
int last;
}

.EI


.SH Defines
.SI 3
* %^BOLD%^CHECK_CMD(_num,%^RESET%^
.EI

.SI 3
* %^BOLD%^CHECK_STATE(_state)%^RESET%^
.EI

.SI 3
* %^BOLD%^FOLDER_H%^RESET%^
.EI

.SI 3
* %^BOLD%^LOGIN_OB%^RESET%^
.EI

.SI 3
* %^BOLD%^POP3_VERSION%^RESET%^
.EI

.SI 3
* %^BOLD%^POP_AUTH_PASS%^RESET%^
.EI

.SI 3
* %^BOLD%^POP_AUTH_USER%^RESET%^
.EI

.SI 3
* %^BOLD%^POP_TRANSACTION%^RESET%^
.EI

.SI 3
* %^BOLD%^POP_UPDATE%^RESET%^
.EI

.SI 3
* %^BOLD%^PORT_POP3%^RESET%^
.EI

.SI 3
* %^BOLD%^TP(_x)%^RESET%^
.EI

