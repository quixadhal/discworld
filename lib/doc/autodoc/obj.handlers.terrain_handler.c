.DT
terrain_handler.c
Disk World autodoc help
terrain_handler.c

.SH Description
.SP 5 5

This is the terrain handler method documentation. For a more general
introduction to the terrain handler, please see the help for terrains.

.EP
.SP 10 5


Written by Sin, Turrican and others...

.EP



.SH See also
.SP 5 5
help::terrains

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/terrain.h, /include/map.h and /include/dirs.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_fixed_location%^RESET%^
.EI
.SI 5
int add_fixed_location(string terrain, string file, int * co_ords)
.EI
.SP 7 5

This method adds a new fixed location for a terrain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
terrain - the terrain name
.EP
.SP 9 5
file - the file name
.EP
.SP 9 5
co_ords - the coordinates for the location
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it succeeds, 0 if it fails (wrong coordinates or location
already present)
.EP

.SI 3
* %^BOLD%^add_floating_location%^RESET%^
.EI
.SI 5
int add_floating_location(string terrain, string file, int * co_ords, int level)
.EI
.SP 7 5

This method adds a new floating location for a terrain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
terrain - the terrain name
.EP
.SP 9 5
file - the file name
.EP
.SP 9 5
co_ords - the coordinates for the location (either a single coordinate
or 2 forming a bounding rectangle)
.EP
.SP 9 5
level - the level of this location
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it succeeds, 0 if it fails (wrong coordinates or location
already present)
.EP

.SI 3
* %^BOLD%^clear_cloned_locations%^RESET%^
.EI
.SI 5
void clear_cloned_locations(string terrain)
.EI
.SP 7 5

This method clears the cloned locations cache for a given terrain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
terrain - the terrain name
.EP

.SI 3
* %^BOLD%^clear_connections%^RESET%^
.EI
.SI 5
void clear_connections(string terrain)
.EI
.SP 7 5

This method clears all connections for a given terrain. This needs to
be done when a terrain was modified.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
terrain - the terrain name
.EP

.SI 3
* %^BOLD%^delete_cloned_location%^RESET%^
.EI
.SI 5
int delete_cloned_location(string terrain, string file)
.EI
.SP 7 5

This method deletes a cloned location for a terrain from the cache.
It is meant to be called when a cloned location is destructed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
terrain - the terrain name
.EP
.SP 9 5
file - the file name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it succeeds, 0 if it fails (location not present)
.EP

.SI 3
* %^BOLD%^delete_fixed_location%^RESET%^
.EI
.SI 5
int delete_fixed_location(string terrain, string file)
.EI
.SP 7 5

This method deletes a fixed location for a terrain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
terrain - the terrain name
.EP
.SP 9 5
file - the file name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it succeeds, 0 if it fails (location not present)
.EP

.SI 3
* %^BOLD%^delete_floating_location%^RESET%^
.EI
.SI 5
int delete_floating_location(string terrain, string file, int * co_ords)
.EI
.SP 7 5

This method deletes a floating location for a terrain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
terrain - the terrain name
.EP
.SP 9 5
file - the file name
.EP
.SP 9 5
co_ords - the coordinates for the location (either a single coordinate
or 2 forming a bounding rectangle)
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it succeeds, 0 if it fails (location not present)
.EP

.SI 3
* %^BOLD%^find_location%^RESET%^
.EI
.SI 5
object find_location(string terrain, int * co_ords)
.EI
.SP 7 5

This method is the main entry point for the terrain handler. It loads
the room that is associated with the given terrain and coordinates.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
terrain - the terrain name
.EP
.SP 9 5
co_ords - the coordinates to look up
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the room that was found or loaded
.EP

.SI 3
* %^BOLD%^get_data_file%^RESET%^
.EI
.SI 5
int get_data_file(string word)
.EI
.SP 7 5

This method loads the data file for a given terrain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the terrain to load the data for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the terrain exists, 0 if it didn't

.EP

.SI 3
* %^BOLD%^get_room_size%^RESET%^
.EI
.SI 5
int get_room_size(string file, int level)
.EI
.SP 7 5

This method returns the room size for a given room. It uses a cache
to speed things up.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
file - the file name of the room
.EP
.SP 9 5
level - the recursion level
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the size of the room

.EP

.SI 3
* %^BOLD%^member_cloned_locations%^RESET%^
.EI
.SI 5
string member_cloned_locations(int * co_ords)
.EI
.SP 7 5

