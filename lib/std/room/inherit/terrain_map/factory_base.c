/**
 * This is specifically for creating rooms in the world map system, where
 * it just makes and passes out the room details.  So it does not need to
 * include all the room crap.
 * @author Pinkfish
 * @started Fri Jun 28 16:14:19 PDT 2002
 */
#include <terrain_map.h>

private int _room_jump_size;
private int _follow_road;
private int _default_height;
private int _min_height;
private string _adjacent_desc;
private mapping _height_info;
private mapping _adjacent_items;

void create() {
   _adjacent_items = ([ ]);
} /* create() */

/**
 * This method sets the room jump size for the room.
 * @param jump the room jump size
 */
void set_room_jump_size(int jump) {
   _room_jump_size = jump;
}

/**
 * This method returns the room jump size for the room
 * @return the room jump size
 */
int query_room_jump_size() {
   return _room_jump_size;
}

/**
 * This method sets the flag that makes us follow the road.
 * @param follow the follow road flag
 */
void set_follow_road(int follow) {
  _follow_road = follow;
}

/**
 * This method returns the follow road flag for the road.
 * @return the follow road flag
 */
int query_follow_road() {
   return _follow_road;
}

/**
 * This method sets the default height for the room.
 * @param def the default height for the room
 */
void set_default_height(int height) {
   _default_height = height;
}

/**
 * This method sets the string to use for adjancent descriptions.  THis is
 * the bit tacked onto the end of the long in another room.
 * @param desc the adjacent description
 */
void set_adjacent_description(string desc) {
   _adjacent_desc = desc;
}

/**
 * This method returns the string to use for adjancent descriptions.  THis is
 * the bit tacked onto the end of the long in another room.
 * @return the adjacent description
 */
string query_adjacent_description() {
   return _adjacent_desc;
}

/**
 * This method returns the default height for the room.
 * @return the default height for the room
 */
int query_default_height() {
   return _default_height;
}

/**
 * This method sets the minimum height, this is only really useful
 * in locations which have multiple depth rooms.
 * @param height the minimum height */
void set_minimum_height(int height) {
   _min_height = height;
}

/**
 * This method returns the mimum height allowed for this factory.
 * @return the mimuim height
 */
int query_minimum_height() {
   return _min_height;
}

/**
 * This method sets the height information for the room, setting up
 * which rooms are used at which heights.
 * @param heights the heights for the rooms
 */
void set_height_rooms(mapping map) {
   _height_info = map;
}

/**
 * This method figures out the base path to use for the specified room.
 * @param x the x position
 * @param y the y position
 * @param z the z position
 * @param air checking upwards in air land.
 * @return the base room
 */
string find_base_terrain_room(int x, int y, int z, string base_name, int map_room, string ref base) {
   // If there is a set room at a location, use it.
   if (_height_info && _height_info[z]) {
      return _height_info[z];
   }

   // Find the new room.
   if (z > _default_height) {
      // air room
      if (map_room == TERRAIN_MAP_ROOM_CLIMB) {
         return 0;
      }
      if (map_room == TERRAIN_MAP_ROOM_EXACT) {
         return TERRAIN_MAP_DEFAULT_AIR_ROOM;
      }
      if (!_height_info) {
         return base_name;
      }
      return _height_info[_default_height];
   }
   if (!_height_info) {
      if (z == _default_height) {
         return base_name;
      }
      return 0;
   }
   if (_min_height > z) {
      return 0;
   }
   if (z == _default_height) {
      return base_name; //__FILE__[0..<3];
   }
   return _height_info["default"];
}

/**
 * This method sets an item which will be visible from adjacent rooms.
 * @param name the name of the item
 * @param desc the description of the item
 */
void add_adjacent_item(string name, string desc) {
   _adjacent_items[name] = desc;
}

/**
 * This method returns all the adjacent items.
 * @return the mapping of all the adjacent items
 */
mapping query_adjacent_items() {
   return _adjacent_items;
}

mixed stats() {
   return ({
  ({ "room jump", _room_jump_size }),
  ({ "follow road", _follow_road }),
  ({ "adjacent desc", _adjacent_desc }) });
}
