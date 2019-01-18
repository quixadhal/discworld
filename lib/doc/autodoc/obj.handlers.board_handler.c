.DT
board_handler.c
Disk World autodoc help
board_handler.c

.SH Description
.SP 5 5

Keeps track of all the bulletin board notes and related information.

.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/am_time.h, /include/board.h, /include/mail.h and /include/clubs.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^newsrc%^RESET%^
class newsrc {
int cached;
int changed;
mapping newsrc;
string * kill;
string * board_order;
}

.EI

.SI 3
* %^BOLD%^read_access_cache%^RESET%^
class read_access_cache {
int read_time;
int read_access;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_allowed%^RESET%^
.EI
.SI 5
int add_allowed(string board, string name)
.EI
.SP 7 5

Adds a member into the security array for a board.  This allows
certain people to read boards they may not normaly have
access to.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to change the access on
.EP
.SP 9 5
name - the name of the person to add to the array
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, 1 on success

.EP

.SI 3
* %^BOLD%^add_idiot%^RESET%^
.EI
.SI 5
void add_idiot(string idiot, int length)
.EI
.SP 7 5

This method adds an idiot to the current list of idiots.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
idiot - the idiot to add
.EP
.SP 9 5
length - the amount of days to add them for

.EP

.SI 3
* %^BOLD%^add_message%^RESET%^
.EI
.SI 5
int add_message(string board, string cap_name, string subject, string body, int reply_to, class reply_type bing)
.EI
.SP 7 5

Adds a new message onto the board.  This call can only be done from
verified source, like the bulletin oard objects themselves.  The
number used as a reply to should be the message number itself, not
the logical index.  If the reply_to is 0 then it is not
replying to anything at all.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to add the message to
.EP
.SP 9 5
cap_name - the name ofthe person posting
.EP
.SP 9 5
subject - the subject of the message
.EP
.SP 9 5
body - the main section of the text
.EP
.SP 9 5
reply_to - the note the message is replying to
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the note number, or 0 on failure

.EP

.SI 3
* %^BOLD%^can_delete_message%^RESET%^
.EI
.SI 5
int can_delete_message(string board, int off, string pname)
.EI
.SP 7 5

Check to see if the named person can delete the message.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pname - the player name
.EP
.SP 9 5
board - the board name
.EP
.SP 9 5
off - the offset to delete
.EP

.SI 3
* %^BOLD%^clean_cache%^RESET%^
.EI
.SI 5
void clean_cache()
.EI

.SI 3
* %^BOLD%^create_board%^RESET%^
.EI
.SI 5
int create_board(string board, int priviliges, string person)
.EI
.SP 7 5

Create a new board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the name of the board to create
.EP
.SP 9 5
priva - is this board only allowed prviliged access?
.EP
.SP 9 5
person - the person to add into the security array initialy
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on a failure, 1 on success

.EP

.SI 3
* %^BOLD%^delete_board%^RESET%^
.EI
.SI 5
int delete_board(string board)
.EI
.SP 7 5

Complete erase a board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to delete
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure and 1 on success

.EP

.SI 3
* %^BOLD%^delete_message%^RESET%^
.EI
.SI 5
int delete_message(string board, int off, string override_name)
.EI
.SP 7 5

Remove a message from a board.  The offset is the offset into the
subjects array.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to remove the message from
.EP
.SP 9 5
off - the offset to delete
.EP
.SP 9 5
override_name - used by the web boards
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure and 1 on success

.EP

.SI 3
* %^BOLD%^expire_boards%^RESET%^
.EI
.SI 5
void expire_boards()
.EI
.SP 7 5

This method runs through all the messages and expires any which are
too old.

.EP

.SI 3
* %^BOLD%^force_board_method_access_restricted%^RESET%^
.EI
.SI 5
int force_board_method_access_restricted(string board)
.EI
.SP 7 5

This method changes the type of the board to be a method access call
access restriction, instead of whatever it had before.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the name of the board to control the access for

.EP

.SI 3
* %^BOLD%^get_message%^RESET%^
.EI
.SI 5
string get_message(string board, int num, string person)
.EI
.SP 7 5

Get the text of a specific message.  This will look up the
text on a board with the given number and return that to the
caller.  The number is the offset into the subject array in
which to get the message from.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board name to get the message from
.EP
.SP 9 5
num - the message number to use
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the message or 0 if it failed

.EP

.SI 3
* %^BOLD%^get_subjects%^RESET%^
.EI
.SI 5
varargs mixed get_subjects(string name, string person)
.EI
.SP 7 5

Get the subjects for the specifed board.  The subjects are
returns in a special array format.  See the include file for the
defines to get at the members of the array.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the board name to lookup
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the subject array

.EP

.SI 3
* %^BOLD%^is_board%^RESET%^
.EI
.SI 5
int is_board(string board)
.EI
.SP 7 5

This method tests to see if the board exists.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the name of the board to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the board exists, 0 if it does not

.EP

.SI 3
* %^BOLD%^is_idiot%^RESET%^
.EI
.SI 5
int is_idiot(string name)
.EI
.SP 7 5

This method checks to see if the specified person is an idiot or
not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are an idiot

.EP

.SI 3
* %^BOLD%^list_killfile%^RESET%^
.EI
.SI 5
string * list_killfile(string player)
.EI
.SP 7 5

Retrieve a players killfile list

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
string - the players name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
string * the list of boards in their killfile

.EP

.SI 3
* %^BOLD%^list_of_boards%^RESET%^
.EI
.SI 5
string * list_of_boards()
.EI
.SP 7 5

The names of all the boards.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of all the boards

.EP

.SI 3
* %^BOLD%^query_archive%^RESET%^
.EI
.SI 5
string query_archive(string board)
.EI
.SP 7 5

Return the archive file location for the board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to get the archive location for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the archive file location, 0 on failure

.EP

.SI 3
* %^BOLD%^query_board_order%^RESET%^
.EI
.SI 5
string * query_board_order(string player)
.EI
.SP 7 5

Retrieve someones chosen board order.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
string - the players name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of boards, in order

.EP

.SI 3
* %^BOLD%^query_boards%^RESET%^
.EI
.SI 5
string * query_boards()
.EI
.SP 7 5

Returns the names of all the boards.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the names of all the boards

.EP

.SI 3
* %^BOLD%^query_cache%^RESET%^
.EI
.SI 5
void query_cache()
.EI

.SI 3
* %^BOLD%^query_idiot%^RESET%^
.EI
.SI 5
int query_idiot(string idiot)
.EI
.SP 7 5

This method returns if they are an idiot and when their period of
idiocy runs out.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
idiot - the idiot to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if not an idiot, the time left otherwise

.EP

.SI 3
* %^BOLD%^query_killfile%^RESET%^
.EI
.SI 5
int query_killfile(string player, string board)
.EI
.SP 7 5

Find out if a given board is in a player killfile

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
string - the name of the player
.EP
.SP 9 5
string - the name of the board (in lowercase)
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is, 0 if it isn't

.EP

.SI 3
* %^BOLD%^query_maximum%^RESET%^
.EI
.SI 5
int query_maximum(string board)
.EI
.SP 7 5

Return the maximum number of message allowed on the board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to get the maximum numbr of message for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, the maximum number of messages on success

.EP

.SI 3
* %^BOLD%^query_minimum%^RESET%^
.EI
.SI 5
int query_minimum(string board)
.EI
.SP 7 5

Return the minimum number of message allowed on the board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to get the minimum numbr of message for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure, the minimum number of messages on success

.EP

.SI 3
* %^BOLD%^query_newsrc%^RESET%^
.EI
.SI 5
mapping query_newsrc(string player)
.EI
.SP 7 5

Get someones newsrc

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
string - the name of the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
mapping their newsrc

.EP

.SI 3
* %^BOLD%^query_no_inform%^RESET%^
.EI
.SI 5
int query_no_inform(string board)
.EI
.SP 7 5

This method checks to see if the board is in a no inform mode.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the name of the board to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is no inform, 0 if not

.EP

.SI 3
* %^BOLD%^query_num%^RESET%^
.EI
.SI 5
int query_num()
.EI
.SP 7 5
The current max board number. 
.EP

.SI 3
* %^BOLD%^query_prevent_shadow%^RESET%^
.EI
.SI 5
int query_prevent_shadow()
.EI
.SP 7 5
Prevents the object from being shadowed. 
.EP

.SI 3
* %^BOLD%^query_privilage%^RESET%^
.EI
.SI 5
int query_privilage(string board)
.EI
.SP 7 5

This method returns the current privilage level of the board in
question.  This should be used for testing only.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current privilage level

.EP

.SI 3
* %^BOLD%^query_read_only%^RESET%^
.EI
.SI 5
int query_read_only(string board)
.EI
.SP 7 5

This method checks to see if the board is in a read only mode.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the name of the board to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is read only, 0 if not

.EP

.SI 3
* %^BOLD%^query_restricted_access%^RESET%^
.EI
.SI 5
int query_restricted_access(string board)
.EI
.SP 7 5

This method checks to see if the board is in restricted access mode.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the name of the board to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is, 0 if it is not

.EP

.SI 3
* %^BOLD%^query_restricted_access_file%^RESET%^
.EI
.SI 5
int query_restricted_access_file(string board)
.EI
.SP 7 5

This method checkes to see if the board is in restricted access file
mode.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the name of the board to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is, 0 if it is not

.EP

.SI 3
* %^BOLD%^query_security%^RESET%^
.EI
.SI 5
string * query_security(string board)
.EI
.SP 7 5

Returns the security array for the given board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to get the security array for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the security array

.EP

.SI 3
* %^BOLD%^query_timeout%^RESET%^
.EI
.SI 5
int query_timeout(string board)
.EI
.SP 7 5

Return the timeout time of the board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to get the timeout for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the timeout in seconds
.EP

.SI 3
* %^BOLD%^remove_allowed%^RESET%^
.EI
.SI 5
int remove_allowed(string board, string name)
.EI
.SP 7 5

Remove someone from the allowed array of the board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to remove the person from
.EP
.SP 9 5
name - the name of the person to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on nfailure and 1 on success

.EP

.SI 3
* %^BOLD%^remove_killfile%^RESET%^
.EI
.SI 5
int remove_killfile(string player, string board)
.EI

.SI 3
* %^BOLD%^save_me%^RESET%^
.EI
.SI 5
void save_me()
.EI
.SP 7 5

Saves the state of the object.

.EP

.SI 3
* %^BOLD%^set_archive%^RESET%^
.EI
.SI 5
int set_archive(string board, string file)
.EI
.SP 7 5

Set the archive file location.  This is where all deleted messages
wil be stored.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to set the archive for
.EP
.SP 9 5
file - the file name to set it to
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure and 1 on success

.EP

.SI 3
* %^BOLD%^set_board_order%^RESET%^
.EI
.SI 5
int set_board_order(string player, string * new_order)
.EI
.SP 7 5

Set the order boards should be shown in for a given player.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
string - a players name
.EP
.SP 9 5
string - * the list of boards
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure

.EP

.SI 3
* %^BOLD%^set_killfile%^RESET%^
.EI
.SI 5
int set_killfile(string player, string board)
.EI
.SP 7 5

Add a board to someones killfile

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
string - the name of the player
.EP
.SP 9 5
string - the name of the board (in lowercase)
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure

.EP

.SI 3
* %^BOLD%^set_maximum%^RESET%^
.EI
.SI 5
int set_maximum(string board, int max)
.EI
.SP 7 5

Set the maximum number of message before they start being auto deleted
when someone posts to the board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to set the maximum for
.EP
.SP 9 5
max - the maximum number of messages
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if it failed and 1 on success

.EP

.SI 3
* %^BOLD%^set_method_access_call%^RESET%^
.EI
.SI 5
int set_method_access_call(string board, string method, string name)
.EI
.SP 7 5

This method sets the method to call to check for allowed postings
to a board setup as an method controlled post board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the name of the board to setup the method for
.EP
.SP 9 5
method - the method to call on the object
.EP
.SP 9 5
name - the object to call the method on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if the method failed, 1 if it was successful

.EP

.SI 3
* %^BOLD%^set_minimum%^RESET%^
.EI
.SI 5
int set_minimum(string board, int min)
.EI
.SP 7 5

Sets the minimum number of message to keep on a board.  If there is less
than this
number then they will not be auto deleted.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to set the minimum for
.EP
.SP 9 5
min - the number of message to keep
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure and 1 on success

.EP

.SI 3
* %^BOLD%^set_newsrc%^RESET%^
.EI
.SI 5
int set_newsrc(string player, mapping newsrc)
.EI
.SP 7 5

Set someones newsrc

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
string - the name of the player
.EP
.SP 9 5
mapping - their new newsrc
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for success, 0 for failure

.EP

.SI 3
* %^BOLD%^set_timeout%^RESET%^
.EI
.SI 5
int set_timeout(string board, int timeout)
.EI
.SP 7 5

Change the time before a message automatic gets deleted off a
board.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the name of the board to set the timeout for
.EP
.SP 9 5
timeout - the timeout (in seconds)
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on failure and 1 on success

.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI

.SI 3
* %^BOLD%^test_can_delete%^RESET%^
.EI
.SI 5
int test_can_delete(string board, object previous, string name)
.EI
.SP 7 5

Check to see if delete is allowed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to check
.EP
.SP 9 5
previous - the previous object
.EP
.SP 9 5
name - the name of the person doing stuff
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is allowed, 0 if not

.EP

.SI 3
* %^BOLD%^test_can_read%^RESET%^
.EI
.SI 5
int test_can_read(string board, object previous, string name)
.EI
.SP 7 5

Check to see if read access is allowed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to check
.EP
.SP 9 5
previous - the previous object
.EP
.SP 9 5
name - the name of the person doing stuff
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is allowed, 0 if not

.EP

.SI 3
* %^BOLD%^test_can_write%^RESET%^
.EI
.SI 5
int test_can_write(string board, object previous, string name)
.EI
.SP 7 5

Check to see if write access is allowed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
board - the board to check
.EP
.SP 9 5
previous - the previous object
.EP
.SP 9 5
name - the name of the person doing stuff
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is allowed, 0 if not

.EP


