/**
 * The include file for the terrain map system.
 * @author Dek
 */
#ifndef __TERRAIN_MAP_H
/** @ignore yes */
#define __TERRAIN_MAP_H

/**
 * The location of the world map controller.  Any terrain map that has
 * a real co-ordinate registers itself with the map controller.
 */
#define TERRAIN_MAP_WORLD_MAP "/obj/handlers/world_map"

/**
 * The location of the terrain map files.
 */
#define TERRAIN_MAP              "/std/room/inherit/terrain_map/"

/**
 * The define for the inherit for the local feature base.
 */
#define TERRAIN_MAP_FEATURE_LOCAL  TERRAIN_MAP + "feature_local"
/**
 * The define for the inherit for the simple feature base.
 */
#define TERRAIN_MAP_SIMPLE_FEATURE_BASE  TERRAIN_MAP + "simple_feature"
/**
 * The define for the inherit for the feature base.
 */
#define TERRAIN_MAP_FEATURE_BASE  TERRAIN_MAP + "feature_base"
/**
 * The define for the inherit for the distance stuff.
 */
#define TERRAIN_MAP_DISTANCE_BASE  TERRAIN_MAP + "distance_str"

/**
 * The define for the region class for rectangular regions.
 */
#define TERRAIN_MAP_REGION_RECTANGULAR  TERRAIN_MAP + "region_rect"
/**
 * The define for the region class for polygon regions.
 */
#define TERRAIN_MAP_REGION_POLYGON  TERRAIN_MAP + "region_poly"
/**
 * The define for the region class for line regions.
 */
#define TERRAIN_MAP_REGION_LINE  TERRAIN_MAP + "region_line"

/**
 * The define for the inherit for the terrain handler.
 */
#define TERRAIN_MAP_HANDLER_BASE  TERRAIN_MAP + "handler_base.c"
/**
 * The define for the inherit for the factory type of an inherit.
 * THis is used in the case where you want to define something about
 * a group of shared rooms (shared between other factories) and yet
 * want to customise some specific part about they work.  For
 * example see the seas.
 */
#define TERRAIN_MAP_FACTORY     TERRAIN_MAP + "factory.c"
/**
 * The base to use for factories.  Do not include this into
 * production code.
 */
#define TERRAIN_MAP_FACTORY_BASE     TERRAIN_MAP + "factory_base.c"
/**
 * The define for the inherit for the basic room functionality.  This
 * will need to be inherited into any room used in the terrain.
 */
#define TERRAIN_MAP_ROOM_BASE     TERRAIN_MAP + "terrain_room.c"
/**
 * The base file for outside areas.
 */
#define TERRAIN_MAP_OUTSIDE_BASE  TERRAIN_MAP + "outside_base.c"
/**
 * The base file for outside water areas.
 */
#define TERRAIN_MAP_WATER_OUTSIDE_BASE  TERRAIN_MAP + "water_outside_base.c"
/**
 * The base file for inside water areas.
 */
#define TERRAIN_MAP_WATER_INSIDE_BASE  TERRAIN_MAP + "water_inside_base.c"
/**
 * The base file for inside areas.
 */
#define TERRAIN_MAP_INSIDE_BASE   TERRAIN_MAP + "inside_base.c"
/**
 * The base for for air rooms.
 */
#define TERRAIN_MAP_AIR_BASE TERRAIN_MAP + "air_base.c"

/**
 * This is the size
 * of one mile in co-ordinates.
 */
#define TERRAIN_MAP_ONE_MILE 73920
/**
 * This is the size
 * of one mile in co-ordinates.
 */
#define TERRAIN_MAP_HALF_MILE 36960

/**
 * Smallest terrain we will deal with in the terrain system.
 * (7 miles).
 */
#define TERRAIN_MAP_GRID_SIZE (7 * TERRAIN_MAP_ONE_MILE)

/**
 * This is the property to check on the player to see if the map should
 * be displayed in the long or not.
 */
#define TERRAIN_MAP_IN_LOOK_PROP "terrain map look"
/**
 * This is the property to check on the player to see if you should jump
 * or not.
 */