This method checks wether there is a cloned location which matches the
given coordinates.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
co_ords - the coordinates to find the room for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the file name of the room with the given coordinates, or 0 if
it is not found
.EP

.SI 3
* %^BOLD%^member_fixed_locations%^RESET%^
.EI
.SI 5
string member_fixed_locations(int * co_ords)
.EI
.SP 7 5

This method checks wether there is a fixed location which matches the
given coordinates.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
co_ords - the coordinates to find the room for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the file name of the room with the given coordinates, or 0 if
it is not found
.EP

.SI 3
* %^BOLD%^member_floating_locations%^RESET%^
.EI
.SI 5
mixed * member_floating_locations(int * co_ords)
.EI
.SP 7 5

This method checks wether there are floating locations which match the
given coordinates.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
co_ords - the coordinates to find the rooms for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array with the file names of the rooms and the levels, or
the empty array if none are found
.EP

.SI 3
* %^BOLD%^modify_fixed_location%^RESET%^
.EI
.SI 5
int modify_fixed_location(string terrain, string file, int * co_ords)
.EI
.SP 7 5

This method modifies an existing fixed location for a terrain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
terrain - the terrain name
.EP
.SP 9 5
file - the file name
.EP
.SP 9 5
co_ords - the coordinates for the location
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it succeeds, 0 if it fails (wrong coordinates or location
not found)
.EP

.SI 3
* %^BOLD%^query_cloned_locations%^RESET%^
.EI
.SI 5
mapping query_cloned_locations(string terrain)
.EI
.SP 7 5

This method returns the cloned_locations mapping for a given terrain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the terrain name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the cloned_locations mapping; keys are the file names, values are
the coordinates for the files. There are also keys on x coordinates to
facilitate faster coordinate lookups.
.EP

.SI 3
* %^BOLD%^query_co_ord%^RESET%^
.EI
.SI 5
int * query_co_ord(string terrain, string file)
.EI
.SP 7 5

This method returns the coordinates for a given terrain and fixed location
filename.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
terrain - the terrain name
.EP
.SP 9 5
file - the file name of the location
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the coordinates

.EP

.SI 3
* %^BOLD%^query_connected%^RESET%^
.EI
.SI 5
int query_connected(string terrain, int * co_ords)
.EI
.SP 7 5

This method returns wether the given coordinates is connected to another
room in the terrain handler system.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
terrain - the terrain name
.EP
.SP 9 5
co_ords - the coordinates
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is found, 0 otherwise
.EP

.SI 3
* %^BOLD%^query_connection%^RESET%^
.EI
.SI 5
string query_connection(string terrain, int * co_ords, string direc)
.EI
.SP 7 5

This method returns the connecting room for the given coordinates and
direction.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
terrain - the terrain name
.EP
.SP 9 5
co_ords - the coordinates
.EP
.SP 9 5
direc - the direction to find the room for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the file name of the connecting room or 0 if none was found
.EP

.SI 3
* %^BOLD%^query_fixed_locations%^RESET%^
.EI
.SI 5
mapping query_fixed_locations(string word)
.EI
.SP 7 5

This method returns the fixed_locations mapping for the given terrain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the terrain name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the fixed_locations mapping; keys are the file names, values are
the coordinates for the files
.EP

.SI 3
* %^BOLD%^query_floating_locations%^RESET%^
.EI
.SI 5
mixed * query_floating_locations(string word)
.EI
.SP 7 5

This method returns the floating_locations array for the given terrain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the terrain name
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the floating_locations array; this is an array of arrays where each
array consists of the file name, an array of 6 coordinates forming a
bounding box or normal coordinates for the location and the level of the
floating location
.EP

.SI 3
* %^BOLD%^setup_location%^RESET%^
.EI
.SI 5
void setup_location(object place, string terrain)
.EI
.SP 7 5

This method is called when a normal room is loaded which is part of
a terrain. The terrain handler needs to know about this to be able
to calculate the exits into the floating portion of the terrain.
It also sets the coordinates of the room to the values as saved in the
terrain.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
place - the room object which is a fixed location in the terrain
.EP
.SP 9 5
terrain - the terrain name
.EP

.SI 3
* %^BOLD%^top_floating_location%^RESET%^
.EI
.SI 5
string top_floating_location(int * co_ords)
.EI
.SP 7 5

This method searches the top level floating location for the given
coordinates.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
co_ords - the coordinates to find the top level for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the file name of the top level room with the given coordinates,
or 0 if none was found
.EP


