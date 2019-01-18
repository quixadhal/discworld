.DT
club_owned_room.c
Disk World autodoc help
club_owned_room.c

.SH Description
.SP 5 5

This is the same as an owned room, but it is controlled by a club
instead.
.EP
.SP 10 5


Written by Pinkfish

Started 16th of May

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/inherit/owned_room.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_club_controller%^RESET%^
.EI
.SI 5
string query_club_controller()
.EI
.SP 7 5

This method returns the club controller for the club.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the club controller

.EP

.SI 3
* %^BOLD%^query_owned_id%^RESET%^
.EI
.SI 5
string query_owned_id()
.EI
.SP 7 5

This method sets the owned id to use for this room.  The id is then
used to query the club controller to see who owns the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id to set the room to use
.EP

.SI 3
* %^BOLD%^set_club_controller%^RESET%^
.EI
.SI 5
void set_club_controller(string club_controller)
.EI
.SP 7 5

This method sets the club controller of the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
club_controller - the club controller

.EP

.SI 3
* %^BOLD%^set_owned_id%^RESET%^
.EI
.SI 5
void set_owned_id(string id)
.EI
.SP 7 5

This method sets the owned id to use for this room.  The id is then
used to query the club controller to see who owns the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id to set the room to use
.EP