#define TERRAIN_MAP_LONG_JUMP_PROPERTY "terrain map jump"

/**
 * This is the flag to use for making the feature use whole sentances.
 */
#define TERRAIN_MAP_WHOLE_SENTANCE_FLAG 1
/**
 * This is the flag to use for making the feature blocking.  If a feature
 * is blocking then you cannot see other features behind it.
 */
#define TERRAIN_MAP_BLOCKING_FLAG       2

/**
 * This is the start bit for a 'journey' exit.
 */
#define TERRAIN_MAP_JOURNEY_EXIT "journey "
/**
 * This is the start bit for a 'walk' exit.
 */
#define TERRAIN_MAP_WALK_EXIT "walk "

/**
 * This is the air room to use if nothing else is specfied.
 */
#define TERRAIN_MAP_DEFAULT_AIR_ROOM "/room/air_terrain"

/**
 * The class with the coordinate definitions in it.
 * @member x the x coordinate
 * @member y the y coordinate
 * @member z the z coordinate
 */
class coord {
   int x;
   int y;
   int z;
}

/**
 * This define is used by the world map to determine where to put
 * areas in the world.
 */
#define TERRAIN_MAP_WORLD_BOUNDARY (56 * TERRAIN_MAP_ONE_MILE)
/**
 * This define is used by the world map to determine how many areas are
 * in each region.  An area is each terrain map square, if you look
 * at the co-ordinates in the terrain handlers it fits to these values.
 */
#define TERRAIN_MAP_WORLD_REGION_SIZE 15
/**
 * This define is used by the world map to detmine the boundary of a
 * region.  You add the offset onto the co-ordinate and divide by the
 * boundary to get the area.  You then divide by the region size to get
 * the region.
 */
#define TERRAIN_MAP_WORLD_REGION_BOUNDARY (TERRAIN_MAP_WORLD_REGION_SIZE * BOUNDARY)
/**
 * This define is used by the world map to detmine the offset value to
 * co-ordinates.  This makes sure all the region values are positive.
 * You add the offset onto the co-ordinate and divide by the
 * boundary to get the area.  You then divide by the region size to get
 * the region.
 */
#define TERRAIN_MAP_WORLD_OFFSET (100 * TERRAIN_MAP_WORLD_BOUNDARY)
/**
 * This is the width of the region in areas.
 */
#define TERRAIN_MAP_WORLD_REGION_WIDTH 15

/**
 * This is the size of the world in pixels.  There are 4 pixels to a terrain
 * map, so 4 to an area.
 */
#define TERRAIN_MAP_WORLD_PIXEL_WIDTH 768

/**
 * This is the lowest x coordinate of the world.
 */
#define TERRAIN_MAP_WORLD_LOW_X (-(TERRAIN_MAP_WORLD_PIXEL_WIDTH / 2) * TERRAIN_MAP_GRID_SIZE * 10)
/**
 * This is the lowest y coordinate of the world.
 */
#define TERRAIN_MAP_WORLD_LOW_Y (-(TERRAIN_MAP_WORLD_PIXEL_WIDTH / 2) * TERRAIN_MAP_GRID_SIZE * 10)
/**
 * This is the highest x coordinate of the world.
 */
#define TERRAIN_MAP_WORLD_HIGH_X ((TERRAIN_MAP_WORLD_PIXEL_WIDTH / 2) * TERRAIN_MAP_GRID_SIZE * 10)
/**
 * This is the highest y coordinate of the world.
 */
#define TERRAIN_MAP_WORLD_HIGH_Y ((TERRAIN_MAP_WORLD_PIXEL_WIDTH / 2) * TERRAIN_MAP_GRID_SIZE * 10)

/**
 * This is the type to use when doing a find for the exactly room.
 */
#define TERRAIN_MAP_ROOM_EXACT 0
/**
 * This is the type to use when doing a find for a room to use for
 * the map.
 */
#define TERRAIN_MAP_ROOM_MAP   1
/**
 * This is the type to use when doing a find for a possible climbing
 * room.  A climbing room is a room which goes up or down from the
 * current height location.
 */
#define TERRAIN_MAP_ROOM_CLIMB 2


#endif
