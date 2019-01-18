.DT
terrain_room.c
Disk World autodoc help
terrain_room.c

.SH Description
.SP 5 5
 Base inherit for a terrain_map room.


This file should be inherited by any room within a terrain mapped virtual area. 

When inheriting from this file, it is essential that the function query_handler_path() be overriden to return the full pathname of the handler associated with this area. 

To inherit from this file, use the following lines: 
.EP
.SO 6 2 -10

	*	#include <terrain_map.h> 
	*	
	*	inherit TERRAIN_MAP_OUTSIDE_BASE 
.EO
.SP 5 5

.EP
.SP 10 5


Written by Dek

Started Thurs April 12 2001 Too Damn Early BST

.EP



.SH See also
.SP 5 5
terrain_map_handler, terrain_map_intro and terrain_map_example
.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/inherit/terrain_map/factory_base.c.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/terrain_map.h, /include/login_handler.h and /include/room.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^feature%^RESET%^
class feature {
mapping direcs;
mixed items;
int sentance;
int index;
int visible;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_default_exit%^RESET%^
.EI
.SI 5
void add_default_exit(string exit, string location)
.EI
.SP 7 5

This method adds a default exit into the list of default exits.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
exit - the exit to make variable
.EP
.SP 9 5
location - this is the walk exit location

.EP

.SI 3
* %^BOLD%^add_variable_exit%^RESET%^
.EI
.SI 5
void add_variable_exit(string exit)
.EI
.SP 7 5

This method adds a variable exit into the list of variable exits.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
exit - the exit to make variable

.EP

.SI 3
* %^BOLD%^can_enter_from%^RESET%^
.EI
.SI 5
int can_enter_from(string type, string direc, string base_path)
.EI
.SP 7 5

This method may be overriden to prevent the handler from setting up
exits from one or more types of location.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of location the exit would be from. This is the same as 
the character used in the ASCII map.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
return 1 if the exits should be made, 0 otherwise
.EP

.SI 3
* %^BOLD%^can_exit_to%^RESET%^
.EI
.SI 5
int can_exit_to(string type, string direc, string base_path)
.EI
.SP 7 5

This method may be overriden to prevent the handler from setting up
exits to one or more types of location.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of location the exit would be to. This is the same as 
the character used in the ASCII map.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
return 1 if the exits should be made, 0 otherwise
.EP

.SI 3
* %^BOLD%^can_move_between%^RESET%^
.EI
.SI 5
int can_move_between(string my_type, string direc, string first_type, string second_type, string new_base_path)
.EI
.SP 7 5

This method may be overriden to allow the handler to setup diagonal exits
between terrain types that are different.  What this does, here is
a small explaination.  If you have a map below:

.EP
.SP 7 5
   ff-
.EP
.SP 7 5
   f-f
.EP
.SP 7 5
   -ff
.EP
.SP 7 5

.EP
.SP 7 5
Where the '-'s are roads, you do not want to let someone diagonaly
.EP
.SP 7 5
jump the road.  So you make it so that you cannot go from the
.EP
.SP 7 5
f on one side of the road to the other.  THis is the default behaviour,
.EP
.SP 7 5
however you want people to be able to actualy walk diagonaly on the road
.EP
.SP 7 5
so you need to override this to allow the road to be walked on.
.EP
.SP 7 5
exits from one or more types of location.
.EP
.SP 7 5



.EP
.SP 7 5
The values passed in are the paths to the final object files.  Not the
.EP
.SP 7 5
key values in the map.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
my_type - our type
.EP
.SP 9 5
direc - the direction we are checking in
.EP
.SP 9 5
first_type - the first neighbouring path
.EP
.SP 9 5
second_type - the second neighbouring path
.EP
.SP 9 5
new_base_path - the path of the thing that is the destination
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
return 1 if the exits should be made, 0 otherwise
.EP

.SI 3
* %^BOLD%^can_view_adjacent_desc%^RESET%^
.EI
.SI 5
int can_view_adjacent_desc(string char, string key_here, string path, int z, string direc)
.EI
.SP 7 5

If this can be seen from this location or not.  This can do things
like dealing with lower levels and stuff.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
char - the character of the item in the map
.EP
.SP 9 5
path - the path of the room (0 if an obstacle)
.EP
.SP 9 5
z - the z co-ordinate of the room
.EP
.SP 9 5
direc - the direction in which the obstacle faces

.EP

.SI 3
* %^BOLD%^can_view_feature%^RESET%^
.EI
.SI 5
int can_view_feature(string name)
.EI
.SP 7 5

This method may be overriden to mask out feature descriptions and chats
from one or more features in the current terrain map.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the feature being queried.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
return 1 if the feature can be viewed from this room, 0 otherwise
.EP

.SI 3
* %^BOLD%^can_view_feature_from%^RESET%^
.EI
.SI 5
int can_view_feature_from(string name, string direc, int distance)
.EI
.SP 7 5

This method may be overriden to mask out feature descriptions and chats
from one or more features in the current terrain map.  It allows more
detailed blocking, based on distance and direction.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the feature being queried.
.EP
.SP 9 5
direc - the direction to the feature
.EP
.SP 9 5
distance - the range to the feature
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
return 1 if the feature can be viewed from this room, 0 otherwise
.EP

.SI 3
* %^BOLD%^do_map%^RESET%^
.EI
.SI 5
int do_map()
.EI
.SP 7 5

This method draws a little debug map for the creator.

.EP

.SI 3
* %^BOLD%^do_map_terrain%^RESET%^
.EI
.SI 5
int do_map_terrain()
.EI
.SP 7 5

This method draws the map of the entire terrain.

.EP

.SI 3
* %^BOLD%^do_outside_chats%^RESET%^
.EI
.SI 5
int do_outside_chats()
.EI
.SP 7 5

This method may be overriden to prevent the handler from setting up
additional room_chats from appropriate adjacent locations.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
return 1 if the exits should be made, 0 otherwise
.EP

.SI 3
* %^BOLD%^hash%^RESET%^
.EI
.SI 5
int hash(int mod)
.EI
.SP 7 5

This method makes a deterministic hash value based on the real co-ordinates
of this room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mod - the modules to use on the return result
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a deterministicly random number between 0..mod -1

.EP

.SI 3
* %^BOLD%^hash_time%^RESET%^
.EI
.SI 5
int hash_time(int mod, int period)
.EI
.SP 7 5

This produces a deterministicly static value that only exists for a
specific period of time, before moving.  You can use this for movable
oasises for example.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mod - the modulus to get the random number over
.EP
.SP 9 5
period - the period the number should remain constant

.EP

.SI 3
* %^BOLD%^long_test%^RESET%^
.EI
.SI 5
string long_test(string str, int dark)
.EI

.SI 3
* %^BOLD%^query_default_exits%^RESET%^
.EI
.SI 5
mapping query_default_exits()
.EI
.SP 7 5

This method returns the default exits mapping.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the default exits mapping

.EP

.SI 3
* %^BOLD%^query_dest_coordinate_string%^RESET%^
.EI
.SI 5
string query_dest_coordinate_string(int x, int y, int z, string direc, string key)
.EI
.SP 7 5

This function allows you to change the destination location of the room
if nessessary.  This is useful for bridges and other things which need to
move people up or down one z-coordinate when they cross the line.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x coordinate
.EP
.SP 9 5
y - the y coordinate
.EP
.SP 9 5
z - the z coordinate
.EP
.SP 9 5
direct - the direction used to enter this location
.EP

.SI 3
* %^BOLD%^query_dest_dir%^RESET%^
.EI
.SI 5
mixed * query_dest_dir(mixed * data)
.EI
.SP 7 5

Mess with the data.

.EP

.SI 3
* %^BOLD%^query_dest_other%^RESET%^
.EI
.SI 5
mixed * query_dest_other(string exit, mixed * data)
.EI
.SP 7 5

This messes with the return values of the rooms.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
exit - the exit to look at
.EP
.SP 9 5
data - the actual data from the room code
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the messed with data

.EP

.SI 3
* %^BOLD%^query_direction_distance_str%^RESET%^
.EI
.SI 5
string query_direction_distance_str(int num, string dir)
.EI
.SP 7 5

This method returns a string which says how far they have moved.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - how many steps in the direction
.EP
.SP 9 5
dir - the direction

.EP

.SI 3
* %^BOLD%^query_enter_exit_direction%^RESET%^
.EI
.SI 5
string query_enter_exit_direction(string key, string key_here, string direc, object room, string base_path)
.EI
.SP 7 5

This method is used to setup any unusual sorts of entrances to 
specific room types.  For example so that you have to actually swim
to get into a river rather than acidentally walking into it.  If
this does return a string a default exit is not setup for this direction
so just going 'east' doesn't drop you in the river.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
key - the key of the map location we are coming from
.EP
.SP 9 5
key_here - my key of the character in the map
.EP
.SP 9 5
direc - the direction the exit is in
.EP
.SP 9 5
room - the room being setup
.EP
.SP 9 5
base_path - the base path of the object we are coming from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 to do nothing, a string to replace the main exit

.EP

.SI 3
* %^BOLD%^query_from_other%^RESET%^
.EI
.SI 5
varargs mixed * query_from_other(string type, string to_direc, string from_direc, string path_from)
.EI
.SP 7 5

The handler calls this function when setting up exits to allow the
room to set up any modify_exit parameters which should be applied
to exits leading from other locations of different types to this. 
Override it to provide any extra exit modifiers.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the ASCII character representing the room type the exit leads from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of exit modifiers. This should take the same form as
used in modify_exit
.EP

.SI 3
* %^BOLD%^query_map_handler%^RESET%^
.EI
.SI 5
object query_map_handler()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
returns the handler object associated with this terrain map.
.EP

.SI 3
* %^BOLD%^query_my_real_file_name%^RESET%^
.EI
.SI 5
string query_my_real_file_name()
.EI
.SP 7 5

This method returns the actual file name of this object.

.EP

.SI 3
* %^BOLD%^query_room_path%^RESET%^
.EI
.SI 5
string query_room_path(int x, int y, int z)
.EI
.SP 7 5

This method returns the path at the specific co-ordinate.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x co-ordinate
.EP
.SP 9 5
y - the y co-ordinate
.EP
.SP 9 5
z - the z co-ordinate
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the file name of the new room

.EP

.SI 3
* %^BOLD%^query_terrain_coords%^RESET%^
.EI
.SI 5
mixed * query_terrain_coords()
.EI
.SP 7 5

This returns the terrain coordinate of the room.
It returns an array that contains the x, y, z values
of the co-ordinate.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the terrain co-ordinate

.EP

.SI 3
* %^BOLD%^query_terrain_handler%^RESET%^
.EI
.SI 5
string query_terrain_handler()
.EI
.SP 7 5

This method returns the terrain handler for
the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the terrain handler

.EP

.SI 3
* %^BOLD%^query_terrain_map_block%^RESET%^
.EI
.SI 5
int query_terrain_map_block()
.EI
.SP 7 5

This method returns if the room is blocking or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
if the room is blocking or not

.EP

.SI 3
* %^BOLD%^query_terrain_map_character%^RESET%^
.EI
.SI 5
string query_terrain_map_character()
.EI
.SP 7 5

This method returns the character to use for this terrain in the player
visible map.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the terrain character

.EP

.SI 3
* %^BOLD%^query_terrain_map_journey_exit%^RESET%^
.EI
.SI 5
string query_terrain_map_journey_exit()
.EI
.SP 7 5

This method returns the prefix to use for 'journey' exits in this room.

.EP

.SI 3
* %^BOLD%^query_terrain_map_walk_exit%^RESET%^
.EI
.SI 5
string query_terrain_map_walk_exit()
.EI
.SP 7 5

This method returns the prefix to use for 'walk' exits in this room.

.EP

.SI 3
* %^BOLD%^query_to_other%^RESET%^
.EI
.SI 5
varargs mixed * query_to_other(string type, string to_direc, string from_direc, string path_to)
.EI
.SP 7 5

The handler calls this function when setting up exits to allow the
room to set up any modify_exit parameters which should be applied
to exits leading to other locations of different types to this. 
Override it to provide any extra exit modifiers.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the ASCII character representing the room type the exit leads to
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of exit modifiers. This should take the same form as
used in modify_exit
.EP

.SI 3
* %^BOLD%^query_to_same%^RESET%^
.EI
.SI 5
varargs mixed * query_to_same(string type, string to_direc, string from_direc, string path_to)
.EI
.SP 7 5

The handler calls this function when setting up exits to allow the
room to set up any modify_exit parameters which should be applied
to exits leading to other locations of the same type as this. 
Override it to provide any extra exit modifiers.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the ASCII character representing this room type
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of exit modifiers. This should take the same form as
used in modify_exit
.EP

.SI 3
* %^BOLD%^reset_handler%^RESET%^
.EI
.SI 5
void reset_handler()
.EI
.SP 7 5

This method destroys every single room in the associated terrain map.
It is provided for development purposes only, and should be used cautiously
as it will currently dump every single player in the map into the void.
Plans are afoot to make the side-effects a little less dire.

.EP

.SI 3
* %^BOLD%^set_terrain_handler%^RESET%^
.EI
.SI 5
void set_terrain_handler(string handler)
.EI
.SP 7 5

This method sets the terrain handler for the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
handler - the terrain handler

.EP

.SI 3
* %^BOLD%^set_terrain_map_block%^RESET%^
.EI
.SI 5
void set_terrain_map_block(int blocking)
.EI
.SP 7 5

This method sets if the particular room is see through or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
blocking - sets the blocking flag

.EP

.SI 3
* %^BOLD%^set_terrain_map_character%^RESET%^
.EI
.SI 5
void set_terrain_map_character(string terr)
.EI
.SP 7 5

This method sets the character to use for this terrain in the player
visible map.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
char - the character to use in the map

.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed stats()
.EI


