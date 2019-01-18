.DT
news_handler.c
Disk World autodoc help
news_handler.c

.SH Description
.SP 5 5

The News Handler
.EP
.SP 10 5


Written by Taffyd

Started 19/09/2000

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/board.h and /include/login_handler.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^broadcast_announcement%^RESET%^
.EI
.SI 5
void broadcast_announcement()
.EI

.SI 3
* %^BOLD%^has_unread_announcements%^RESET%^
.EI
.SI 5
int has_unread_announcements(string player)
.EI

.SI 3
* %^BOLD%^login_event%^RESET%^
.EI
.SI 5
void login_event(mixed person, string type)
.EI

.SI 3
* %^BOLD%^show_message%^RESET%^
.EI
.SI 5
void show_message(mixed person)
.EI


