.DT
room_handler.c
Disk World autodoc help
room_handler.c

.SH Description
.SP 5 5

This file contains all the bits needed to handle rooms, plus the code to
handle following when moving.
.EP
.SP 10 5


Written by Pinkfish
.EP



.SH See also
.SP 5 5
/std/room/basic_room.c

.EP
.SH Includes
.SP 5 5
This class includes the following files /include/position.h, /include/player.h, /include/move_failures.h, /include/climate.h, /include/room.h, /include/living.h, /include/weather.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_chatter%^RESET%^
.EI
.SI 5
void add_chatter(object thing, int number)
.EI

.SI 3
* %^BOLD%^add_door%^RESET%^
.EI
.SI 5
void add_door(object thing)
.EI

.SI 3
* %^BOLD%^add_exit_type%^RESET%^
.EI
.SI 5
int add_exit_type(string type, mixed message, mixed obvious, int size, mixed func)
.EI
.SP 7 5

This method adds an exit type to the current list of available exit types.
This is used when the room handler is setup to add all the used exit
types.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the name of the exit type
.EP
.SP 9 5
message - the message to display when going through the exit
.EP
.SP 9 5
obvious - if the exit is obvious or not
.EP
.SP 9 5
size - the size of the exit (used for heigh restrictions)
.EP
.SP 9 5
func - the function to call when using the exit
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successfuly added, 0 if not
.EP

.SI 3
* %^BOLD%^check_chatters%^RESET%^
.EI
.SI 5
void check_chatters()
.EI

.SI 3
* %^BOLD%^check_door%^RESET%^
.EI
.SI 5
void check_door(mixed args)
.EI
.SP 7 5

This method checks to see if the door exists or not.
It is passed in the room we are going from and the direction the
exit faces in the array.
.EP
.SP 7 5
({ room_from, direction })
.EP
.SP 7 5
This is the function which generates those door xx not found messages.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
args - the arguements passed into the function
.EP

.SI 3
* %^BOLD%^check_doors%^RESET%^
.EI
.SI 5
void check_doors()
.EI

.SI 3
* %^BOLD%^exit_move%^RESET%^
.EI
.SI 5
int exit_move(string verb, string extra, mixed special, object thing)
.EI
.SP 7 5

This is the main code for moving someone.  The move_thing code above
should not be called directly.  This code handlers all the followers
and any other things that need to be handled.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
verb - the movement verb
.EP
.SP 9 5
extra - extra information
.EP
.SP 9 5
special - special informaiton
.EP
.SP 9 5
thing - the thing to move
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^housekeeping%^RESET%^
.EI
.SI 5
void housekeeping()
.EI

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping maps)
.EI

.SI 3
* %^BOLD%^move_thing%^RESET%^
.EI
.SI 5
int move_thing(object thing, string dir, string dest, mixed exit, mixed enter, string move)
.EI
.SP 7 5

THis is the code that actually moves the thing around the place.
It handles all the weirdness involved with dragging things and other
such stuff.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - what is being moved
.EP
.SP 9 5
dir - the direction we are going
.EP
.SP 9 5
dest - the destionation room
.EP
.SP 9 5
exit - the exit name
.EP
.SP 9 5
enter - the enter name
.EP
.SP 9 5
move - the string to tell the object when it moves
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^query_chatters%^RESET%^
.EI
.SI 5
mixed * query_chatters()
.EI
.SP 7 5

This method returns the current list of rooms that are enabled for
chatting.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current chatters

.EP

.SI 3
* %^BOLD%^query_door_type%^RESET%^
.EI
.SI 5
mixed * query_door_type(string type, string direc, string dest)
.EI
.SP 7 5

This method returns information about the door type, the door has
extra information associated with it than the standard exit type.
This function does a double job of trying to find the corresponding
door on the other side of the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of the door
.EP
.SP 9 5
direc - the direction the door points
.EP
.SP 9 5
dest - the destination of the door
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the door type array of information

.EP

.SI 3
* %^BOLD%^query_doors%^RESET%^
.EI
.SI 5
mixed * query_doors()
.EI
.SP 7 5

This method returns the current list of doors handled by the room
handler.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current array of doors

.EP

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^query_exit_type%^RESET%^
.EI
.SI 5
mixed * query_exit_type(string type, string dir)
.EI
.SP 7 5

This method returns the information associated with the exit type.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the exit type to query
.EP
.SP 9 5
dir - the direction the type information is for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a huge amount of info as specified above

.EP

.SI 3
* %^BOLD%^query_opposite_direction%^RESET%^
.EI
.SI 5
string query_opposite_direction(string dir)
.EI
.SP 7 5

This method returns the opposite direction to this exit.  This should
only be used for printing, since it is not a useful
real name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the direction to get the opposite of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the opposite direction

.EP

.SI 3
* %^BOLD%^remove_exit_type%^RESET%^
.EI
.SI 5
int remove_exit_type(string type)
.EI
.SP 7 5

This method remove the named exit from the type list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the name of the exit type to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1
.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


