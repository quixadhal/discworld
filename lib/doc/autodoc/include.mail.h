.DT
mail.h
Disk World autodoc help
mail.h

.SH Description
.SP 5 5

This is the include file for the mailer.
.EP
.SP 10 5


Written by Pinkfish

Started 1992 sometime

.EP

.SH Classes

.SI 3
* %^BOLD%^mail_header%^RESET%^
class mail_header {
int number;
string status;
string subject;
string from;
}

.EI
.SP 7 5

The mail header class.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
number - the message number
.EP
.SP 9 5
status - the current status of the message
.EP
.SP 9 5
subject - the subject of the message
.EP
.SP 9 5
from - who the message is from

.EP

.SI 3
* %^BOLD%^mail_message%^RESET%^
class mail_message {
string * to;
string * cc;
string body;
string from;
string subject;
}

.EI
.SP 7 5

The mail message class.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
to - the list of people the message is to
.EP
.SP 9 5
cc - the list of people the message is cced to
.EP
.SP 9 5
body - the actual message text
.EP
.SP 9 5
subject - the subject of the message

.EP


.SH Defines
.SI 3
* %^BOLD%^AUTO_MAILER%^RESET%^
.EI
.SP 7 5

The path to the auto mailer object.

.EP

.SI 3
* %^BOLD%^DAYS%^RESET%^
.EI
.SP 7 5

A define of the days as strings, starting from Sun as 0 to Sat as 6.

.EP

.SI 3
* %^BOLD%^EXTERNAL_MAIL_PATH%^RESET%^
.EI
.SP 7 5

The path to the location to look for external incoming mail.

.EP

.SI 3
* %^BOLD%^FROG_HOME%^RESET%^
.EI
.SP 7 5

The path to the place where the frogs try to go when they have
finished delivering their mail.

.EP

.SI 3
* %^BOLD%^MAIL_PARCEL_HANDLER%^RESET%^
.EI
.SP 7 5

This is the path to the parcel post handler.

.EP

.SI 3
* %^BOLD%^MAIL_TRACK%^RESET%^
.EI
.SP 7 5

The path to the mail tracker handler.  Keeps track of who has which
mailer.

.EP

.SI 3
* %^BOLD%^MAILER%^RESET%^
.EI
.SP 7 5

The path to the actual mailer object.

.EP

.SI 3
* %^BOLD%^MAX_FOLDER_SIZE%^RESET%^
.EI
.SP 7 5

This is the maximum number of messages storable in a single folder.

.EP

.SI 3
* %^BOLD%^MONTHS%^RESET%^
.EI
.SP 7 5

A define of an array of the months as strings, starting from Jan as 0 and
Dec as 11.

.EP

.SI 3
* %^BOLD%^OUT_MAIL%^RESET%^
.EI
.SP 7 5

The path to the smtp mail outgoing handler.

.EP

.SI 3
* %^BOLD%^POST_FROG%^RESET%^
.EI
.SP 7 5

The path to the post office frog NPC.

.EP

.SI 3
* %^BOLD%^SMTP%^RESET%^
.EI
.SP 7 5

The path to the SMTP object.

.EP

.SI 3
* %^BOLD%^WARN_FOLDER_SIZE%^RESET%^
.EI

