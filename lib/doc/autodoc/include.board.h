.DT
board.h
Disk World autodoc help
board.h

.SH Description
.SP 5 5

Defines and support stuff for the board handler.  This is one of the very
early objects/files.  One which has not changed overly much either.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Classes

.SI 3
* %^BOLD%^reply_type%^RESET%^
class reply_type {
int type;
mixed data;
}

.EI


.SH Defines
.SI 3
* %^BOLD%^B_ACCESS_APPROVE%^RESET%^
.EI
.SP 7 5

This is the parameter passed in the access method if the check is for
approving a post.

.EP

.SI 3
* %^BOLD%^B_ACCESS_DELETE%^RESET%^
.EI
.SP 7 5

This is the parameter passed in the access method if the check is for
deleting.

.EP

.SI 3
* %^BOLD%^B_ACCESS_INFORM%^RESET%^
.EI
.SP 7 5

This is the parameter passed in the access method when an inform
is required.

.EP

.SI 3
* %^BOLD%^B_ACCESS_READ%^RESET%^
.EI
.SP 7 5

This is the parameter passed in the access method if the check is for
reading.

.EP

.SI 3
* %^BOLD%^B_ACCESS_WRITE%^RESET%^
.EI
.SP 7 5

This is the parameter passed in the access method if the check is for
writing.

.EP

.SI 3
* %^BOLD%^B_NAME%^RESET%^
.EI
.SP 7 5

The index into the subject array that is the name of the person who
posted the note.

.EP

.SI 3
* %^BOLD%^B_NUM%^RESET%^
.EI
.SP 7 5

The index into the subject array that is the note number associated with
the note.

.EP

.SI 3
* %^BOLD%^B_PRIV_ACCESS_RESTRICTED%^RESET%^
.EI
.SP 7 5

Restrict access to a specified group of people.

.EP

.SI 3
* %^BOLD%^B_PRIV_ACCESS_RESTRICTED_FILE%^RESET%^
.EI
.SP 7 5

Restrict access to a specified group of files.

.EP

.SI 3
* %^BOLD%^B_PRIV_ACCESS_RESTRICTED_METHOD%^RESET%^
.EI
.SP 7 5

Do all the access checks and everything through another function check in
another file.

.EP

.SI 3
* %^BOLD%^B_PRIV_NO_INFORM%^RESET%^
.EI
.SP 7 5

Don't inform people when messages are posted.

.EP

.SI 3
* %^BOLD%^B_PRIV_NORMAL%^RESET%^
.EI
.SP 7 5

The default privilages for a board.

.EP

.SI 3
* %^BOLD%^B_PRIV_READ_ONLY%^RESET%^
.EI
.SP 7 5

Make the board read only.

.EP

.SI 3
* %^BOLD%^B_PRIV_TYPE_MASK%^RESET%^
.EI
.SP 7 5

The type mask.

.EP

.SI 3
* %^BOLD%^B_REPLY_NAMED%^RESET%^
.EI
.SP 7 5

This is the type to use if the reply should be mailed to the specified
person in the data field.

.EP

.SI 3
* %^BOLD%^B_REPLY_NUM%^RESET%^
.EI
.SP 7 5

The index into the subject array that points back to the message we
followed up to.

.EP

.SI 3
* %^BOLD%^B_REPLY_POSTER%^RESET%^
.EI
.SP 7 5

This is the type to use if the reply should be mailed to the
poster.

.EP

.SI 3
* %^BOLD%^B_REPLY_TYPE%^RESET%^
.EI
.SP 7 5

The index into the subject array that determines who the followups and
replies to the note should go to.  If this value is 0 then the
followups go to the board and the replies go to the person who
posted it.  If it is a string then the followups and replies go to the
person in the string.

.EP

.SI 3
* %^BOLD%^B_SUBJECT%^RESET%^
.EI
.SP 7 5

The index into the subject array that is the subject.
.EP

.SI 3
* %^BOLD%^B_TIME%^RESET%^
.EI
.SP 7 5

The index into the subject array that contains the time the note was
posted.

.EP

.SI 3
* %^BOLD%^BOARD_DIR%^RESET%^
.EI
.SP 7 5

This is the directory into which the individual board messages are
saved.

.EP

.SI 3
* %^BOLD%^BOARD_FILE%^RESET%^
.EI
.SP 7 5

This is the main save file for the board handler.  It has the current
boards, subjects, permissions etc saved to it.

.EP

.SI 3
* %^BOLD%^BOARD_HAND%^RESET%^
.EI
.SP 7 5

The path to the board handler object itself.

.EP

.SI 3
* %^BOLD%^BOARD_ORDER%^RESET%^
.EI
.SP 7 5

This is the property on the player that stores the board order.  The
board order is the order in which the boards are read from the web and
by using the 'new' word inside the mud itself.

.EP

.SI 3
* %^BOLD%^BOARD_RAM_DIR%^RESET%^
.EI
.SP 7 5

This is the directory into which the individual board messages are
saved temporarily.

.EP

.SI 3
* %^BOLD%^BOARD_SOCIAL_POINT_COST%^RESET%^
.EI
.SP 7 5

This amount of social points used to post to the boards.

.EP

.SI 3
* %^BOLD%^BOARD_WEB%^RESET%^
.EI
.SP 7 5

The path to the web board reader.

.EP

.SI 3
* %^BOLD%^MASTER_OB%^RESET%^
.EI
.SP 7 5

The master object.

.EP

.SI 3
* %^BOLD%^NEWS_RC%^RESET%^
.EI
.SP 7 5

The name of the property on the player that has the news rc.  The news RC
is the last time each board was read.

.EP

