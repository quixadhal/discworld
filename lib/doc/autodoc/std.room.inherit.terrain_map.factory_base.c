.DT
factory_base.c
Disk World autodoc help
factory_base.c

.SH Description
.SP 5 5

This is specifically for creating rooms in the world map system, where
it just makes and passes out the room details.  So it does not need to
include all the room crap.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Jun 28 16:14:19 PDT 2002

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/terrain_map.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_adjacent_item%^RESET%^
.EI
.SI 5
void add_adjacent_item(string name, string desc)
.EI
.SP 7 5

This method sets an item which will be visible from adjacent rooms.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the item
.EP
.SP 9 5
desc - the description of the item

.EP

.SI 3
* %^BOLD%^find_base_terrain_room%^RESET%^
.EI
.SI 5
string find_base_terrain_room(int x, int y, int z, string base_name, int map_room, string ref base)
.EI
.SP 7 5

This method figures out the base path to use for the specified room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x position
.EP
.SP 9 5
y - the y position
.EP
.SP 9 5
z - the z position
.EP
.SP 9 5
air - checking upwards in air land.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the base room

.EP

.SI 3
* %^BOLD%^query_adjacent_description%^RESET%^
.EI
.SI 5
string query_adjacent_description()
.EI
.SP 7 5

This method returns the string to use for adjancent descriptions.  THis is
the bit tacked onto the end of the long in another room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the adjacent description

.EP

.SI 3
* %^BOLD%^query_adjacent_items%^RESET%^
.EI
.SI 5
mapping query_adjacent_items()
.EI
.SP 7 5

This method returns all the adjacent items.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping of all the adjacent items

.EP

.SI 3
* %^BOLD%^query_default_height%^RESET%^
.EI
.SI 5
int query_default_height()
.EI
.SP 7 5

This method returns the default height for the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the default height for the room

.EP

.SI 3
* %^BOLD%^query_follow_road%^RESET%^
.EI
.SI 5
int query_follow_road()
.EI
.SP 7 5

This method returns the follow road flag for the road.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the follow road flag

.EP

.SI 3
* %^BOLD%^query_minimum_height%^RESET%^
.EI
.SI 5
int query_minimum_height()
.EI
.SP 7 5

This method returns the mimum height allowed for this factory.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mimuim height

.EP

.SI 3
* %^BOLD%^query_room_jump_size%^RESET%^
.EI
.SI 5
int query_room_jump_size()
.EI
.SP 7 5

This method returns the room jump size for the room
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the room jump size

.EP

.SI 3
* %^BOLD%^set_adjacent_description%^RESET%^
.EI
.SI 5
void set_adjacent_description(string desc)
.EI
.SP 7 5

This method sets the string to use for adjancent descriptions.  THis is
the bit tacked onto the end of the long in another room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
desc - the adjacent description

.EP

.SI 3
* %^BOLD%^set_default_height%^RESET%^
.EI
.SI 5
void set_default_height(int height)
.EI
.SP 7 5

This method sets the default height for the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
def - the default height for the room

.EP

.SI 3
* %^BOLD%^set_follow_road%^RESET%^
.EI
.SI 5
void set_follow_road(int follow)
.EI
.SP 7 5

This method sets the flag that makes us follow the road.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
follow - the follow road flag

.EP

.SI 3
* %^BOLD%^set_height_rooms%^RESET%^
.EI
.SI 5
void set_height_rooms(mapping map)
.EI
.SP 7 5

This method sets the height information for the room, setting up
which rooms are used at which heights.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
heights - the heights for the rooms

.EP

.SI 3
* %^BOLD%^set_minimum_height%^RESET%^
.EI
.SI 5
void set_minimum_height(int height)
.EI
.SP 7 5

This method sets the minimum height, this is only really useful
in locations which have multiple depth rooms.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
height - the minimum height 
.EP

.SI 3
* %^BOLD%^set_room_jump_size%^RESET%^
.EI
.SI 5
void set_room_jump_size(int jump)
.EI
.SP 7 5

This method sets the room jump size for the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
jump - the room jump size

.EP

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed stats()
.EI


