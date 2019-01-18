.DT
auto_mailer.c
Disk World autodoc help
auto_mailer.c

.SH Description
.SP 5 5

Allows objects to send mail as if they were from the given player.

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h and /include/mail.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^auto_mail%^RESET%^
.EI
.SI 5
int auto_mail(string to, string from, string sub, string cc, string body, int send_inter, string only_to, int flag)
.EI
.SP 7 5

Allows people to send mail automaticly from the player in question.
Thus allows mail to be sent on someone being promoted to creator
and stuff.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
to - who the mail is to
.EP
.SP 9 5
from - who the mail is from
.EP
.SP 9 5
cc - the people to carbon copy
.EP
.SP 9 5
body - the body of the message
.EP
.SP 9 5
send_inter - should be send it ove the inter mud system?
.EP
.SP 9 5
only_to - ignore the cc fields
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the same value as do_mail_message does, 0 on failure

.EP


