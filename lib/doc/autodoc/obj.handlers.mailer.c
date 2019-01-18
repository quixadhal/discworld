.DT
mailer.c
Disk World autodoc help
mailer.c

.SH Description
.SP 5 5

The mailer handler object.  This is used to send mail and do oyther
stuff related to mail.
 The new mailer object ! *shiver*
 Thanks to Wodan and Pinkfish for ideas and help.
 By Turrican@Discworld, May 1995.
.EP
.SP 10 5


Written by Turrican

Started May 1995

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/localtime.h, /include/mail.h and /include/mime.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_mail_message%^RESET%^
.EI
.SI 5
varargs int do_mail_message(string t, string from, string sub, string ccs, string body,  int, string only_to, int flag)
.EI
.SP 7 5

This method allows a message to be mailed.  It checks the previous
object to make sure it is one of the allowed set to
do mailing.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
t - who it is to
.EP
.SP 9 5
from - who it is from
.EP
.SP 9 5
sub - the subject of the message
.EP
.SP 9 5
ccs - the carbon copy recipients
.EP
.SP 9 5
body - the body of the message
.EP
.SP 9 5
only_to - only mail to the to address
.EP
.SP 9 5
flag - prevent this_player() from getting the messages if flag != 0
.EP

.SI 3
* %^BOLD%^finger_mail%^RESET%^
.EI
.SI 5
string finger_mail(string pname)
.EI
.SP 7 5

This method returns the mail information which is placed into the
the finger command.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the name of the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the function mail string

.EP

.SI 3
* %^BOLD%^format_date%^RESET%^
.EI
.SI 5
string format_date(int x)
.EI
.SP 7 5

This formats the date as needed by the mailer object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the date to format
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new date

.EP

.SI 3
* %^BOLD%^new_mail%^RESET%^
.EI
.SI 5
string new_mail(string pname)
.EI
.SP 7 5

This method returns a string saying if the player has new mail or not.
This is what is used when the player first logs on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the name of the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new mail string
.EP

.SI 3
* %^BOLD%^query_do_this_last%^RESET%^
.EI
.SI 5
mixed * query_do_this_last()
.EI
.SP 7 5

This method returns the call back fuinction to use when
the mailer has finished. ({ ob, func })
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the call back function

.EP

.SI 3
* %^BOLD%^query_folders%^RESET%^
.EI
.SI 5
string * query_folders(string pname)
.EI
.SP 7 5

This method returns the list of folders associated with the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the player name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array containing the folder names

.EP

.SI 3
* %^BOLD%^read_mail%^RESET%^
.EI
.SI 5
void read_mail(string str, string sub)
.EI
.SP 7 5

This method is the main entry point to the mailer.  It is
what is called to start up the system when a mailer is used.
.EP

.SI 3
* %^BOLD%^set_do_this_last%^RESET%^
.EI
.SI 5
void set_do_this_last(mixed * bing)
.EI
.SP 7 5

This method sets the call back fuinction to use when
the mailer has finished. ({ ob, func })
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bing - the call back function

.EP


