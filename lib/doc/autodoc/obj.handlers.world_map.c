.DT
world_map.c
Disk World autodoc help
world_map.c

.SH Description
.SP 5 5

This is the handler to help control the world map.  It co-ordinates
all the terrains connecting themselves together.  All terrains are
rectangular.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Mar 22 16:58:35 PST 2002

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/terrain_map.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^region%^RESET%^
class region {
mixed * terrains;
string * features;
int last_touched;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_feature%^RESET%^
.EI
.SI 5
void add_feature(string feature)
.EI
.SP 7 5

This method adds a feature into the world map.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
feature - the feature to add

.EP

.SI 3
* %^BOLD%^add_terrain%^RESET%^
.EI
.SI 5
int add_terrain(string path, int x1, int y1, int x2, int y2)
.EI
.SP 7 5

This method adds the specified terrain to the world map.

.EP

.SI 3
* %^BOLD%^find_room_at%^RESET%^
.EI
.SI 5
string find_room_at(int x, int y)
.EI
.SP 7 5

This method finds the specific room at the specific location
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
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the path to the room

.EP

.SI 3
* %^BOLD%^find_terrain_at%^RESET%^
.EI
.SI 5
string find_terrain_at(int x, int y)
.EI
.SP 7 5

This method finds the terrain at the specific location.
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
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the path to the terrain

.EP

.SI 3
* %^BOLD%^generate_blocking%^RESET%^
.EI
.SI 5
mixed * generate_blocking(int x, int y, string * start, string * straight, string * diagonal)
.EI

.SI 3
* %^BOLD%^query_blocking_template%^RESET%^
.EI
.SI 5
string * query_blocking_template(int x, int y, int type)
.EI
.SP 7 5

Find the template to use for the area blocked by an obstacle.
This will do nifty template fitting stuff for obstacles.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x - the x co-ordinate of the blockage
.EP
.SP 9 5
y - the y co-ordinate of the blockage
.EP
.SP 9 5
type - the type is 0 for normal, 1 for straight, 2 for diagonal

.EP

.SI 3
* %^BOLD%^query_features_at%^RESET%^
.EI
.SI 5
string * query_features_at(int x, int y)
.EI
.SP 7 5

This method returns the list of all the features available at the
specific location.
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
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the features at the location

.EP

.SI 3
* %^BOLD%^query_features_in_region%^RESET%^
.EI
.SI 5
string * query_features_in_region(int x1_orig, int y1_orig, int x2_orig, int y2_orig)
.EI
.SP 7 5

This method finds all the features inside the specified region.  This
is quite expensive in terms of evaluation cost, so do not try and call
this too much.  Cache the results if possible.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
x1 - the top x location
.EP
.SP 9 5
y1 - the top y location
.EP
.SP 9 5
x2 - the bottom x location
.EP
.SP 9 5
y2 - the bottom y location
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the features in the region

.EP

.SI 3
* %^BOLD%^query_map_template%^RESET%^
.EI
.SI 5
mixed * query_map_template(int size)
.EI
.SP 7 5

This method returns the template for the specific size of the
map.  This is for visibility considerations.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
size - the size of the template
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the template

.EP

.SI 3
* %^BOLD%^remove_feature%^RESET%^
.EI
.SI 5
void remove_feature(string feature)
.EI
.SP 7 5

This method removes a feature from the world map.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
feature - the feature to remove

.EP


