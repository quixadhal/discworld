.DT
terrain_map.h
Disk World autodoc help
terrain_map.h

.SH Description
.SP 5 5

The include file for the terrain map system.
.EP
.SP 10 5


Written by Dek

.EP

.SH Classes

.SI 3
* %^BOLD%^coord%^RESET%^
class coord {
int x;
int y;
int z;
}

.EI
.SP 7 5

The class with the coordinate definitions in it.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
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


.SH Defines
.SI 3
* %^BOLD%^TERRAIN_MAP%^RESET%^
.EI
.SP 7 5

The location of the terrain map files.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_AIR_BASE%^RESET%^
.EI
.SP 7 5

The base for for air rooms.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_BLOCKING_FLAG%^RESET%^
.EI
.SP 7 5

This is the flag to use for making the feature blocking.  If a feature
is blocking then you cannot see other features behind it.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_DEFAULT_AIR_ROOM%^RESET%^
.EI
.SP 7 5

This is the air room to use if nothing else is specfied.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_DISTANCE_BASE%^RESET%^
.EI
.SP 7 5

The define for the inherit for the distance stuff.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_FACTORY%^RESET%^
.EI
.SP 7 5

The define for the inherit for the factory type of an inherit.
THis is used in the case where you want to define something about
a group of shared rooms (shared between other factories) and yet
want to customise some specific part about they work.  For
example see the seas.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_FACTORY_BASE%^RESET%^
.EI
.SP 7 5

The base to use for factories.  Do not include this into
production code.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_FEATURE_BASE%^RESET%^
.EI
.SP 7 5

The define for the inherit for the feature base.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_FEATURE_LOCAL%^RESET%^
.EI
.SP 7 5

The define for the inherit for the local feature base.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_GRID_SIZE%^RESET%^
.EI
.SP 7 5

Smallest terrain we will deal with in the terrain system.
(7 miles).

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_HALF_MILE%^RESET%^
.EI
.SP 7 5

This is the size
of one mile in co-ordinates.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_HANDLER_BASE%^RESET%^
.EI
.SP 7 5

The define for the inherit for the terrain handler.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_IN_LOOK_PROP%^RESET%^
.EI
.SP 7 5

This is the property to check on the player to see if the map should
be displayed in the long or not.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_INSIDE_BASE%^RESET%^
.EI
.SP 7 5

The base file for inside areas.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_JOURNEY_EXIT%^RESET%^
.EI
.SP 7 5

This is the start bit for a 'journey' exit.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_LONG_JUMP_PROPERTY%^RESET%^
.EI
.SP 7 5

This is the property to check on the player to see if you should jump
or not.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_ONE_MILE%^RESET%^
.EI
.SP 7 5

This is the size
of one mile in co-ordinates.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_OUTSIDE_BASE%^RESET%^
.EI
.SP 7 5

The base file for outside areas.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_REGION_LINE%^RESET%^
.EI
.SP 7 5

The define for the region class for line regions.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_REGION_POLYGON%^RESET%^
.EI
.SP 7 5

The define for the region class for polygon regions.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_REGION_RECTANGULAR%^RESET%^
.EI
.SP 7 5

The define for the region class for rectangular regions.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_ROOM_BASE%^RESET%^
.EI
.SP 7 5

The define for the inherit for the basic room functionality.  This
will need to be inherited into any room used in the terrain.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_ROOM_CLIMB%^RESET%^
.EI
.SP 7 5

This is the type to use when doing a find for a possible climbing
room.  A climbing room is a room which goes up or down from the
current height location.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_ROOM_EXACT%^RESET%^
.EI
.SP 7 5

This is the type to use when doing a find for the exactly room.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_ROOM_MAP%^RESET%^
.EI
.SP 7 5

This is the type to use when doing a find for a room to use for
the map.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_SIMPLE_FEATURE_BASE%^RESET%^
.EI
.SP 7 5

The define for the inherit for the simple feature base.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WALK_EXIT%^RESET%^
.EI
.SP 7 5

This is the start bit for a 'walk' exit.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WATER_INSIDE_BASE%^RESET%^
.EI
.SP 7 5

The base file for inside water areas.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WATER_OUTSIDE_BASE%^RESET%^
.EI
.SP 7 5

The base file for outside water areas.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WHOLE_SENTANCE_FLAG%^RESET%^
.EI
.SP 7 5

This is the flag to use for making the feature use whole sentances.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WORLD_BOUNDARY%^RESET%^
.EI
.SP 7 5

This define is used by the world map to determine where to put
areas in the world.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WORLD_HIGH_X%^RESET%^
.EI
.SP 7 5

This is the highest x coordinate of the world.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WORLD_HIGH_Y%^RESET%^
.EI
.SP 7 5

This is the highest y coordinate of the world.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WORLD_LOW_X%^RESET%^
.EI
.SP 7 5

This is the lowest x coordinate of the world.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WORLD_LOW_Y%^RESET%^
.EI
.SP 7 5

This is the lowest y coordinate of the world.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WORLD_MAP%^RESET%^
.EI
.SP 7 5

The location of the world map controller.  Any terrain map that has
a real co-ordinate registers itself with the map controller.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WORLD_OFFSET%^RESET%^
.EI
.SP 7 5

This define is used by the world map to detmine the offset value to
co-ordinates.  This makes sure all the region values are positive.
You add the offset onto the co-ordinate and divide by the
boundary to get the area.  You then divide by the region size to get
the region.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WORLD_PIXEL_WIDTH%^RESET%^
.EI
.SP 7 5

This is the size of the world in pixels.  There are 4 pixels to a terrain
map, so 4 to an area.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WORLD_REGION_BOUNDARY%^RESET%^
.EI
.SP 7 5

This define is used by the world map to detmine the boundary of a
region.  You add the offset onto the co-ordinate and divide by the
boundary to get the area.  You then divide by the region size to get
the region.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WORLD_REGION_SIZE%^RESET%^
.EI
.SP 7 5

This define is used by the world map to determine how many areas are
in each region.  An area is each terrain map square, if you look
at the co-ordinates in the terrain handlers it fits to these values.

.EP

.SI 3
* %^BOLD%^TERRAIN_MAP_WORLD_REGION_WIDTH%^RESET%^
.EI
.SP 7 5

This is the width of the region in areas.

.EP

