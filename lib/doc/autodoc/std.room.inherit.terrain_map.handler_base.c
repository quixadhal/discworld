.DT
handler_base.c
Disk World autodoc help
handler_base.c

.SH Description
.SP 5 5
 Base inherit for terrain map handler.


When creating a new terrain map, inherit from this file to make your area handler. 

When inheriting from this file, it is essential that the functions query_map_file() and setup_handler() be overriden to return the full pathname of the area map, and to set up type mappings. 

To inherit from this file, use the following lines: 
.EP
.SO 6 2 -10

	*	#include <terrain_map.h> 
	*	
	*	inherit TERRAIN_MAP_HANDLER_BASE 
.EO
.SP 5 5

.EP
.SP 10 5


Written by Dek

Started Thurs April 12 2001 Too Damn Early BST

.EP



.SH See also
.SP 5 5
terrain_map_inside, terrain_map_outside, terrain_map_intro and terrain_map_example
.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/inherit/terrain_map/distance_str.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player.h, /include/terrain_map.h, /include/playtesters.h and /include/weather.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^char_map_data%^RESET%^
class char_map_data {
string char;
string colour;
string path;
string room_desc;
mixed items;
mixed item_desc;
mixed chats;
int type;
int height;
mapping height_rooms;
int whole_sentance;
string adjacent_desc;
int room_jump;
int room_jump_matching;
}

.EI

.SI 3
* %^BOLD%^feature_data%^RESET%^
class feature_data {
int range;
string * descs;
string * night_descs;
mixed f_items;
mixed f_item_desc;
mixed f_item_night_desc;
int whole_sentance;
mixed chats;
mixed night_chats;
object feature_ob;
}

.EI

.SI 3
* %^BOLD%^real_coords%^RESET%^
class real_coords {
int x1;
int y1;
int x2;
int y2;
int xdiff;
int ydiff;
int xydiff;
int zdiff;
int in_world_map;
}

.EI

