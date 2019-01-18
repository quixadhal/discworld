.DT
start_positions.c
Disk World autodoc help
start_positions.c

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h and /include/login_handler.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_buggy_start%^RESET%^
.EI
.SI 5
int add_buggy_start(string fname)
.EI
.SP 7 5

Add a filename to the array of buggy start positions to be removed.
Returns 1 if successful, -1 if the start position was already in the
list, or 0 for an error.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - The filename of the room.  '.c' will be stripped.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The integer success code.

.EP

.SI 3
* %^BOLD%^delete_buggy_start%^RESET%^
.EI
.SI 5
int delete_buggy_start(string fname)
.EI
.SP 7 5

Remove a filename from the array of buggy start positions.
Returns 1 if successful, -1 if the start position was not in the list,
or 0 for an error.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - The filename of the room.  '.c' will be stripped.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The integer success code.

.EP

.SI 3
* %^BOLD%^list_buggy_starts%^RESET%^
.EI
.SI 5
string * list_buggy_starts()
.EI
.SP 7 5

Get a list of all the currently registered buggy start positions.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The list of filenames.

.EP

.SI 3
* %^BOLD%^load_file%^RESET%^
.EI
.SI 5
void load_file()
.EI

.SI 3
* %^BOLD%^login_callback%^RESET%^
.EI
.SI 5
void login_callback(string player, string type)
.EI
.SP 7 5

This is the callback function registered with the login handler.  It
takes two arguments: the player and the type of login event.  It checks
a property on the player so that it doesn't process the list of buggy
starts once for each player every single time they log in.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player's name to check.
.EP
.SP 9 5
type - the type of login event, as defined in <login_handler.h>.

.EP

.SI 3
* %^BOLD%^remove_buggy_starts%^RESET%^
.EI
.SI 5
void remove_buggy_starts(object player)
.EI
.SP 7 5

Process a player and remove start positions as required.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - The player object to check.

.EP


