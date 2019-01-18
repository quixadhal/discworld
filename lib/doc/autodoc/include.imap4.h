.DT
imap4.h
Disk World autodoc help
imap4.h

.SH Classes

.SI 3
* %^BOLD%^imap_session%^RESET%^
class imap_session {
int fd;
int state;
int time;
string user_name;
string selected;
string command;
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
* %^BOLD%^IMAP4_VERSION%^RESET%^
.EI

.SI 3
* %^BOLD%^IMAP_AUTHENTICATED%^RESET%^
.EI

.SI 3
* %^BOLD%^IMAP_CONNECTED%^RESET%^
.EI

.SI 3
* %^BOLD%^IMAP_SELECTED%^RESET%^
.EI

.SI 3
* %^BOLD%^LOGIN_OB%^RESET%^
.EI

.SI 3
* %^BOLD%^PORT_IMAP4%^RESET%^
.EI

.SI 3
* %^BOLD%^TP(_x)%^RESET%^
.EI

