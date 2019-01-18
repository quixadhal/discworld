.DT
start_position.c
Disk World autodoc help
start_position.c

.SH Description
.SP 5 5

This file contains all the start position related code.  It sets up
and controls the start location of the player.  This was split off from
the main player code by Pinkfish in April 1998.
.EP
.SP 10 5


Written by Pinkfish

.EP



.SH See also
.SP 5 5
/global/player.c
.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/virtual_quit_control.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/config.h and /include/playerinfo.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_start%^RESET%^
.EI
.SI 5
void add_start(string start_file, string start_desc)
.EI
.SP 7 5

This method will add a new start location to the player.  The
description will be used in the starts command.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
start_file - the file name of the start location
.EP
.SP 9 5
start_desc - the description of the start location
.EP

.SI 3
* %^BOLD%^check_last_pos%^RESET%^
.EI
.SI 5
void check_last_pos()
.EI
.SP 7 5

This method figured out what the last saved position was and sets it
on the player.  This basicly sets the last saved postion.
.EP

.SI 3
* %^BOLD%^inform_reconnect_game%^RESET%^
.EI
.SI 5
void inform_reconnect_game()
.EI
.SP 7 5

THis method is called to inform people that someone has just reconnected.

.EP

.SI 3
* %^BOLD%^query_last_pos%^RESET%^
.EI
.SI 5
string query_last_pos()
.EI
.SP 7 5

This method returns the last saved position of the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the last saved position

.EP

.SI 3
* %^BOLD%^query_saved_co_ords%^RESET%^
.EI
.SI 5
int * query_saved_co_ords()
.EI
.SP 7 5

This method returns the co-ordinates of the room in which the player
last saved.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the last saved co-ordinates

.EP

.SI 3
* %^BOLD%^query_start_pos%^RESET%^
.EI
.SI 5
string query_start_pos()
.EI
.SP 7 5

This method returns the starting location of the player.  This the
location they will start at if the saved position does not work
for some reason.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the start position to use
.EP

.SI 3
* %^BOLD%^query_starts%^RESET%^
.EI
.SI 5
string * query_starts()
.EI
.SP 7 5

This method returns the current array of possible start positions on
the player.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of possible start locations
.EP

.SI 3
* %^BOLD%^remove_start%^RESET%^
.EI
.SI 5
void remove_start(string start_file)
.EI
.SP 7 5

This method will remove a start location from the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
start_file - the file name of the location to remove
.EP

.SI 3
* %^BOLD%^reset_starts%^RESET%^
.EI
.SI 5
void reset_starts()
.EI
.SP 7 5

This method resets the start locations back to an empty array.
.EP

.SI 3
* %^BOLD%^set_start_location%^RESET%^
.EI
.SI 5
void set_start_location(string start_file)
.EI
.SP 7 5

This method sets the start location of the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
start_file - the start location to set
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^inform_entered_game%^RESET%^
.EI
.SI 5
void inform_entered_game()
.EI
.SP 7 5

This method handles informing people that the player has entered the
game.
.EP

.SI 3
* %^BOLD%^move_to_start_pos%^RESET%^
.EI
.SI 5
void move_to_start_pos()
.EI
.SP 7 5

This method moves the player to the last saved position.  This should
only be called in the startup sequence.  If the last location could
not be loaded, the the start position will be used.
.EP

.SI 3
* %^BOLD%^set_last_pos%^RESET%^
.EI
.SI 5
void set_last_pos(string pos)
.EI
.SP 7 5

This method sets the current start location.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
last_pos - the start location

.EP