.SI 3
* %^BOLD%^sign_data%^RESET%^
class sign_data {
string long;
string read_mess;
string short;
string name;
string language;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_exit_at%^RESET%^
.EI
.SI 5
varargs void add_exit_at(int x, int y, string direc, string dest, string type, mixed modifiers)
.EI
.SP 7 5

This method is used to add a standard exit to a specified location
in a terrain map. The chief use is to insert exits to standard rooms
or other terrain maps.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x coordinate of the room (internal coordinate system)
.EP
.SP 9 5
y - the y coordinate of the room (internal coordinate system)
.EP
.SP 9 5
direc - the direction of the exit (as in add_exit)
.EP
.SP 9 5
dest - the path of the exit's destination (as in add_exit)
.EP
.SP 9 5
type - the type of exit (as in add_exit)
.EP
.SP 9 5
modifiers - an optional array of exit modifiers (as in modify_exit)
.EP

.SI 3
* %^BOLD%^add_feature%^RESET%^
.EI
.SI 5
void add_feature(string title, int x, int y, int width, int height, int range, string * descs, mixed items, mixed item_desc, string * night_descs, mixed night_items)
.EI
.SP 7 5

This method is called to add a new 'feature' to a terrain map.
Features have no real existance, but add atmosphere to a whole area,
by being visible across a large number of locations.
You define location, size, item desc, and add an array of 'longs'
sorted by distance, plus the usual item descriptions. Any occurrences
of $D in the longs array will be replaced by the direction in which
the feature lies.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the name of the feature
.EP
.SP 9 5
x - the x coordinate of the room (internal coordinate system)
.EP
.SP 9 5
y - the y coordinate of the room (internal coordinate system)
.EP
.SP 9 5
width - the width of the feature
.EP
.SP 9 5
height - the height of the feature
.EP
.SP 9 5
range - the distance (number of rooms) it can be seen for
.EP
.SP 9 5
descs - array of descriptions to be shown at different ranges.
These are divided equally along the total range
.EP
.SP 9 5
items - add_item name(s) (identical to the 'shorts' parameter in add_item)
.EP
.SP 9 5
item_desc - a string, or array of strings giving the corresponding
.EP
.SP 9 5
night_descs - the night description array
.EP
.SP 9 5
night_items - the descriptions for the items at night
.EP

.SI 3
* %^BOLD%^add_feature_ob%^RESET%^
.EI
.SI 5
void add_feature_ob(string title, object feature)
.EI
.SP 7 5

This method is called to add a new 'feature' to a terrain map.
Features have no real existance, but add atmosphere to a whole area,
by being visible across a large number of locations.
You define location, size, item desc, and add an array of 'longs'
sorted by distance, plus the usual item descriptions. Any occurrences
of $D in the longs array will be replaced by the direction in which
the feature lies.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the name of the feature
.EP
.SP 9 5
region - the region object to use in calculating the feature
.EP
.SP 9 5
range - the distance (number of rooms) it can be seen for
.EP
.SP 9 5
descs - array of descriptions to be shown at different ranges.
These are divided equally along the total range
.EP
.SP 9 5
items - add_item name(s) (identical to the 'shorts' parameter in add_item)
.EP
.SP 9 5
item_desc - a string, or array of strings giving the corresponding
.EP
.SP 9 5
whole_sentance - use a whole sentance for the descriptions or wrap
them up into fragments
.EP

.SI 3
* %^BOLD%^add_feature_region%^RESET%^
.EI
.SI 5
void add_feature_region(string title, object region, int range, string * descs, mixed items, mixed item_desc, string * night_descs, mixed night_items, int whole_sentance)
.EI
.SP 7 5

This method is called to add a new 'feature' to a terrain map.
Features have no real existance, but add atmosphere to a whole area,
by being visible across a large number of locations.
You define location, size, item desc, and add an array of 'longs'
sorted by distance, plus the usual item descriptions. Any occurrences
of $D in the longs array will be replaced by the direction in which
the feature lies.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the name of the feature
.EP
.SP 9 5
region - the region object to use in calculating the feature
.EP
.SP 9 5
range - the distance (number of rooms) it can be seen for
.EP
.SP 9 5
descs - array of descriptions to be shown at different ranges.
These are divided equally along the total range
.EP
.SP 9 5
items - add_item name(s) (identical to the 'shorts' parameter in add_item)
.EP
.SP 9 5
item_desc - a string, or array of strings giving the corresponding
.EP
.SP 9 5
whole_sentance - use a whole sentance for the descriptions or wrap
them up into fragments
.EP

.SI 3
* %^BOLD%^add_feature_sentance%^RESET%^
.EI
.SI 5
void add_feature_sentance(string title, int x, int y, int width, int height, int range, string * descs, mixed items, mixed item_desc, string * night_descs, mixed night_items)
.EI
.SP 7 5

This method is called to add a new 'feature' to a terrain map.
Features have no real existance, but add atmosphere to a whole area,
by being visible across a large number of locations.  This method is
different to the normal add_feature in that the system will try
and construct a sentance from the features instead of once sentance
per feature.
You define location, size, item desc, and add an array of 'longs'
sorted by distance, plus the usual item descriptions. Any occurrences
of $D in the longs array will be replaced by the direction in which
the feature lies.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the name of the feature
.EP
.SP 9 5
x - the x coordinate of the room (internal coordinate system)
.EP
.SP 9 5
y - the y coordinate of the room (internal coordinate system)
.EP
.SP 9 5
width - the width of the feature
.EP
.SP 9 5
height - the height of the feature
.EP
.SP 9 5
range - the distance (number of rooms) it can be seen for
.EP
.SP 9 5
descs - array of descriptions to be shown at different ranges.
.EP
.SP 9 5
items - add_item name(s) (identical to the 'shorts' parameter in add_item)
.EP
.SP 9 5
item_desc - a string, or array of strings giving the corresponding
These are divided equally along the total range
.EP

.SI 3
* %^BOLD%^add_obstacle_type%^RESET%^
.EI
.SI 5
varargs void add_obstacle_type(string type, string description, mixed items, mixed item_desc, string room_desc)
.EI
.SP 7 5

This method is used to add an 'obstacle' to an area map. An obstacle is
effectively an area on the map which cannot be entered, and can be seen
from adjacent locations. Any occurences of $D in the description will
be replaced with the direction(s) in which the obstacle can be seen.
An add_item is automatically performed to allow for consistency.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the ASCII type used on the map to denote the obstacle
.EP
.SP 9 5
description - the description of the obstacle, as seen from
neighbouring rooms
.EP
.SP 9 5
items - a string, or array of strings giving the corresponding
add_item name(s) (identical to the 'shorts' parameter in add_item)
.EP
.SP 9 5
item_desc - a string, or array of strings giving the corresponding
.EP
.SP 9 5
items - add_item name(s) (identical to the 'shorts' parameter in add_item)
.EP
.SP 9 5
item_desc - a string, or array of strings giving the corresponding item desc
.EP
.SP 9 5
room_desc - allows a description for the obstacle in the key
.EP

.SI 3
* %^BOLD%^add_random_chat%^RESET%^
.EI
.SI 5
void add_random_chat(string types, string * chats)
.EI
.SP 7 5

This method is to add to the list of random chats that can be added to
specific room types. A specific random sequence is used so that the randoms
won't vary over time.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
types - the string of ASCII character type(s) that this desc could apply to
.EP
.SP 9 5
chats - an array of chats to add. Usual add_chat format.
.EP

.SI 3
* %^BOLD%^add_random_desc%^RESET%^
.EI
.SI 5
void add_random_desc(string types, string description, mixed items, mixed item_desc)
.EI
.SP 7 5

This method is to add to the list of random descriptions that can be added to
specific room types. A specific random sequence is used so that the randoms
won't vary over time.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
types - the string of ASCII character type(s) that this desc could apply to
.EP
.SP 9 5
items - add_item name(s) (identical to the 'shorts' parameter in add_item)
.EP
.SP 9 5
item_desc - a string, or array of strings giving the corresponding item desc
.EP

.SI 3
* %^BOLD%^add_road_special_type%^RESET%^
.EI
.SI 5
varargs void add_road_special_type(string type, string base_path, int size, int road_skip, int height, string room_desc, string description, mixed items, mixed item_desc, mapping height_rooms)
.EI
.SP 7 5

This method is used to add a standard road type to the terrain map,
mapped to an ASCII character as used in the ASCII room map.
A road is a terrain map room which can be seen from adjacent locations.
The handler uses 'room_desc' to add an entry to the room description,
describing where the road goes. 'description' is added to the
descriptions of adjacent locations, to enable the road to be seen.
In 'room_desc' and 'description', $D will be expanded to the relevant
directions.
A skipped
room is one in which every nth room in the terrain will be skipped
unless it has something interesting in it.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the ASCII type used on the map to denote the room
.EP
.SP 9 5
base_path - the full pathname of the source file for the room
.EP
.SP 9 5
size - the number of rooms in each clump to skip over
.EP
.SP 9 5
road_skip - if this is 1 then the skip type is a 'road skip', this
will assume the path is one wide and try to follow around curves
.EP
.SP 9 5
height - the height of the room
.EP
.SP 9 5
room_desc - the 'continuation' description, which added to the room description to show where the road leads
.EP
.SP 9 5
description - the 'adjacent' description, which is seen from adjacent locations.
.EP
.SP 9 5
items - add_item name(s) (identical to the 'shorts' parameter in add_item)
.EP
.SP 9 5
item_desc - a string, or array of strings giving the corresponding item desc
.EP
.SP 9 5
height_rooms - is a mapping containing what rooms to use at different height levels
.EP

.SI 3
* %^BOLD%^add_road_type%^RESET%^
.EI
.SI 5
varargs void add_road_type(string type, string base_path, string room_desc, string description, mixed items, mixed item_desc)
.EI
.SP 7 5

This method is used to add a standard road type to the terrain map,
mapped to an ASCII character as used in the ASCII room map.
A road is a terrain map room which can be seen from adjacent locations.
The handler uses 'room_desc' to add an entry to the room description,
describing where the road goes. 'description' is added to the
descriptions of adjacent locations, to enable the road to be seen.
In 'room_desc' and 'description', $D will be expanded to the relevant
directions.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the ASCII type used on the map to denote the room
.EP
.SP 9 5
base_path - the full pathname of the source file for the room
.EP
.SP 9 5
room_desc - the 'continuation' description, which added to the room description to show where the road leads
.EP
.SP 9 5
description - the 'adjacent' description, which is seen from adjacent locations.
.EP
.SP 9 5
items - add_item name(s) (identical to the 'shorts' parameter in add_item)
.EP
.SP 9 5
item_desc - a string, or array of strings giving the corresponding item desc
.EP

.SI 3
* %^BOLD%^add_room_type%^RESET%^
.EI
.SI 5
void add_room_type(string type, string base_path)
.EI
.SP 7 5

This method is used to add a standard room type to the terrain map,
mapped to an ASCII character as used in the ASCII room map.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the ASCII type used on the map to denote the room
.EP
.SP 9 5
base_path - the full pathname of the source file for the room
.EP

.SI 3
* %^BOLD%^add_sign%^RESET%^
.EI
.SI 5
void add_sign(int x, int y, string sign_long, mixed sign_read_mess, string sign_short, mixed sign_name, string sign_language)
.EI
.SP 7 5

This method is used to add a signpost into a specified location in a
terrain_map. It is identical to the /std/room/ add_sign, except for
the x and y coordinates which specify where the sign should go.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the name of the feature
.EP
.SP 9 5
x - the x coordinate of the center point
.EP
.SP 9 5
y - the y coordinate of the center point
.EP

.SI 3
* %^BOLD%^add_special_type%^RESET%^
.EI
.SI 5
void add_special_type(string type, string base_path, int size, int road_skip, int height, mapping height_rooms)
.EI
.SP 7 5

This method is used to add a standard skip room type to the terrain map.
Mapped to an ASCII character as used in the ASCII room map.  A skipped
room is one in which every nth room in the terrain will be skipped
unless it has something interesting in it.  The height is used to
do specific sorts of checks, like if a specific direction is passable or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the ASCII type used on the map to denote the room
.EP
.SP 9 5
base_path - the full pathname of the source file for the room
.EP
.SP 9 5
size - the number of rooms in each clump to skip over
.EP
.SP 9 5
road_skip - if this is 1 then the skip type is a 'road skip', this
will assume the path is one wide and try to follow around curves
.EP
.SP 9 5
height - the height of the room
.EP
.SP 9 5
height_rooms - is a mapping containing what rooms to use at different height levels
.EP

.SI 3
* %^BOLD%^add_world_room_type%^RESET%^
.EI
.SI 5
varargs void add_world_room_type(string type, string base_path)
.EI
.SP 7 5

This method is used to add a world map based file into the terrain.
It handles all the options like a special road type, but queries all the
details off the base room instead.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the ASCII type used on the map to denote the room
.EP
.SP 9 5
base_path - the full pathname of the source file for the room
.EP

.SI 3
* %^BOLD%^add_z_exit_at%^RESET%^
.EI
.SI 5
varargs void add_z_exit_at(int x, int y, int z, string direc, string dest, string type, mixed modifiers)
.EI
.SP 7 5

This method is used to add a standard exit to a specified location
in a terrain map. The chief use is to insert exits to standard rooms
or other terrain maps.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x coordinate of the room (internal coordinate system)
.EP
.SP 9 5
y - the y coordinate of the room (internal coordinate system)
.EP
.SP 9 5
direc - the direction of the exit (as in add_exit)
.EP
.SP 9 5
dest - the path of the exit's destination (as in add_exit)
.EP
.SP 9 5
type - the type of exit (as in add_exit)
.EP
.SP 9 5
modifiers - an optional array of exit modifiers (as in modify_exit)
.EP

.SI 3
* %^BOLD%^add_zones%^RESET%^
.EI
.SI 5
void add_zones(string type, string * zone_names)
.EI
.SP 7 5

This method is used to map ASCII characters used in the zone map onto
one or more actual room zones.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the ASCII character used in the zone map
.EP
.SP 9 5
zones - a zone name or an array of zone names
.EP

.SI 3
* %^BOLD%^calc_features%^RESET%^
.EI
.SI 5
mapping calc_features(object room)
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the blocking data

.EP

.SI 3
* %^BOLD%^calc_random_chats%^RESET%^
.EI
.SI 5
void calc_random_chats(object room, string key)
.EI

.SI 3
* %^BOLD%^calc_random_descs%^RESET%^
.EI
.SI 5
void calc_random_descs(object room, string key)
.EI

.SI 3
* %^BOLD%^debug_adjacent_terrains%^RESET%^
.EI
.SI 5
string * debug_adjacent_terrains()
.EI

.SI 3
* %^BOLD%^find_adjacent_terrain%^RESET%^
.EI
.SI 5
string find_adjacent_terrain(int x, int y)
.EI
.SP 7 5

THis method finds a terrain adjacent to us from this location
in the specific direction.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x location
.EP
.SP 9 5
y - the y location

.EP

.SI 3
* %^BOLD%^find_all_adjacent_terrains%^RESET%^
.EI
.SI 5
void find_all_adjacent_terrains()
.EI
.SP 7 5

This is specifically for debug.  It finds al the adjacent terrains.

.EP

.SI 3
* %^BOLD%^find_next_room_from%^RESET%^
.EI
.SI 5
string * find_next_room_from(int x, int y, int z, string direc)
.EI
.SP 7 5

This method finds the next room in the path.  It returns the next
room as the first element in the array and the list of the directions
to get there as the second element.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the start x position
.EP
.SP 9 5
y - the start y position
.EP
.SP 9 5
z - the start z position
.EP
.SP 9 5
direc - the direction to go from here
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
({ the end location, directions... })

.EP

.SI 3
* %^BOLD%^find_room_at_real_coord%^RESET%^
.EI
.SI 5
string find_room_at_real_coord(int x, int y, int z)
.EI
.SP 7 5

This method finds the room at the specific real coordinate.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x location
.EP
.SP 9 5
y - the y location
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the room, or 0 on failure

.EP

.SI 3
* %^BOLD%^load_room_at%^RESET%^
.EI
.SI 5
object load_room_at(int x, int y, int z)
.EI
.SP 7 5

This method is used to find a room at the specified coordinates,
or to clone and setup a new one if none exists so far.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x coordinate of the room (internal coordinate system)
.EP
.SP 9 5
y - the y coordinate of the room (internal coordinate system)
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the room at those coordinates, or 0 if none can be cloned
.EP

.SI 3
* %^BOLD%^modify_feature%^RESET%^
.EI
.SI 5
varargs void modify_feature(string title, string * descs, mixed item_desc)
.EI
.SP 7 5

This method is used to modify the descriptions and/or item descs of
an established feature. Changes will be instantly seen in all the
rooms the feature is visible from.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the name of the feature
.EP
.SP 9 5
descs - array of descriptions to be shown at different ranges.
add_item name(s) (identical to the 'shorts' parameter in add_item)
.EP
.SP 9 5
item_desc - a string, or array of strings giving the corresponding
These are divided equally along the total range
.EP

.SI 3
* %^BOLD%^query_adjacent_terrains%^RESET%^
.EI
.SI 5
mapping query_adjacent_terrains()
.EI

.SI 3
* %^BOLD%^query_area_map%^RESET%^
.EI
.SI 5
mixed * query_area_map()
.EI

.SI 3
* %^BOLD%^query_base_room%^RESET%^
.EI
.SI 5
string query_base_room(int x, int y, int z, int map_room)
.EI
.SP 7 5

This returns the destination room in the given directoin, initially
it tries for a normal exit type.  If that fails, it tries for a climbing
exit type.  A climbing exit type is one that moves up or down from the
current z height.
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
.SP 9 5
base - the base path of the parent terrain handler

.EP

.SI 3
* %^BOLD%^query_char_map_real_coord%^RESET%^
.EI
.SI 5
class char_map_data query_char_map_real_coord(int x, int y)
.EI
.SP 7 5

This is used by the terrain system to work out what is beside
us.
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

.SI 3
* %^BOLD%^query_char_maps%^RESET%^
.EI
.SI 5
mapping query_char_maps()
.EI

.SI 3
* %^BOLD%^query_climb_base_room%^RESET%^
.EI
.SI 5
string query_climb_base_room(int x, int y, int z, int map_room, string ref base)
.EI
.SP 7 5

This method returns the basic room as the specified location.  A climb
room type means a room that is up or down on the z axis from where we
are, but we are allowed to climb to.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x location
.EP
.SP 9 5
y - the y location
.EP
.SP 9 5
z - the z location
.EP
.SP 9 5
map_room - check down through the air for the room for the map
.EP
.SP 9 5
base - the updated base (for climbing rooms)
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the basic room for the location

.EP

.SI 3
* %^BOLD%^query_debug_map%^RESET%^
.EI
.SI 5
string query_debug_map(int x, int y, int width, int showx, int showy)
.EI
.SP 7 5

This method returns a grid of the specified size around the center
point to give creators an overview of where they are currently.
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
width - the width of the box

.EP

.SI 3
* %^BOLD%^query_debug_map_char%^RESET%^
.EI
.SI 5
string query_debug_map_char(int x, int y)
.EI
.SP 7 5

This method returns the specific debug map character at the
given location.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x location
.EP
.SP 9 5
y - the y location
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the map character

.EP

.SI 3
* %^BOLD%^query_debug_map_feature%^RESET%^
.EI
.SI 5
string query_debug_map_feature(string name)
.EI
.SP 7 5

This method returns a grid of the specified size around the center
point to give creators an overview of where they are currently.
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
width - the width of the box

.EP

.SI 3
* %^BOLD%^query_debug_map_feature_distant%^RESET%^
.EI
.SI 5
string query_debug_map_feature_distant(string feature)
.EI
.SP 7 5

This method returns a grid of the specified size around the center
point to give creators an overview of where they are currently.
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
width - the width of the box

.EP

.SI 3
* %^BOLD%^query_debug_real_coord%^RESET%^
.EI
.SI 5
class real_coords query_debug_real_coord()
.EI

.SI 3
* %^BOLD%^query_dest_coordinate_string%^RESET%^
.EI
.SI 5
string query_dest_coordinate_string(int x, int y, int z, string direc, string key)
.EI
.SP 7 5

This method finds the co-ordinates to use for getting to the
next room.  THis handles messing about to make bridges and things
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
direc - the direction it is from
.EP
.SP 9 5
key - the key is it from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new mangled co-ordinates

.EP

.SI 3
* %^BOLD%^query_dimensions%^RESET%^
.EI
.SI 5
int * query_dimensions()
.EI

.SI 3
* %^BOLD%^query_direction_distance%^RESET%^
.EI
.SI 5
int query_direction_distance(string dir)
.EI
.SP 7 5

This method returns the distance that is moved if you go in the
specified direction.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the distance

.EP

.SI 3
* %^BOLD%^query_feature_item_desc%^RESET%^
.EI
.SI 5
varargs string query_feature_item_desc(string title)
.EI
.SP 7 5

This method returns the item description.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the title of the feature
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the feature item description

.EP

.SI 3
* %^BOLD%^query_feature_ob%^RESET%^
.EI
.SI 5
object query_feature_ob(string name)
.EI
.SP 7 5

This method returns the region used when calculating the extents of the
feature.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the feature to find the region of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the feature region

.EP

.SI 3
* %^BOLD%^query_feature_rooms%^RESET%^
.EI
.SI 5
mapping query_feature_rooms()
.EI

.SI 3
* %^BOLD%^query_features%^RESET%^
.EI
.SI 5
mapping query_features()
.EI

.SI 3
* %^BOLD%^query_ground_room%^RESET%^
.EI
.SI 5
string query_ground_room(int x, int y)
.EI
.SP 7 5

This method returns the location of the ground room.  It is basically
just the co-ordates connected onto the terrain_handler definition.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x location
.EP
.SP 9 5
y - the y location
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string name of the ground room

.EP

.SI 3
* %^BOLD%^query_ground_room_co_ords%^RESET%^
.EI
.SI 5
int * query_ground_room_co_ords(int x, int y)
.EI
.SP 7 5

This method returns the ground room for a specific co-ordinate.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x location
.EP
.SP 9 5
y - the y location
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the terrain co-ordinates of the ground room

.EP

.SI 3
* %^BOLD%^query_map_file%^RESET%^
.EI
.SI 5
string query_map_file()
.EI

.SI 3
* %^BOLD%^query_more_terrains_rooms_from%^RESET%^
.EI
.SI 5
mixed * query_more_terrains_rooms_from(int x, int y, int z, string direc, int moves_left, int jump, string main_key, int road_jump)
.EI
.SP 7 5

This method is used by connecting terrains to find the next room in
sequence.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the real x coordinate
.EP
.SP 9 5
y - the real y coordinate
.EP
.SP 9 5
moves_left - the number of moves left to pop along
.EP
.SP 9 5
jump - the jump length we are looking for
.EP
.SP 9 5
main_key - the key, this is only used for non-road jumps
.EP
.SP 9 5
road_jump - is this a road jump

.EP

.SI 3
* %^BOLD%^query_player_map%^RESET%^
.EI
.SI 5
string query_player_map(int x_c, int y_c, int z_c, int visibility)
.EI
.SP 7 5

This method returns the player line of sight view of the current terrain.
It depends on height and other wonderful things.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x_c - the x center point
.EP
.SP 9 5
y_c - the y center point
.EP
.SP 9 5
z_c - the z center point
.EP
.SP 9 5
visibility - the visibility percentage currently

.EP

.SI 3
* %^BOLD%^query_player_map_internal_template%^RESET%^
.EI
.SI 5
mapping query_player_map_internal_template(mixed * map, mapping key, mixed * colours, int x_c, int y_c, int z_c, int distance)
.EI
.SP 7 5

This method does the updating of the map based on the template.
Pretty easy really, check each location in the map to see if it
exists and what we should do with it.

.EP

.SI 3
* %^BOLD%^query_player_map_template%^RESET%^
.EI
.SI 5
string query_player_map_template(int x_c, int y_c, int z_c, int visibility, int max_size)
.EI
.SP 7 5

This method generates a player map using templates instead of using
lines from the center point.  This is more efficent than a
line drawing method.  THe max_size is used to
make sure the size of the visible area is squashed, if we want a
smaller partial map for other displays.  The max size must be
an odd number.

.EP

.SI 3
* %^BOLD%^query_random_chats%^RESET%^
.EI
.SI 5
mapping query_random_chats()
.EI

.SI 3
* %^BOLD%^query_random_descs%^RESET%^
.EI
.SI 5
mapping query_random_descs()
.EI

.SI 3
* %^BOLD%^query_real_coords%^RESET%^
.EI
.SI 5
mixed * query_real_coords()
.EI

.SI 3
* %^BOLD%^query_real_from_terrain_coord%^RESET%^
.EI
.SI 5
class coord query_real_from_terrain_coord(int x, int y, int z)
.EI
.SP 7 5

This method returns the real coordinates from the terrain coordinates.
For simplicity we always assume the terrain sits at the water level.
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

.SI 3
* %^BOLD%^query_room%^RESET%^
.EI
.SI 5
mixed query_room(int x, int y, int z)
.EI
.SP 7 5

This method queries the path of a room. If it takes the form:
'/d/full/path/name.c:x:y' where x and y are integers, it clones
a new room at the specified coordinates, sets it up fully, and returns it.
If the room path is of the standard form, it simply returns the loaded
room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room_path - the full pathname of the room to be cloned.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the cloned/loaded room, or 0 if it could not be cloned/loaded
.EP

.SI 3
* %^BOLD%^query_room_at%^RESET%^
.EI
.SI 5
object query_room_at(int x, int y, int z)
.EI
.SP 7 5

This method is used to find a room at the specified coordinates,
if one has already been cloned.
If a room has been cloned at those coordinates, it is returned,
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x coordinate of the room (internal coordinate system)
.EP
.SP 9 5
y - the y coordinate of the room (internal coordinate system)
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the room at those coordinates, or 0 if none has been cloned
.EP

.SI 3
* %^BOLD%^query_room_map%^RESET%^
.EI
.SI 5
mixed * query_room_map()
.EI

.SI 3
* %^BOLD%^query_room_zones%^RESET%^
.EI
.SI 5
mapping query_room_zones()
.EI

.SI 3
* %^BOLD%^query_rooms_in_block%^RESET%^
.EI
.SI 5
object * query_rooms_in_block(int x, int y, int z, int width, int height)
.EI
.SP 7 5

This method is used to find all the rooms currently cloned within a
specified rectangular area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x coordinate of rectangle's bottom left corner
.EP
.SP 9 5
y - the y coordinate of rectangle's bottom left corner
.EP
.SP 9 5
width - the width of the rectangle
.EP
.SP 9 5
height - the height of the rectangle
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array containing any rooms currently cloned within the
specified rectangle
.EP

.SI 3
* %^BOLD%^query_rooms_in_range%^RESET%^
.EI
.SI 5
varargs object * query_rooms_in_range(int x, int y, int z, int max, int min)
.EI
.SP 7 5

This method is used to find all the rooms currently cloned within a
specified distance of the specified coordinates.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x coordinate of the center point
.EP
.SP 9 5
y - the y coordinate of the center point
.EP
.SP 9 5
max - the maximum distance of rooms to be returned
.EP
.SP 9 5
min - the minimum distance of rooms to be returned. Defaults to 0
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array containing any rooms currently cloned within the
specified range
.EP

.SI 3
* %^BOLD%^query_rooms_in_zone%^RESET%^
.EI
.SI 5
object * query_rooms_in_zone(string zone)
.EI
.SP 7 5

This method is used to find all rooms currently loaded within the
specified room zone.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
zone - the name of the room zone
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array containing all the currently cloned rooms within this zone
.EP

.SI 3
* %^BOLD%^query_rooms_to%^RESET%^
.EI
.SI 5
mixed * query_rooms_to(int x, int y, int z, string direc, int len, string main_key, int jump_len, int road_type)
.EI
.SP 7 5

This method find the specific squence of rooms in the given direction
with the given room jump.  We do not worry about contents here.  Only if
it is a special interest marker or if there are two different ways from
the location of the same type.  Items in the map are set as being
special interest to stop a jump.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x co-ordinate to start from
.EP
.SP 9 5
y - the y co-ordinate to start from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of rooms

.EP

.SI 3
* %^BOLD%^query_signposts%^RESET%^
.EI
.SI 5
mapping query_signposts()
.EI

.SI 3
* %^BOLD%^query_standard_exits%^RESET%^
.EI
.SI 5
mapping query_standard_exits()
.EI

.SI 3
* %^BOLD%^query_terrain_from_real_coord%^RESET%^
.EI
.SI 5
class coord query_terrain_from_real_coord(int x, int y, int z)
.EI
.SP 7 5

This method returns the room coordinates from the real coordinates.
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

.SI 3
* %^BOLD%^query_visible_distance%^RESET%^
.EI
.SI 5
int query_visible_distance()
.EI
.SP 7 5

This method returns the distance that you can see in this terrain.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the visible distance in the terrain

.EP

.SI 3
* %^BOLD%^query_zone_map%^RESET%^
.EI
.SI 5
mixed * query_zone_map()
.EI

.SI 3
* %^BOLD%^query_zone_types%^RESET%^
.EI
.SI 5
mapping query_zone_types()
.EI

.SI 3
* %^BOLD%^remove_feature%^RESET%^
.EI
.SI 5
void remove_feature(string title)
.EI
.SP 7 5

This method is used to remove a named feature from the terrain map
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the name of the feature
.EP

.SI 3
* %^BOLD%^set_external_chats%^RESET%^
.EI
.SI 5
void set_external_chats(string type, mixed chats)
.EI
.SP 7 5

This method is used to setup 'external' chats for a road or obstacle,
which will be added to the room chats of adjacent locations. You
should supply the ASCII character corresponding to the road/obstacle
type, and an array of chat strings (in the same format as supplied to
room_chat)
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the ASCII type used on the map to denote the room
.EP
.SP 9 5
base_path - the full pathname of the source file for the room
   set_external_chats("=",
                ({ "The stream bubbles merrily on its way.",
                   "You hear a faint splashing sound from the stream." }));
.EP

.SI 3
* %^BOLD%^set_feature_chats%^RESET%^
.EI
.SI 5
void set_feature_chats(string title, mixed * chats, mixed * night_chats)
.EI
.SP 7 5

This method is used to add room chats to locations that can view a
particular feature. $D substitution is carried out on the chats.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the name of the feature
.EP
.SP 9 5
chats - an array of strings (room chats related to the feature)
.EP

.SI 3
* %^BOLD%^set_newline_mode%^RESET%^
.EI
.SI 5
void set_newline_mode(int mode)
.EI
.SP 7 5

This method sets the 'newline' mode for room description extras.
If set to 1, a newline will be inserted after all feature and
neighbouring room items. 1 is the default.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mode - the new mode.

.EP

.SI 3
* %^BOLD%^set_random_chat_limit%^RESET%^
.EI
.SI 5
void set_random_chat_limit(int new_limit)
.EI
.SP 7 5

This method is used to limit the number of random chats that are
picked out for each room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_limit - the new chat limit
.EP

.SI 3
* %^BOLD%^set_random_desc_limit%^RESET%^
.EI
.SI 5
void set_random_desc_limit(int new_limit)
.EI
.SP 7 5

This method is used to limit the number of random descs that are
picked out for each room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_limit - the new desc limit
.EP

.SI 3
* %^BOLD%^set_real_coordinates%^RESET%^
.EI
.SI 5
void set_real_coordinates(int x1, int y1, int x2, int y2)
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x1 - the start x location
.EP
.SP 9 5
y1 - the start y location
.EP
.SP 9 5
x1 - the end x location
.EP
.SP 9 5
y1 - the end y location

.EP

.SI 3
* %^BOLD%^set_real_coordinates_no_inform%^RESET%^
.EI
.SI 5
void set_real_coordinates_no_inform(int x1, int y1, int x2, int y2)
.EI
.SP 7 5

This method sets the real coordinates for the room without informing
the map handler.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x1 - the start x location
.EP
.SP 9 5
y1 - the start y location
.EP
.SP 9 5
x1 - the end x location
.EP
.SP 9 5
y1 - the end y location

.EP

.SI 3
* %^BOLD%^set_real_offsets%^RESET%^
.EI
.SI 5
void set_real_offsets(int xoff, int yoff, int zoff)
.EI
.SP 7 5

This method sets the real offsets for movement in the rooms.  This
is needed in rooms that do not have real co-ordinates set but still
require movement to generate the correct messages.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
xoff - how far you move in the x direction
.EP
.SP 9 5
yoff - how far you move in the y direction
.EP
.SP 9 5
zoff - how far you move in the z direction

.EP

.SI 3
* %^BOLD%^set_visible_distance%^RESET%^
.EI
.SI 5
void set_visible_distance(int distance)
.EI
.SP 7 5

This method sets the distance you can see in this terrain based on the
map.  The distance is used to determine nice little map things to show
the players.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
distance - the distance you can see

.EP

.SI 3
* %^BOLD%^setup_handler%^RESET%^
.EI
.SI 5
void setup_handler()
.EI

.SI 3
* %^BOLD%^setup_room%^RESET%^
.EI
.SI 5
void setup_room(object room, string base_path)
.EI
.SP 7 5

The main nasty func for setting up a room. It adds all the necessary exits,
and adds to the room's extra_look & add_items for anything outside the room
which should be visible. It's fairly nasty. But hey - it works.

.EP

.SI 3
* %^BOLD%^tell_feature%^RESET%^
.EI
.SI 5
varargs void tell_feature(string title, mixed message)
.EI
.SP 7 5

This message displays messages in all rooms within range of a feature.
The message can be either a string, which goes to all the rooms, or an
array of strings, in which case they get spread across the distances (in
which case you should supply the same number of array elements as in the
descriptions in the original add_feature. Any occurences of $D in the
message(s) will be replaced in each room with the direction that the
feature lies in.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
title - the name of the feature, as used in add_feature
.EP
.SP 9 5
message - the message to be displayed
.EP

.SI 3
* %^BOLD%^tell_zones%^RESET%^
.EI
.SI 5
varargs void tell_zones(mixed zones, string message, mixed mess_class)
.EI
.SP 7 5

This method displays a message in all the currently loaded rooms
within the specified zone(s).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
zones - a zone name as a string, or an array of zone names
.EP
.SP 9 5
message - the message to be displayed
.EP
.SP 9 5
mess_class - the message type. Normally this need not be specified
.EP


