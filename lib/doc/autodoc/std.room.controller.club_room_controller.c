.DT
club_room_controller.c
Disk World autodoc help
club_room_controller.c

.SH Description
.SP 5 5

THis is the controller object to deal with the club rooms.  It
keeps track of which club owns the room, if the club room has
a board and any ownership issues that are needed.
.EP
.SP 10 5


Written by Pinkfish

Started May 16th

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/setup.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/am_time.h, /include/room/club_room.h and /include/clubs.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^ownership%^RESET%^
class ownership {
string name;
int type;
int control_type;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_controlled_ownership%^RESET%^
.EI
.SI 5
int add_controlled_ownership(string name, int type, int default_type, mixed default_name)
.EI
.SP 7 5

This method adds an item to have ownership.  If a function pointer
is set as the default name, then the method will be evaluated each
time the club changes.  The first arguement will be the name of the
owned item being changed and the second argument will be the new
club name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the option that needs ownership control
.EP
.SP 9 5
type - the type of controlled ownership
.EP
.SP 9 5
default_type - the default ownership of the item
.EP
.SP 9 5
default_name - the default name of the owner
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^discuss_elect_ownership%^RESET%^
.EI
.SI 5
mixed discuss_elect_ownership(string message, string club_name, class discussion_idea frog, string arg1, string arg2)
.EI
.SP 7 5

This is the function used for electing whoever gets to have the 
nice owned room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
message - the message being sent
.EP
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
frog - the discussion item
.EP
.SP 9 5
arg1 - the first argument
.EP
.SP 9 5
arg2 - the second argument

.EP

.SI 3
* %^BOLD%^discuss_nominate_ownership%^RESET%^
.EI
.SI 5
mixed discuss_nominate_ownership(string message, string club_name, class discussion_idea frog, string arg1, string arg2)
.EI
.SP 7 5

This is the function used for nominating someone exciting the the
owned position of the club rooms.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
message - the message being sent
.EP
.SP 9 5
club_name - the name of the club
.EP
.SP 9 5
frog - the discussion item
.EP
.SP 9 5
arg1 - the first argument
.EP
.SP 9 5
arg2 - the second argument

.EP

.SI 3
* %^BOLD%^is_option%^RESET%^
.EI
.SI 5
int is_option(string name)
.EI
.SP 7 5

This method checks to see if there is an option associated with the
club room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the optionto check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if there is, 0 if not

.EP

.SI 3
* %^BOLD%^load_me%^RESET%^
.EI
.SI 5
void load_me()
.EI
.SP 7 5

This method loads all the data from the save file.

.EP

.SI 3
* %^BOLD%^query_board_associated%^RESET%^
.EI
.SI 5
int query_board_associated()
.EI
.SP 7 5

This method queries to see if there is a board associated or not
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if there is aboard associated with this club room set

.EP

.SI 3
* %^BOLD%^query_club%^RESET%^
.EI
.SI 5
string query_club()
.EI
.SP 7 5

This method queries the club that currently owns the set of rooms.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the club that owns the rooms

.EP

.SI 3
* %^BOLD%^query_option%^RESET%^
.EI
.SI 5
mixed query_option(string name)
.EI
.SP 7 5

This method queries and option associated with the club room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the option
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value of the option

.EP

.SI 3
* %^BOLD%^query_room_save_file_name%^RESET%^
.EI
.SI 5
string query_room_save_file_name(mixed fname)
.EI
.SP 7 5

This method returns the save file name to use for the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the file name to use for the room
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the save file name to use

.EP

.SI 3
* %^BOLD%^query_save_directory%^RESET%^
.EI
.SI 5
string query_save_directory()
.EI
.SP 7 5

This method returns the save file for the club room controller.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the save file for the club room controller

.EP

.SI 3
* %^BOLD%^remove_option%^RESET%^
.EI
.SI 5
int remove_option(string name)
.EI
.SP 7 5

This remove removes an option associated with the club room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the optoin
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^save_me%^RESET%^
.EI
.SI 5
void save_me()
.EI
.SP 7 5

This method saves all the data to the save file.

.EP

.SI 3
* %^BOLD%^set_board_associated%^RESET%^
.EI
.SI 5
void set_board_associated(int board_associated)
.EI
.SP 7 5

This method sets if a board is associated with the club room set or
not.
.EP

.SI 3
* %^BOLD%^set_club%^RESET%^
.EI
.SI 5
int set_club(string club)
.EI
.SP 7 5

This method sets the club that currently owns the set of rooms.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club - the club that owns the rooms
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^set_controlled_ownership_owner%^RESET%^
.EI
.SI 5
int set_controlled_ownership_owner(string controlled, string owner, int type)
.EI
.SP 7 5

This method sets the owner of the controlled ownership type.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
controlled - the item being controlled
.EP
.SP 9 5
owner - the owner of the item
.EP
.SP 9 5
type - the type of the owner
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^set_option%^RESET%^
.EI
.SI 5
int set_option(string name, mixed value)
.EI
.SP 7 5

This method sets the value of the option associated with the club room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the option
.EP
.SP 9 5
value - the new value for the option
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^set_save_directory%^RESET%^
.EI
.SI 5
void set_save_directory(string save_directory)
.EI
.SP 7 5

This method sets the save directory for the club room controller.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
save_file - the new save file

.EP


