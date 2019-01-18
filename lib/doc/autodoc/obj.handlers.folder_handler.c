.DT
folder_handler.c
Disk World autodoc help
folder_handler.c

.SH Description
.SP 5 5

 Folder handler for the mailer.
 Actually it does a lot more :-)


Thanks to Wodan and Pinkfish for ideas and help. 
.EP
.SP 5 5
By Turrican@Discworld, May 1995.
.EP
.SP 10 5


Written by Turrican

Started May 1995

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/board.h, /include/mail.h, /include/socket_errors.h, /include/mime.h, /include/comms.h, /include/player_handler.h and /include/network.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_it%^RESET%^
.EI
.SI 5
void add_it(class mail_message mesg, int flag)
.EI
.SP 7 5

This method adds a message into all the correct folders and stuff.
This is the method that is called when a piece of mail is actually
sent.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mesg - the message to send
.EP
.SP 9 5
flag - if this is set to a non-zero value then no delivery messages will
occur
.EP

.SI 3
* %^BOLD%^can_create_folder%^RESET%^
.EI
.SI 5
nomask int can_create_folder(string player, string folder)
.EI
.SP 7 5

Can a folder with a certain name be created?
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player creating the folder
.EP
.SP 9 5
folder - the name of the folder to be created
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the folder can be created, 0 if it can't.

.EP

.SI 3
* %^BOLD%^check_empty%^RESET%^
.EI
.SI 5
int check_empty(string pname, string fname)
.EI
.SP 7 5

This method checks to see if the specified players folder is empty.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the player name to check
.EP
.SP 9 5
fname - the folder name to check

.EP

.SI 3
* %^BOLD%^check_local%^RESET%^
.EI
.SI 5
string check_local(string str)
.EI
.SP 7 5

This method checks to see if the address is local.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the address to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
local name for local messages, 0 for remote messages

.EP

.SI 3
* %^BOLD%^check_mail%^RESET%^
.EI
.SI 5
string check_mail(string pname)
.EI
.SP 7 5

This method is called when the player logs on to determine if they have
any new mail or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the players name to check

.EP

.SI 3
* %^BOLD%^delete_account%^RESET%^
.EI
.SI 5
void delete_account(string pname)
.EI
.SP 7 5

This method totaly removes the account for a particular player.  This
will be called when a player is deleted for whatever reason.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the player name to delete

.EP

.SI 3
* %^BOLD%^delete_it%^RESET%^
.EI
.SI 5
void delete_it(string pname, string fname, int * deleted)
.EI
.SP 7 5

This method will attempt to the delete the specified messages from
the folder.  The array deleted must be an array of integers where each
number is a message number to be deleted.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the players name
.EP
.SP 9 5
fname - the folder name
.EP
.SP 9 5
deleted - the array of message numbers to delete from the folder

.EP

.SI 3
* %^BOLD%^finger_mail%^RESET%^
.EI
.SI 5
string finger_mail(string pname)
.EI
.SP 7 5

This method returns the nice string used when fingering a player to
determine their mail status.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the player name to finger
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string associated with the inbox of the player

.EP

.SI 3
* %^BOLD%^get_messages%^RESET%^
.EI
.SI 5
class mail_header * get_messages(string pname, string fname)
.EI
.SP 7 5

This method returns the messages for the player in the specified
folder.  The return is an array of messages of type 'class mail_header'.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the players name
.EP
.SP 9 5
fname - the folder name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a list of messages

.EP

.SI 3
* %^BOLD%^load_message%^RESET%^
.EI
.SI 5
string load_message(string pname, string fname, int number)
.EI
.SP 7 5

This method loads a message for the specified player in the specified
folder.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the players name
.EP
.SP 9 5
fname - the folder name
.EP
.SP 9 5
number - the message number to open
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the text of the message

.EP

.SI 3
* %^BOLD%^mail_count%^RESET%^
.EI
.SI 5
int * mail_count(string pname)
.EI
.SP 7 5

This method returns a 2 item array for the amount of mail in a persons
inbox.  This first index is the total number of messages and the second
is the number that are unread.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the players name.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a two item array.

.EP

.SI 3
* %^BOLD%^mark_read%^RESET%^
.EI
.SI 5
void mark_read(string pname, string fname, int * unread)
.EI
.SP 7 5

This method marks the messages which are not in the unread array
in the specified folder as being read.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the player name
.EP
.SP 9 5
fname - the folder name
.EP
.SP 9 5
unread - the list of messages that are still unread

.EP

.SI 3
* %^BOLD%^move_it%^RESET%^
.EI
.SI 5
int move_it(string pname, string from, string to, int number)
.EI
.SP 7 5

This method moves the specified message from one folder to another.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the player name
.EP
.SP 9 5
from - the folder to move from
.EP
.SP 9 5
to - the folder to move to
.EP
.SP 9 5
number - the message number to move

.EP

.SI 3
* %^BOLD%^set_counter%^RESET%^
.EI
.SI 5
void set_counter(int x)
.EI
.SP 7 5

This method sets the current message counter.  This will return a
message if the counter was not valid.  This should be called in the
event that a recovery is needed if the counter screws up.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the new value for the counter

.EP


