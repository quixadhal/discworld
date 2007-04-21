/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: handler_base.c,v 1.116 2003/07/14 19:44:09 pinkfish Exp $
 *
*/
/**
* @main
* Base inherit for terrain map handler.
* <p>
* When creating a new terrain map, inherit from this file to make your area
* handler.
* <p>
* When inheriting from this file, it is essential that the functions
* query_map_file() and setup_handler() be overriden to return the full
* pathname of the area map, and to set up type mappings.
* <p>
* To inherit from this file, use the following lines:
* <ul>
* <li>#include <terrain_map.h>
* <li>
* <li>inherit TERRAIN_MAP_HANDLER_BASE
* </ul>
*
* @index terrain_map_handler
* @see terrain_map_inside
* @see terrain_map_outside
* @see terrain_map_intro
* @see terrain_map_example
* @author Dek
* @started Thurs April 12 2001 Too Damn Early BST
**/

#include <weather.h>
#include <terrain_map.h>
#include <player.h>

#define   AREA_OBSTACLE    0
#define   AREA_ROOM        1
#define   AREA_ROAD        2
#define   AREA_WORLD       3
#define   RANDOM_HANDLER   "/obj/handlers/random_num"

inherit "/std/room/inherit/terrain_map/distance_str";

#define X_DIFF 1
#define Y_DIFF 0
#define XY_DIFF 2

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
   // How many rooms to jump with each movement.
   int room_jump;
   // If this is 1 then we treat is as a road and try to follow twists.
   // If it is 0 then we go in the direction as far as we can.
   int room_jump_matching;
}

class feature_data {
   int range;
   string* descs;
   string* night_descs;
   mixed f_items;
   mixed f_item_desc;
   mixed f_item_night_desc;
   int whole_sentance;
   mixed chats;
   mixed night_chats;
   object feature_ob;
}

class sign_data {
   string long;
   string read_mess;
   string short;
   string name;
   string language;
}

class real_coords {
   int x1;
   int y1;
   int x2;
   int y2;
   int xdiff;
   int ydiff;
   int xydiff; // Diff in a diagonal.
   int zdiff;
   int in_world_map;
}


private mixed     *_room_map = 0;
private mixed     *_area_map = 0;
private mixed     *_zone_map = 0;
private int       _visible_distance;
private int       _width = 0;
private int       _height = 0;
private int       _normal_skip_size = 1;
private int       _random_desc_limit = 2;
private int       _random_chat_limit = 4;
private int       _newline_mode = 1;
private mapping   _room_zones = ([ ]);
private mapping   _feature_rooms = ([ ]);
private mapping   _char_map = ([ ]);
private mapping   _zone_types = ([ ]);
private mapping   _features = ([ ]);
private mapping   _signposts = ([ ]);
private mapping   _standard_exits = ([ ]);
private mapping   _adjacent_terrains = ([ ]);
private mapping   _random_descs = ([ ]);
private mapping   _random_chats = ([ ]);
private class real_coords _real_coords;
private mapping _key = ([ ]);
private mapping   _direcs = ([ "north":     ({ 0, 1, "south", Y_DIFF, 0 }),
                       "northeast": ({ 1, 1, "southwest", XY_DIFF, ({ "obvious", 0 }) }),
                       "east":      ({ 1, 0, "west", X_DIFF, 0 }),
                       "southeast": ({ 1, -1, "northwest", XY_DIFF, ({ "obvious", 0 }) }),
                       "south":     ({ 0, -1, "north", Y_DIFF, 0 }),
                       "southwest": ({ -1, -1, "northeast", XY_DIFF, ({ "obvious", 0 }) }),
                       "west":      ({ -1, 0, "east", X_DIFF, 0 }),
                       "northwest": ({ -1, 1, "southeast", XY_DIFF, ({ "obvious", 0 }) }) ]);
// This is the order to do the directions in.  Make sure we do the
// main cardinal ones first.
private string* _direc_order = ({
                        "north", "east", "south", "west",
                        "northeast", "southeast", "southwest", "northwest" });

private mixed    *_feature_direcs = ({ ({ "southwest", "south", "southeast" }),
                               ({ "west", "Oops", "east" }),
                               ({ "northwest", "north", "northeast" }) });

void load_area_map();
void load_zone_map();
class char_map_data query_char_map(int x, int y);
void setup_area_map();
void setup_room(object room, string base_path);
string find_adjacent_terrain(int x, int y);

// A load of accessor functions to make debugging a lot easier.
mixed   *query_room_map() { return _room_map; }
mixed   *query_area_map() { return _area_map; }
mixed   *query_zone_map() { return _zone_map; }
mapping query_zone_types() { return _zone_types; }
mapping query_feature_rooms() { return _feature_rooms; }
mapping query_features() { return _features; }
mapping query_room_zones() { return _room_zones; }
mapping query_char_maps() { return _char_map; }
mapping query_signposts() { return _signposts; }
mapping query_random_descs() { return _random_descs; }
mapping query_random_chats() { return _random_chats; }
mapping query_standard_exits() { return _standard_exits; }
mapping query_adjacent_terrains() { return _adjacent_terrains; }
class real_coords query_debug_real_coord() { return _real_coords; }
mixed* query_real_coords() { return ({ ({ _real_coords->x1, _real_coords->y1 }), ({ _real_coords->x2, _real_coords->y2 }) }); }
int     *query_dimensions() { return ({ _width, _height }); }
void    setup_handler() { }
string  query_map_file() { return ""; } // OVERRIDE to return the map file path.

// No, this makes us have a 700M driver and end up swapping like crazy!
//int     clean_up() { return 0;  }

/**
 * This method sets the real coordinates for the room without informing
 * the map handler.
 * @param x1 the start x location
 * @param y1 the start y location
 * @param x1 the end x location
 * @param y1 the end y location
 */
void set_real_coordinates_no_inform(int x1, int y1, int x2, int y2) {
   int tmp;

   if (x1 > x2) {
      tmp = x1;
      x1 = x2;
      x2 = tmp;
   }
   if (y1 > y2) {
      tmp = y1;
      y1 = y2;
      y2 = tmp;
   }
   _real_coords = new(class real_coords, x1:x1, y1:y1, x2:x2, y2:y2);
   _real_coords->xdiff = (x2 - x1) / _width;
   _real_coords->ydiff = (y2 - y1) / _height;
   // All z differentials in 15 foot lengths
   _real_coords->zdiff = 15;
   _real_coords->xydiff = to_int(sqrt(to_float(_real_coords->xdiff) * to_float(_real_coords->xdiff)+
                               to_float(_real_coords->ydiff) * to_float(_real_coords->ydiff)));
}

/**
 * @param x1 the start x location
 * @param y1 the start y location
 * @param x1 the end x location
 * @param y1 the end y location
 */
void set_real_coordinates(int x1, int y1, int x2, int y2) {
   set_real_coordinates_no_inform(x1, y1, x2, y2);
   _real_coords->in_world_map = TERRAIN_MAP_WORLD_MAP->add_terrain(base_name(this_object()),
                                      _real_coords->x1, _real_coords->y1,
                                      _real_coords->x2, _real_coords->y2);
}

/**
 * This method sets the real offsets for movement in the rooms.  This
 * is needed in rooms that do not have real co-ordinates set but still
 * require movement to generate the correct messages.
 * @param xoff how far you move in the x direction
 * @param yoff how far you move in the y direction
 * @param zoff how far you move in the z direction
 */
void set_real_offsets(int xoff, int yoff, int zoff) {
   _real_coords = new(class real_coords);
   _real_coords->xdiff = xoff;
   _real_coords->ydiff = yoff;
   _real_coords->xydiff = to_int(sqrt(to_float(_real_coords->xdiff) * to_float(_real_coords->xdiff)+
                               to_float(_real_coords->ydiff) * to_float(_real_coords->ydiff)));
}

/**
 * This method returns the room coordinates from the real coordinates.
 * @param x the x coordinate
 * @param y the y coordinate
 */
class coord query_terrain_from_real_coord(int x, int y, int z) {
   return new(class coord,
       x: (x - _real_coords->x1)  / _real_coords->xdiff,
       y: (y - _real_coords->y1) / _real_coords->ydiff,
       z: z / _real_coords->zdiff
        );
}

/**
 * This method returns the real coordinates from the terrain coordinates.
 * For simplicity we always assume the terrain sits at the water level.
 * @param x the x coordinate
 * @param y the y coordinate
 */
class coord query_real_from_terrain_coord(int x, int y, int z) {
   if (!_real_coords) {
      return new(class coord, x : 0, y : 0);
   }
   return new(class coord,
      x: _real_coords->x1 + x * _real_coords->xdiff,
      y: _real_coords->y1 + y * _real_coords->ydiff,
      z: z * _real_coords->zdiff
     );
}

/**
* @ignore
*/
int query_newline_mode() { return _newline_mode; }


/**
* This method sets the 'newline' mode for room description extras.
* If set to 1, a newline will be inserted after all feature and
* neighbouring room items. 1 is the default.
* @param mode the new mode.
*/
void set_newline_mode(int mode)

{
  _newline_mode = mode;
}

/**
 * This method sets the distance you can see in this terrain based on the
 * map.  The distance is used to determine nice little map things to show
 * the players.
 * @param distance the distance you can see
 */
void set_visible_distance(int distance) {
   _visible_distance = distance;
}

/**
 * This method returns the distance that you can see in this terrain.
 * @return the visible distance in the terrain
 */
int query_visible_distance() {
   return _visible_distance;
}

/**
* @ignore
*/
void create()

{
   setup_area_map();
   setup_handler();
}


// Loads in the maps, and if the array of rooms hasn't been allocated, do so.
/**
* @ignore
*/
void setup_area_map()

{
   int   index;

   load_area_map();
   load_zone_map();

   if (!arrayp(_room_map)) {
      _room_map = ({ });
      for (index = 0; index < _width; index++)
         _room_map += ({ allocate(_width) });
   }
}


// Install a room in the room array, and set up the descs and
// exits if need be.
/**
 * Not needed.
* @ignore
void install_room(object room)

{
   int   *room_coords = room->query_terrain_coords();
   int   x = room_coords[0];
   int   y = room_coords[1];

   if (!_room_map[y][x]) {
      _room_map[y][x] = room;
   }

   if (!room->query_installed()) {
      setup_room(room, "No base path");
   }
}
*/

/**
 * This method returns the region used when calculating the extents of the
 * feature.
 * @param name the name of the feature to find the region of
 * @return the feature region
 */
object query_feature_ob(string name) {
   return _features[name]->feature_ob;
}


// Calculate whether any of the features are visible from this room.
// If so, put in the relevant descs and add_items.
/**
* @ignore
* @return the blocking data
*/
mapping calc_features(object room)
{
   class feature_data    feature;
   string     title;
   int        *room_coords = room->query_terrain_coords();
   int        x = room_coords[0];
   int        y = room_coords[1];
   int        z = room_coords[2];
   mapping    data;
   string direc;
   int range;
   mapping block_feature;
   mixed* features;
   mixed* tmp;

   block_feature = ([ ]);
   features = ({ });
   foreach(title, feature in _features) {
      if (!room->can_view_feature(title)) {
         continue;
      }

      if (!feature || !feature->feature_ob) {
         debug_printf("Unable to find feature object for %O\n", title);
         continue;
      }

      data = feature->feature_ob->query_feature_desc_from(x, y, z);
      if (!data || !sizeof(data)) {
         continue;
      }

      if (feature->feature_ob->query_blocking()) {
         foreach (direc, range in data) {
            if (!block_feature[direc]) {
               if (block_feature[direc] > range) {
                  block_feature[direc] = range;
               } else {
                  map_delete(data, direc);
               }
            }
         }
      }

      if (sizeof(data)) {
         features += ({ ({ title, feature, data }) });
      }
   }
   foreach (tmp in features) {
      data = tmp[2];
      // Check and see if we are blocked.
      if (sizeof(block_feature)) {
         foreach (direc, range in data) {
            if (block_feature[direc] && block_feature[direc] < range) {
               map_delete(data, direc);
            }
         }
      }
      if (sizeof(data)) {
         feature = tmp[1];
         title = tmp[0];
         room->add_feature(title, data, feature->f_items, feature->whole_sentance);
         if (!_feature_rooms[title]) {
            _feature_rooms[title] = ({ });
         } else {
            _feature_rooms[title] = filter(_feature_rooms[title], (: $1 && objectp($1) :));
         }

         _feature_rooms[title] |= ({ room });
      }
   }
}


// See if this room has a signpost. If so, make it.
/**
* @ignore
*/
void add_signposts(object room)

{
   class sign_data sign;
   int     *coords = room->query_terrain_coords();
   int     x = coords[0];
   int     y = coords[1];
   string  title = sprintf("%d:%d", x, y);
   object  sign_ob;

   //tell_creator("dek", "Looking for a sign: %s.\n", title);

   sign = _signposts[title];
   if (!sign) {
      return;
   }

   sign_ob = room->add_sign(sign->long, sign->read_mess,
                  sign->short, sign->name,
                  sign->language);
   sign_ob->move(room);
}


// See what zones (if any) this room should belong to, and add them.
/**
* @ignore
*/
void setup_zones(object room)

{
   string  *zones;
   int     *coords = room->query_terrain_coords();
   int     x = coords[0];
   int     y = coords[1];
   string  type;
   string  zone;

   if (!sizeof(_zone_map)) {
      return;
   }

   type = _zone_map[y][x..x];
   zones = _zone_types[type];
   if (!zones) {
      return;
   }

   foreach (zone in zones) {
      room->add_zone(zone);
   }
}


void calc_random_descs(object room, string key)

{
   int      limit = _random_desc_limit;
   int      *indices = ({ });
   int      *coords = room->query_terrain_coords();
   int      seed = (coords[0] * 2) + coords[1];
   mapping  desc;
   mixed    *descs = _random_descs[key];
   int      index;

   if (limit > sizeof(descs)) {
      limit = sizeof(descs);
   }

   if (!limit) {
      return;
   }

   while (sizeof(indices) < limit) {
      index = RANDOM_HANDLER->random(sizeof(descs), seed++);
      if (member_array(index, indices) >= 0) {
         continue;
      }

      indices += ({ index });
   }
   for (index = 0; index < limit; index++) {
      desc = descs[indices[index]];
      room->add_random_desc(desc["desc"]);
      room->add_item(desc["items"], desc["item_desc"]);
   }
}


void calc_random_chats(object room, string key)

{
}

/**
 * This method finds the co-ordinates to use for getting to the
 * next room.  THis handles messing about to make bridges and things
 * @param x the x coordinate
 * @param y the y coordinate
 * @param z the z coordinate
 * @param direc the direction it is from
 * @param key the key is it from
 * @return the new mangled co-ordinates
 */
string query_dest_coordinate_string(int x, int y, int z, string direc, string key) {
   class char_map_data  room_setup;

   room_setup = query_char_map(x, y);

   if (room_setup->type == AREA_WORLD) {
      return room_setup->path->query_dest_coordinate_string(x, y, z, direc, key);
   }
   return 0;
}

/**
 * This method returns the ground room for a specific co-ordinate.
 * @param x the x location
 * @param y the y location
 * @return the terrain co-ordinates of the ground room
 */
int* query_ground_room_co_ords(int x, int y) {
   class char_map_data  room_setup;
   
   room_setup = query_char_map(x, y);
   return ({ x, y, room_setup->height });
}

/**
 * This method returns the location of the ground room.  It is basically
 * just the co-ordates connected onto the terrain_handler definition.
 * @param x the x location
 * @param y the y location
 * @return the string name of the ground room
 */
string query_ground_room(int x, int y) {
   int* co_ords;

   co_ords = query_ground_room_co_ords(x, y);
   return base_name(this_object()) + ":" + co_ords[0] + ":" + co_ords[1] + ":" + co_ords[2];
}

/**
 * This method returns the basic room as the specified location.  A climb
 * room type means a room that is up or down on the z axis from where we
 * are, but we are allowed to climb to.
 * @param x the x location
 * @param y the y location
 * @param z the z location
 * @param map_room check down through the air for the room for the map
 * @param base the updated base (for climbing rooms)
 * @return the basic room for the location
 */
string query_climb_base_room(int x, int y, int z, int map_room, string ref base) {
   class char_map_data  room_setup;
   string *tmp;
   
   room_setup = query_char_map(x, y);

   if (!room_setup) {
      return 0;
   }
   if (room_setup->type == AREA_WORLD) {
     // Pass off the creation to the room file.
     if(file_size(room_setup->path + ".c") == -1) {
       tmp = explode(room_setup->path, "_edge");
       if(sizeof(tmp) && file_size(tmp[0] + ".c") > 0)
         room_setup->path = tmp[0];
     }
     
     return room_setup->path->find_base_terrain_room(x, y, z, room_setup->path, map_room, ref base);
   } else if (room_setup->height_rooms) {
      // Find the new room.
      if (z > room_setup->height) {
         if (map_room == TERRAIN_MAP_ROOM_MAP) {
            return room_setup->path;
         }
         if (map_room != TERRAIN_MAP_ROOM_CLIMB) {
            // air room.
            return TERRAIN_MAP_DEFAULT_AIR_ROOM;
         }
         return 0;
      }
      if (z == room_setup->height) {
         return room_setup->path;
      }
      if (room_setup->height_rooms[z]) {
         return room_setup->height_rooms[z];
      }
      if (room_setup->height_rooms["minimum"] > z) {
         return 0;
      }
      return room_setup->height_rooms["default"];
   } else if (z == room_setup->height) {
      return room_setup->path;
   } else if (map_room && z > room_setup->height) {
      return room_setup->path;
   } else if (z > room_setup->height) {
      if (map_room != TERRAIN_MAP_ROOM_CLIMB) {
         return TERRAIN_MAP_DEFAULT_AIR_ROOM;
      }
   }
   return 0;
}

/**
 * This returns the destination room in the given directoin, initially
 * it tries for a normal exit type.  If that fails, it tries for a climbing
 * exit type.  A climbing exit type is one that moves up or down from the
 * current z height.
 * @param x the x co-ordinate
 * @param y the y co-ordinate
 * @param z the z co-ordinate
 * @param base the base path of the parent terrain handler
 */
string query_base_room(int x, int y, int z, int map_room) {
   string dest;

   dest = "bing";
   return query_climb_base_room(x, y, z, map_room, ref dest);
}

/**
 * The main nasty func for setting up a room. It adds all the necessary exits,
 * and adds to the room's extra_look & add_items for anything outside the room
 * which should be visible. It's fairly nasty. But hey - it works.
 */
void setup_room(object room, string base_path)
{
   string main_base;
   string base;
   int      *room_coords = room->query_terrain_coords();
   mixed    *coords;
   int      x;
   int      y;
   string   direc;
   class char_map_data  room_setup;
   string*  dirs;
   mapping  adjacent_descs = ([ ]);
   mapping  adjacent_sent = ([ ]);
   mapping  distant;
   string   adjacent;
   string   key;
   string   room_long = "";
   string   key_here;
   mixed    *exit_mods;
   object   new_room;
   string   outside_types = "";
   mixed    *exits;
   mapping  exit;
   string   *sentances;
   mixed   tmp;
   int z;
   int road_jump;
   class coord fluff;
   string* miss_road;
   mapping ignore_exits;
   string  walk_pre;
   string  walk_full;
   string  journey_pre;
   string  new_base_path;

   if (!arrayp(room_coords)) {
      debug_printf("Unable to find room coordinates from %O\n", file_name(room));
      return ;
   }
   room_setup = query_char_map(room_coords[0], room_coords[1]);
   key_here = room_setup->char;

   if (room_setup->room_desc) {
      adjacent_descs[room_setup->room_desc] = ({ "here" });
      adjacent_sent[room_setup->char] = 1;
      if( room_setup->items ) {
          if( stringp( room_setup->items ) &&
              stringp( room_setup->item_desc ) ) {
              room->add_item( room_setup->items, room_setup->item_desc );
          } else if( arrayp( room_setup->items ) &&
              arrayp( room_setup->item_desc ) && sizeof( room_setup->items ) ) {
              if( sizeof( room_setup->items ) !=
                  sizeof( room_setup->item_desc ) ) {
                  debug_printf( "Error, unequal arrays.  Item not set." );
              } else {
                  int i;

                  for( i = 0; i < sizeof( room_setup->items ); i++ ) {
                      room->add_item( room_setup->items[i],
                          room_setup->item_desc[i] );
                  }
              }
          }
      }

   }

   if (room_setup->room_jump_matching) {
      road_jump = room_setup->room_jump;
   }

   walk_pre = room->query_terrain_map_walk_exit();
   journey_pre = room->query_terrain_map_journey_exit();

   exits = _standard_exits[sprintf("%d:%d:%d", room_coords[0], room_coords[1], room_coords[2])];
   ignore_exits = ([ ]);
   if (exits) {
      foreach(exit in exits) {
         room->add_exit(exit["direc"], exit["dest"], exit["type"]);
         if (exit["modifiers"]) {
            room->modify_exit(exit["direc"], exit["modifiers"]);
         }
         ignore_exits[exit["direc"]] = 1;
      }
   }

   //
   // This is the start of the pre-caclulated exits for the terrain.
   // All the cardinal directions are checked for their ability to be
   // gone up and down.
   // Most checks are done on the base object (if possible) to avoid
   // loading everything in sight.
   //

   // Check the up and down exits first.
   main_base = base_name(this_object());
   if (room_setup->height_rooms ||
       room_setup->type == AREA_WORLD) {
      int i;

      x = room_coords[0];
      y = room_coords[1];
      // Ok, zip through this mapping and see if we match anything.
      // This finds if there is a room above or below us at all.
      for (i = 0; i < 2; i++) {
         z = room_coords[2];
         if (!i) {
            direc = "up";
            tmp = "down";
            z++;
         } else {
            direc = "down";
            tmp = "up";
            z--;
         }
         base = query_base_room(x, y, z, TERRAIN_MAP_ROOM_EXACT);
         if (base) {
            // There is a room!  Check and see if we can get in it or not.
            new_room = load_object(base);
            new_base_path = base;
            if (!new_room) {
               continue;
            }
            if (new_room->can_enter_from(key_here, direc, base_path) &&
                room->can_exit_to(key_here, direc, new_base_path)) {
               room->add_exit(direc, main_base + ":" + x + ":" + y + ":" + z, "path");
               exit_mods = room->get_to_same(tmp, direc, key, new_base_path);
               if (exit_mods) {
                   room->modify_exit(direc, exit_mods);
               }
            }
         }
      }
   }

   //
   // This is to check for crossing over roads/rivers on a diagonal.
   //
   miss_road = allocate(3 * 3);

   // Check the other exits.
   foreach(direc in _direc_order) {
      coords = _direcs[direc];
      x = coords[0] + room_coords[0];
      y = coords[1] + room_coords[1];
      z = room_coords[2];
      if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) {
         // Check and see if we have an adjacent terrain or not
         base = find_adjacent_terrain(x, y);
         if (!base) {
            continue;
         }
         fluff = query_real_from_terrain_coord(x, y, z);
         fluff = base->query_terrain_from_real_coord(fluff->x, fluff->y, fluff->z);
         room_setup = base->query_char_map(fluff->x, fluff->y);
         if (room_setup && room_setup->type != AREA_OBSTACLE) {
            tmp = base->query_base_room(fluff->x, fluff->y, z, TERRAIN_MAP_ROOM_EXACT);
            if (tmp) {
               new_base_path = tmp;
               new_room = load_object(tmp);
               base += ":" + fluff->x + ":" + fluff->y + ":" + z;
            } else {
               new_room = 0;
               new_base_path = 0;
            }
         }
      } else {
         room_setup = query_char_map(x, y);
         if (room_setup && room_setup->type != AREA_OBSTACLE) {
            tmp = query_base_room(x, y, z, TERRAIN_MAP_ROOM_EXACT);
            new_base_path = tmp;
            if (tmp) {
               new_room = load_object(tmp);
               base = main_base + ":" + x + ":" + y + ":" + z;
            } else {
               new_room = 0;
            }
         }
      }
      if (!room_setup || !new_room) {
         continue;
      }

      key = room_setup->char;
      // Make sure it is cardinal.  Check to see if we can leap this
      // location or not.
      // We use the file name of the new room to check for the diagonal
      // exit policy problems.  This should solve the problems of moving
      // across maps.
      if (!coords[0] || !coords[1]) {
         miss_road[(coords[0] + 1) * 3 + coords[1] + 1] = new_base_path;
      } else {
         // Only do this if the values are not 0 too.
         if ((miss_road[(coords[0] + 1) * 3 + 1] != new_base_path ||
              miss_road[coords[1] + 4] != new_base_path) &&
             miss_road[coords[1] + 4] &&
             miss_road[(coords[0] + 1) * 3 + 1] &&
             !miss_road[coords[1] + 4]->can_move_between(key, direc,
                                    miss_road[(coords[0] + 1) * 3 + 1],
                                    miss_road[coords[1] + 4], new_base_path)) {
            // Don't allow this to happen.
            continue;
         }
      }

      switch(room_setup->type) {
         case AREA_OBSTACLE   :
            break;
         case AREA_ROAD       :
         case AREA_ROOM       :
         case AREA_WORLD      :
           if (ignore_exits[direc]) {
              break;
           }
           if (new_room->can_enter_from(key_here, direc, base_path) &&
                room->can_exit_to(key, direc, new_base_path)) {

              // Now check and see if we need to modify the exit to this
              // room at all.
              tmp = new_room->query_enter_exit_direction(key, key_here, direc, room, base_path);
              if (tmp) {
                 room->add_exit(tmp, base, "road");
                 walk_full = tmp;
              } else {
                 // If they are not doing anything, setup all the default
                 // hop stuff.
                 room->add_exit(direc, base, "road");
                 room->add_exit(walk_pre + direc, base, "road");
                 room->add_default_exit(direc, base);
                 walk_full = walk_pre + direc;
              }
              if (coords[4]) {
                 room->modify_exit(walk_full, coords[4]);
                 room->modify_exit(direc, coords[4]);
              }
              if (key_here == key ||
                  (road_jump &&
                   room_setup->room_jump_matching &&
                   room_setup->room_jump == road_jump)) {
                 exit_mods = room->get_to_same(_direcs[direc][2], direc, key, new_base_path);
                 if (exit_mods) {
                    room->modify_exit(direc, exit_mods);
                    room->modify_exit(walk_full, exit_mods);
                 }
                 if (room_setup->room_jump > 1) {
                    //room->modify_exit(direc, ({ "obvious", 0 }));
                    room->modify_exit(walk_full, ({ "obvious", 0 }));
                    room->add_variable_exit(journey_pre + direc);
                    room->add_exit(journey_pre + direc, base, "road");
                    exit_mods = room->get_to_same(walk_full, journey_pre + direc, key, new_base_path);
                    if (coords[4]) {
                       room->modify_exit(journey_pre + direc, coords[4]);
                    }
                    if (exit_mods) {
                        room->modify_exit(journey_pre + direc, exit_mods);
                    }
                 }
              } else {
                 exit_mods = room->get_to_other(_direcs[direc][2], direc, key, new_base_path);
                 if (exit_mods) {
                     room->modify_exit(walk_full, exit_mods);
                 }
                 exit_mods = new_room->
                             get_from_other(_direcs[direc][2], direc, key_here, base_path);
                 if (exit_mods) {
                     room->modify_exit(walk_full, exit_mods);
                 }
              }
              room->modify_exit(direc, ({ "obvious", 0 }));
           }
           break;
      }
      if ((key_here != key) && (member_array(key[0], outside_types) < 0)) {
         outside_types += key;
      }
      adjacent = room_setup->adjacent_desc;
      if (adjacent && strlen(adjacent) > 1) {
         if (room->can_view_adjacent_desc(room_setup->char,
                                          key_here,
                                          room_setup->path,
                                          z,
                                          direc)) {
            if (!room_setup->whole_sentance) {
               adjacent_sent[adjacent] = 1;
            }
            if (!adjacent_descs[adjacent]) {
               adjacent_descs[adjacent] = ({ });
               if( room_setup->items ) {
                   if (!adjacent_sent[room_setup->char]) {
                      adjacent_sent[room_setup->char] = 1;
                      if( stringp( room_setup->items ) &&
                          stringp( room_setup->item_desc ) ) {
                          room->add_item( room_setup->items,
                              room_setup->item_desc );
                      } else if( arrayp( room_setup->items ) &&
                          arrayp( room_setup->item_desc ) &&
                          sizeof( room_setup->items ) ) {
                          if( sizeof( room_setup->items ) !=
                              sizeof( room_setup->item_desc ) ) {
                              debug_printf( "Error, unequal arrays.  Item not "
                                  "set." );
                          } else {
                              int i;
   
                              for( i = 0; i < sizeof( room_setup->items ); i++) {
                                  room->add_item( room_setup->items[i],
                                      room_setup->item_desc[i] );
                              }
                          }
                      }
                   }
               }
            }
            adjacent_descs[adjacent] += ({ direc });
         }
      }
   }

   sentances = ({ });
   foreach(adjacent, dirs in adjacent_descs) {
      tmp = replace_string(adjacent,
                "$D", query_multiple_short(dirs, "the", 1));
      if (adjacent_sent[adjacent]) {
         if (strlen(tmp) > 1) {
            sentances += ({ tmp });
         }
      } else if (tmp != "") {
         if (query_newline_mode()) {
            room_long += tmp + "\n";
         } else {
            room_long += "  " + tmp;
         }
      }
   }

   if (sizeof(sentances)) {
      if (!query_newline_mode()) {
         room_long += "  ";
      }
      room_long += capitalize(query_multiple_short(sentances, "the", 1)) + ".";
      if (query_newline_mode()) {
         room_long += "\n";
      }
   }

   room->set_outside_types(outside_types);
   add_signposts(room);
   setup_zones(room);

   // Do the features.
   exit = calc_features(room);
   if (_real_coords && (_real_coords->x1 || _real_coords->x2)) {
      fluff = query_real_from_terrain_coord(room_coords[0], room_coords[1], 0);
      foreach (string title in TERRAIN_MAP_WORLD_MAP->query_features_at(fluff->x, fluff->y)) {
         if (!room->can_view_feature(title)) {
            continue;
         }
         distant = title->query_feature_desc_from(fluff->x, fluff->y, fluff->z);
         if (sizeof(distant)) {
            // Check and see if the distant features are blocked.
            if (sizeof(exit)) {
               foreach (direc, int range in distant) {
                  if (exit[direc] && exit[direc] < range) {
                     map_delete(distant, direc);
                  }
               }
            }
            if (sizeof(distant)) {
               room->add_distant_feature(title, distant);
            }
         }
      }
   }


   calc_random_descs(room, key_here);
   calc_random_chats(room, key_here);

   room->set_extra_long(room_long);

   room->set_installed(1);
   room->calc_exits();
}

/**
 * THis method finds a terrain adjacent to us from this location
 * in the specific direction.
 * @param x the x location
 * @param y the y location
 */
string find_adjacent_terrain(int x, int y)
{
   string index;
   class coord real;

   if (!_real_coords || !_real_coords->in_world_map) {
      return 0;
   }

   index = x + ":" + y;
   if (!undefinedp(_adjacent_terrains[index])) {
      return _adjacent_terrains[index];
   }

   real = query_real_from_terrain_coord(x, y, 0);
//printf("%O, %O) %O\n", x, y, real);
   // Ok, now see whayt we can find on the world map handler.
   _adjacent_terrains[index] = TERRAIN_MAP_WORLD_MAP->find_terrain_at(real->x, real->y);
   return _adjacent_terrains[index];
}

/**
 * This is specifically for debug.  It finds al the adjacent terrains.
 */
void find_all_adjacent_terrains() {
   find_adjacent_terrain(-1,-1);
   find_adjacent_terrain(-1,0);
   find_adjacent_terrain(-1,40);
   find_adjacent_terrain(0,40);
   find_adjacent_terrain(40,40);
   find_adjacent_terrain(40,0);
   find_adjacent_terrain(40,-1);
   find_adjacent_terrain(0,-1);
}

string* debug_adjacent_terrains() {
   return ({ find_adjacent_terrain(-1, 0),
             find_adjacent_terrain(0, -1),
             find_adjacent_terrain(0, _height),
             find_adjacent_terrain(_width, 0) });
}

/**
 * This method find the specific squence of rooms in the given direction
 * with the given room jump.  We do not worry about contents here.  Only if
 * it is a special interest marker or if there are two different ways from
 * the location of the same type.  Items in the map are set as being
 * special interest to stop a jump.
 * @param x the x co-ordinate to start from
 * @param y the y co-ordinate to start from
 * @return the list of rooms
 */
mixed* query_rooms_to(int x, int y, int z, string direc, int len,
                       string main_key, int jump_len, int road_type ) {
   class char_map_data  room_setup;
   class coord real;
   string new_direc;
   string found_direc;
   string terr;
   string bit;
   int num;
   int found;
   int old_x;
   int old_y;
   int new_x;
   int new_y;
   mixed* ret;
   mixed* data;

   ret = ({ ({ }), ({ }) });
   do {
      data = _direcs[direc];
      if (!data) {
         debug_printf("Bad direction %O\n", direc);
         continue;
      }
      old_x = x;
      old_y = y;
      x += data[0];
      y += data[1];
      if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) {
         terr = find_adjacent_terrain(x, y);
         if (!terr) {
            return ret;
         }
         real = query_real_from_terrain_coord(x, y, 0);
         data = terr->query_more_terrains_rooms_from(real->x, real->y, real->z, direc,
                                 len - num, jump_len,
                                 main_key, road_type);
         if (sizeof(data)) {
            ret[0] += data[0];
            ret[1] += data[1];
         }
         return ret;
      }
      ret[0] += ({ direc });
      bit = sprintf("%d:%d:%d", x, y, z);
      ret[1] += ({ sprintf("%s:%s", base_name(this_object()), bit) });
      // Always stop if there is an exit forced in the room.
      if (_standard_exits[bit]) {
         break;
      }

      if (road_type) {
         // Search around for the next location.
         found = 0;
         foreach (new_direc, data in _direcs) {
            new_x = x + data[0];
            new_y = y + data[1];
            if (new_x != old_x || new_y != old_y) {
               if ((new_x < 0) || (new_x >= _width) || (new_y < 0) || (new_y >= _height)) {
                  terr = find_adjacent_terrain(new_x, new_y);
                  if (terr) {
                     real = query_real_from_terrain_coord(new_x, new_y, z);
                     real = terr->query_terrain_from_real_coord(real->x, real->y, z);
                     room_setup = terr->query_char_map(real->x, real->y);
                  } else {
                     room_setup = 0;
                  }
               } else {
                  room_setup = query_char_map(new_x, new_y);
               }
               if (room_setup && room_setup->room_jump == jump_len) {
//printf("Found! %O %O %O\n", new_direc, room_setup->room_jump, jump_len);
                  found_direc = new_direc;
                  found++;
               }
            }
         }
//printf("(%O, %O) %O %O %O\n", x, y, found, found_direc, jump_len);
         if (found == 1) {
            direc = found_direc;
         } else {
            // dead end or junction.
            break;
         }
      } else {
         room_setup = query_char_map(x + data[0], y + data[1]);
         if (!room_setup || room_setup->char != main_key) {
            break;
         }
      }
   } while (num++ < len);

   return ret;
} /* query_rooms_to() */

/**
 * This method is used by connecting terrains to find the next room in
 * sequence.
 * @param x the real x coordinate
 * @param y the real y coordinate
 * @param moves_left the number of moves left to pop along
 * @param jump the jump length we are looking for
 * @param main_key the key, this is only used for non-road jumps
 * @param road_jump is this a road jump
 */
mixed* query_more_terrains_rooms_from(int x, int y, int z, string direc,
                                       int moves_left, int jump,
                                       string main_key, int road_jump) {
   class coord bing;
   class char_map_data  room_setup;
   int found;
   string found_direc;
   string new_direc;
   mixed* data;
   int new_x;
   int new_y;

//tell_creator("pinkfish", "%O: %O %O %O %O %O\n", base_name(this_object()), x, y, direc, road_jump, main_key);
   bing = query_terrain_from_real_coord(x, y, 0);
   room_setup = query_char_map(bing->x, bing->y);
   if (road_jump) {
      // Is the jump value set correctly here?
      if (room_setup->room_jump != jump) {
         data = _direcs[direc];
         bing->x -= data[0];
         bing->y -= data[1];
         // Try and find it from here.  We move away from the edge of the
         // terrain and search from there (the room we came from).
         foreach (new_direc, data in _direcs) {
            new_x = bing->x + data[0];
            new_y = bing->y + data[1];
            room_setup = query_char_map(new_x, new_y);
            if (room_setup && room_setup->room_jump == jump) {
               found = 1;
               found_direc = new_direc;
               break;
            }
         }
         if (!found) {
            return ({ });
         }
         direc = found_direc;
      } else {
         data = _direcs[direc];
         bing->x -= data[0];
         bing->y -= data[1];
      }
      return query_rooms_to(bing->x, bing->y, bing->z, direc, moves_left,
                            main_key, jump, road_jump);
   } else {
      // Find where we are.
      if (room_setup->char == main_key) {
         data = _direcs[direc];
         bing->x -= data[0];
         bing->y -= data[1];
         return query_rooms_to(bing->x, bing->y, bing->z, direc, moves_left,
                            main_key, jump, road_jump);
      }
      return ({ });
   }
}

/**
 * This method finds the next room in the path.  It returns the next
 * room as the first element in the array and the list of the directions
 * to get there as the second element.
 * @param x the start x position
 * @param y the start y position
 * @param z the start z position
 * @param direc the direction to go from here
 * @return ({ the end location, directions... })
 */
string* find_next_room_from(int x, int y, int z, string direc) {
   class char_map_data  room_setup;
   string test;
   object ob;
   object* obs;
   mixed* stuff;
   int pos;

   //direc = direc[strlen(SKIP_EXIT_PRE)..];

   room_setup = query_char_map(x, y);
   stuff = query_rooms_to(x, y, z, direc, room_setup->room_jump,
                          room_setup->char, room_setup->room_jump,
                          room_setup->room_jump_matching);
   foreach (test in stuff[0]) {
      ob = find_object(test);
      if (ob) {
         obs = filter(all_inventory(ob), (: living($1) :));
         if (sizeof(obs) > 0) {
            return ({ test }) + stuff[0..pos];
         }
      }
      pos++;
   }
   if (sizeof(stuff[1])) {
      return ({ stuff[1][<1] }) + stuff[0];
   }
   return 0;
} /* find_next_room_from() */

// Called by the room-inherit when a room has a zone added. Lets us maintain
// a list of what rooms are in what zones, so we can do snazzy things with them.
/**
* @ignore
*/
void add_room_to_zone(object room, string zone)

{
   object *zones = _room_zones[zone];
   if (!zones)
      zones = ({ room });
   else if (member_array(room, zones) == -1)
      zones += ({ room });

   _room_zones[zone] = zones - ({ 0 });
}


/**
* This method queries the path of a room. If it takes the form:
* '/d/full/path/name.c:x:y' where x and y are integers, it clones
* a new room at the specified coordinates, sets it up fully, and returns it.
* If the room path is of the standard form, it simply returns the loaded
* room.
* @param room_path the full pathname of the room to be cloned.
* @return the cloned/loaded room, or 0 if it could not be cloned/loaded
* @see load_room_at
* @see query_room_at
*/
mixed query_room(int x, int y, int z)

{
   class char_map_data room_map = query_char_map(x, y);
   string base_path;
   object room;
   class coord real;

   base_path = query_base_room(x, y, z, TERRAIN_MAP_ROOM_EXACT);
   if (!base_path) {
      return 0;
   }

   //
   // Not in the driver mapping, so we must need to create it.
   // We assume the external stuff is doing the correct name stuff.
   //
   room = clone_object(base_path);
   if (!room) {
      debug_printf("Unable to create room from %O at %O %O %O\n", base_path, x, y, z);
   } else {
      room->create_terrain_room();
      room->set_terrain_coords(x, y, z);
      real = query_real_from_terrain_coord(x, y, z);
      room->set_co_ord(({ real->x, real->y, real->z }));
      if (classp(_real_coords)) {
         room->set_room_size(({ _real_coords->xdiff, _real_coords->ydiff, _real_coords->zdiff }));
      }
      room->set_terrain_handler(base_name(this_object()));
      room->terrain_setup();
      room->add_property("base path", base_path);
      _room_map[y][x] = room;
      setup_room(room, base_path);
      if (room_map->type == AREA_WORLD && base_path != room_map->path) {
         // Use this to do any useful special stuff (like river flows).
         room_map->path->setup_in_terrain(room, x, y, z);
      }
   }

   return room;
}


/**
* This method is used to find a room at the specified coordinates,
* if one has already been cloned.
* If a room has been cloned at those coordinates, it is returned,
* @param x the x coordinate of the room (internal coordinate system)
* @param y the y coordinate of the room (internal coordinate system)
* @return the room at those coordinates, or 0 if none has been cloned
* @see load_room_at
* @see query_room
* @see query_rooms_in_block
* @see query_rooms_in_range
*/
object query_room_at(int x, int y, int z)

{
   if ((x < 0) || (y < 0) || (x >= _width) || (y >= _height))
      return 0;

   return find_object(sprintf("%s:%d:%d:%d", base_name(this_object()), x, y, z));
}


/**
* This method is used to find all the rooms currently cloned within a
* specified rectangular area.
* @param x the x coordinate of rectangle's bottom left corner
* @param y the y coordinate of rectangle's bottom left corner
* @param width the width of the rectangle
* @param height the height of the rectangle
* @return an array containing any rooms currently cloned within the
* specified rectangle
* @see query_room_at
* @see query_rooms_in_range
* @see query_rooms_in_zone
*/
object *query_rooms_in_block(int x, int y, int z, int width, int height)

{
   int      h, v;
   mixed    *result = ({ });
   object   room;

   for (h = x; h < (x + width); h++) {
      for (v = y; v < (y + height); v++) {
         room = query_room_at(h, v, z);
         if (room)
            result = result + ({ room });
      }
   }

   return result;
}


/**
* This method is used to find all the rooms currently cloned within a
* specified distance of the specified coordinates.
* @param x the x coordinate of the center point
* @param y the y coordinate of the center point
* @param max the maximum distance of rooms to be returned
* @param min the minimum distance of rooms to be returned. Defaults to 0
* @return an array containing any rooms currently cloned within the
* specified range
* @see query_room_at
* @see query_rooms_in_block
* @see query_rooms_in_zone
*/
varargs object *query_rooms_in_range(int x, int y, int z, int max, int min)

{
   int      h, v;
   mixed    *result = ({ });
   object   room;
   int    dist;

   for (h = x - max; h <= (x + max); h++) {
      for (v = y - max; v <= (y + max); v++) {
        dist = to_int(sqrt(pow(x - h, 2) + pow(y - v, 2)) + 0.4999);
     //   tell_creator("dek", "Coords: (%d, %d), Range: %d, Dist: %d\n", h, v, max, dist);
        if ((dist > max) || (dist < min))
           continue;
         room = query_room_at(h, v, z);
         if (room)
            result = result + ({ room });
      }
   }

   return result;
}


/**
* This method is used to find a room at the specified coordinates,
* or to clone and setup a new one if none exists so far.
* @param x the x coordinate of the room (internal coordinate system)
* @param y the y coordinate of the room (internal coordinate system)
* @return the room at those coordinates, or 0 if none can be cloned
* @see load_room_at
* @see query_room
*/
object load_room_at(int x, int y, int z)

{
   class char_map_data room_map = query_char_map(x, y);

   if (!room_map) {
      debug_printf("Unable to load room at %d %d\n", x, y);
      return 0;
   }

   if (room_map->type == AREA_OBSTACLE) {
      return 0;
   }

   return load_object(sprintf("%s:%d:%d:%d", base_name(this_object()), x, y, z));
}


// Given coordinates, get the mapping which controls what goes in the room.
/**
* @ignore
*/
class char_map_data query_char_map(int x, int y)
{
   string   type;
   class char_map_data  room_map;

   if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height)) {
      return 0;
   }

   if (x >= sizeof(_area_map[y])) {
      debug_printf("The x value is out of range: %d > %d", x, sizeof(_area_map[y]));
      return 0;
   }

   type = _area_map[y][x..x];

   room_map = copy(_char_map[type]);

   if (!room_map) {
      debug_printf("There is no char type for '%s'", type);
      return 0;
   }

   room_map->char = type;
   return room_map;
}

/**
 * This method returns a grid of the specified size around the center
 * point to give creators an overview of where they are currently.
 * @param x the x co-ordinate
 * @param y the y co-ordinate
 * @param width the width of the box
 */
string query_debug_map(int x, int y, int width, int showx, int showy) {
   int i;
   int j;
   int sx;
   int sy;
   int ny;
   string ret;
   string base;
   string new_base;
   string colour;

   ret = "";
   sx = x - width / 2;
   sy = y + width / 2;
   for (j = 0; j < width; j++) {
      if (sy - j >= 0 && sy - j < _height) {
         for (i = 0; i < width; i++) {
            if (sx + i >= 0 && sx + i < _width) {
               if (sx +i == showx && sy - j == showy) {
                  ret += "%^YELLOW%^" + _area_map[sy - j][sx + i..sx + i] +
                         "%^RESET%^";
                  colour = 0;
               } else {
                  if (colour) {
                     ret += "%^RESET%^";
                     colour = 0;
                  }
                  ret += _area_map[sy - j][sx + i..sx + i];
               }
            } else {
               // Need to get the character from a nearby terrain instead.
               if (sx + i >= _width) {
                  base = find_adjacent_terrain(_width + 1, sy - j);
               } else {
                  base = find_adjacent_terrain(-1, sy - j);
               }
//printf("Base %O (%d,%d)\n", base, sx + i, sy - j);
               if (base) {
                  //ret += "-";
                  if (colour != "%^CYAN%^") {
                     ret += "%^BOLD%^%^CYAN%^";
                     colour = "%^CYAN%^";
                  }
                  if (sx + i >= _width) {
                     //ret += "-" + (sx + i - _width) + "-";
                     // Assume same size.
                     ret += base->query_debug_map_char(sx + i - _width, sy - j);
                  } else {
                     //ret += "-" + (sx + i + _width) + "-";
                     ret += base->query_debug_map_char(sx + i + _width, sy - j);
                  }
               } else {
                  ret += " ";
               }
            }
         }
         if (colour) {
            ret += "%^RESET%^";
            colour = 0;
         }
         ret += "\n";
      } else {
         // Need to get the character from a nearby terrain instead.
         base = find_adjacent_terrain(sx + i, sy - j);
         if (base) {
            if (sy - j < 0) {
               ny = sy - j + _height;
            } else {
               ny = sy - j - _height;
            }
            for (i = 0; i < width; i++) {
               if (sx + i >= 0 && sx + i < _width) {
                  if (colour != "%^MAGENTA%^") {
                     ret += "%^BOLD%^%^MAGENTA%^";
                     colour = "%^MAGENTA%^";
                  }
                  ret += base->query_debug_map_char(sx + i, ny);
               } else {
                  new_base = find_adjacent_terrain(sx + i, sy - j);
                  if (new_base) {
                     if (colour != "%^RED%^") {
                        ret += "%^BOLD%^%^RED%^";
                        colour = "%^RED%^";
                     }
                     if (sx + i >= _width) {
                        // Assume same size.
                        ret += new_base->query_debug_map_char(sx + i - _width, ny);
                     } else {
                        ret += new_base->query_debug_map_char(sx + i + _width, ny);
                     }
                  } else {
                     ret += " ";
                  }
               }
            }
            ret += "%^RESET%^\n";
            colour = 0;
         }
      }
   }
   return ret;
}

/**
 * This method returns the specific debug map character at the
 * given location.
 * @param x the x location
 * @param y the y location
 * @return the map character
 */
string query_debug_map_char(int x, int y) {
   return _area_map[y][x..x];
}

/**
 * This method returns a grid of the specified size around the center
 * point to give creators an overview of where they are currently.
 * @param x the x co-ordinate
 * @param y the y co-ordinate
 * @param width the width of the box
 */
string query_debug_map_feature(string name) {
   int x;
   int y;
   class feature_data feature;
   mixed data;
   string ret;
   int colour;
   int found;
   //string direc;
   //int distance;

   feature = _features[name];
   ret = "";
   colour = 0;
   for (y = _height - 1; y >= 0; y--) {
      for (x = 0; x < _width; x++) {
         data = feature->feature_ob->query_feature_desc_from(x, y, 0, 1);
         found = sizeof(data);
         if (found) {
            if (!colour) {
               ret += "%^YELLOW%^";
               colour = 1;
            }
         } else {
            if (colour) {
               ret += "%^RESET%^";
               colour = 0;
            }
         }
         ret += _area_map[y][x..x];
      }
      ret += "%^RESET%^\n";
      colour = 0;
   }
   return ret;
}

/**
 * This method returns a grid of the specified size around the center
 * point to give creators an overview of where they are currently.
 * @param x the x co-ordinate
 * @param y the y co-ordinate
 * @param width the width of the box
 */
string query_debug_map_feature_distant(string feature) {
   int x;
   int y;
   mixed data;
   string ret;
   int colour;
   class coord fluff;

   ret = "";
   colour = 0;
   for (y = _height - 1; y >= 0; y--) {
      for (x = 0; x < _width; x++) {
         fluff = query_real_from_terrain_coord(x, y, 0);
         data = feature->query_feature_desc_from(fluff->x, fluff->y, fluff->z);
         if (data && sizeof(data)) {
            if (!colour) {
               ret += "%^YELLOW%^";
               colour = 1;
            }
         } else {
            if (colour) {
               ret += "%^RESET%^";
               colour = 0;
            }
         }
         ret += _area_map[y][x..x];
      }
      ret += "%^RESET%^\n";
      colour = 0;
   }
   return ret;
}

/**
 * This adds the new string to the key.
 */
private void add_key(mapping key, string char, string str) {
  if(strsrch(str, "intersection") != -1)
    return;
  
  if(!key[char])
    key[char] = str;
  else if(strsrch(key[char], str) == -1 ) {
    key[char] += ", " + str;
  }
}

/**
 * This updates the map and makes the data.
 */
private int update_map(mapping data, mixed* map, mapping key, mixed *colours,
                       int x, int y, int z, int offx, int offy) {
   class char_map_data info;
   string char, colour, tmp;
   string room;
   int block;
   object ob;
   
   if (x < 0) {
      if (y < 0) {
         data["-y-x"] = 1;
      } else if (y >= _height) {
         data["+y-x"] = 1;
      } else {
         data["-x"] = 1;
      }
      return 0;
   } else if (x >= _width) {
      if (y < 0) {
         data["-y+x"] = 1;
      } else if (y >= _height) {
         data["+y+x"] = 1;
      } else {
         data["+x"] = 1;
      }
      return 0;
   } else if (y < 0) {
      data["-y"] = 1;
      return 0;
   } else if (y >= _height) {
      data["+y"] = 1;
      return 0;
   }

   info = query_char_map(x, y);
   if (!info) {
      return 1;
   }
   if (info->type == AREA_OBSTACLE) {
      map[y - offy][x - offx] = info->char;
      colours[y - offy][x - offx] = info->colour;
      if(!info->colour) {
        colours[y - offy][x - offx] = "%^BLUE%^";
      }
      if (info->room_desc == "Obstacle" && stringp(info->items)) {
        add_key(key, info->char, info->items);
      } else if (info->room_desc != "") {
        add_key(key, info->char, info->room_desc);
      } else {
         map[y - offy][x - offx] = " ";
      }
      data["block" + info->char] = 1;
      return 1;
   } else {
      char = data[info->char];
      colour = data[info->colour];
      if (!char || !colour) {
         room = query_base_room(x, y, z, TERRAIN_MAP_ROOM_MAP);
         if (room) {
            ob = load_object(room);
            if (ob) {
               char = ob->query_terrain_map_character();
               colour = ob->query_terrain_map_colour();
               block = ob->query_terrain_map_block();

               if(char && ob) {
                  tmp = ob->query_key_desc();
                  if(!tmp) {
                     tmp = ob->query_short();
                  }
                  if(!tmp) {
                     debug_printf("Room %s has no short.", base_name(ob));
                  } else {
                     add_key(key, char, tmp);
                  }
               }

               if (block || info->height > z) {
                  data["block" + info->char] = 1;
               }
            } else {
               debug_printf("Unable to find %O\n", room);
            }
         }
         if (!char) {
            char = info->char;
         }
         if(!colour) {
            colour = info->colour;
         }

         data[info->char] = char;
      }
      map[y - offy][x - offx] = char;
      colours[y - offy][x - offx] = colour;
   }
   if(char && !key[char] && !data["nokey"+info->char]) {
      data["nokey" + info->char] = 1;
      debug_printf("No key for %s", char);
   }
   return data["block" + info->char];
}

/**
 * This method fills in the line with stuff if it is needed.
 * @param map the map to fill in
 * @param sx the startx
 * @param sy the starty
 * @param width the width of the map
 */
private string line_in_map(mapping data, mixed* map, mapping key,
                           mixed *colours, int offx, int offy,
                           int sx, int sy, int ex, int ey, int z) {
   int x;
   int y;
   int dx;
   int dy;
   int incx;
   int incy;
   int balance;

   if (ex >= sx) {
      dx = ex - sx;
      incx = 1;
   } else {
      dx = sx - ex;
      incx = -1;
   }

   if (ey >= sy) {
      dy = ey - sy;
      incy = 1;
   } else {
      dy = sy - ey;
      incy = -1;
   }

   x = sx;
   y = sy;
   if (dx >= dy) {
      dy <<= 1;
      balance = dy - dx;
      dx <<= 1;
      while (x != ex) {
         if (update_map(data, map, key, colours, x, y, z, offx, offy)) {
            break;
         }
         if (balance >= 0) {
            y += incy;
            balance -= dx;
         }
         balance += dy;
         x += incx;
      }
   } else {
      dx <<= 1;
      balance = dx - dy;
      dy <<= 1;
      while (y != ey) {
         if (update_map(data, map, key, colours, x, y, z, offx, offy)) {
            break;
         }
         if (balance >= 0) {
            x += incx;
            balance -= dy;
         }
         balance += dx;
         y += incy;
      }
   }
   // Last pixel...
   update_map(data, map, key, colours, x, y, z, offx, offy);
}

/**
 * Updates the blocking structures.
 */
private void update_blocking(mapping data,
                             mixed* map, mixed* colours, int x, int y,
                             int distance) {
   int dx;
   int dy;
   int adx;
   int ady;
   int dist;
   int nx;
   int ny;
   int tx;
   int ty;
   int incx;
   int incy;
   string* template;
   string str;

   dx = (x - distance);
   dy = (y - distance);

   //str = sprintf("%d", x % 10);
   str = " ";
   if (dx < 0) {
      adx = - dx;
      incx = -1;
   } else {
      adx = dx;
      incx = 1;
   }
   if (dy < 0) {
      ady = - dy;
      incy = -1;
   } else {
      ady = dy;
      incy = 1;
   }

   if (incx != incy) {
      incx = -incx;
      incy = - incy;
   }

   if (adx > ady) {
      template = TERRAIN_MAP_WORLD_MAP->query_blocking_template(ady, adx, 0);
   } else {
      template = TERRAIN_MAP_WORLD_MAP->query_blocking_template(adx, ady, 0);
   }
   // Error.
   if (!template) {
      debug_printf("Error with the template");
      return ;
   }
   dist = distance + 1;
   for (tx = 0; tx < dist; tx++) {
      for (ty = 0; ty < dist; ty++) {
         if (template[tx][ty] == '*') {
            // Zap the structures.
            if (adx > ady) {
               nx = distance + tx * incx;
               ny = distance + ty * incy;
            } else {
               nx = distance + ty * incx;
               ny = distance + tx * incy;
            }
            if (nx < sizeof(map) && ny < sizeof(map[nx]) &&
                nx >= 0 && ny >= 0) {
               map[nx][ny] = str;
               colours[nx][ny] = 0;
            }
            if (adx == ady) {
               nx = distance + tx * incx;
               ny = distance + ty * incy;
               map[nx][ny] = str;
               colours[nx][ny] = 0;
            } else if (adx == 0) {
               nx = distance + ty * incx;
               ny = distance - tx * incy;
               map[nx][ny] = str;
            } else if (ady == 0) {
               nx = distance - tx * incx;
               ny = distance + ty * incy;
               map[nx][ny] = str;
               colours[nx][ny] = 0;
            }
         }
      }
   }
}

/**
 * This method does the updating of the map based on the template.
 * Pretty easy really, check each location in the map to see if it
 * exists and what we should do with it.
 */
mapping query_player_map_internal_template(mixed* map, mapping key,
                                  mixed *colours, int x_c, int y_c, int z_c,
                                  int distance) {
   int offx;
   int offy;
   int x;
   int y;
   mapping data;


   data = ([ ]);
   offx = x_c - distance;
   offy = y_c - distance;
   // Go from the top corner to the bottom corner.
   for (x = 0; x < sizeof(map); x++) {
      for (y = 0; y < sizeof(map[x]); y++) {
         if (map[y][x] == 1) {
            // Update it.
            if (update_map(data, map, key, colours, x + offx, y + offy, z_c, offx, offy)) {
               // Need to update the blocking.
               // Check for diagonals and blocking.
               if (x > 0) {
                  if (y > 0 && data["block" + map[y-1][x-1]]) {
                     update_blocking(data, map, colours, x, y, distance);
                     update_blocking(data, map, colours, x, y-1, distance);
                     //map[y-1][x] = "#";
                  } else if (y < sizeof(map) - 1 && data["block" + map[y+1][x-1]]) {
                     update_blocking(data, map, colours, x, y, distance);
                     update_blocking(data, map, colours, x, y + 1, distance);
                     //map[y+1][x] = "#";
                  } else if (y > 0 && data["block" + map[y-1][x]]) {
                     update_blocking(data, map, colours, x, y, distance);
                     update_blocking(data, map, colours, x - 1, y - 1, distance);
                  } else {
                     update_blocking(data, map, colours, x, y, distance);
                  }
               } else {
                  update_blocking(data, map, colours, x, y, distance);
               }
            }
         }
      }
   }
   return data;
}

/**
 * This method generates a player map using templates instead of using
 * lines from the center point.  This is more efficent than a
 * line drawing method.  THe max_size is used to
 * make sure the size of the visible area is squashed, if we want a
 * smaller partial map for other displays.  The max size must be
 * an odd number.
 */
string query_player_map_template(int x_c, int y_c, int z_c, int visibility,
             int max_size) {
   string ret, item, base, current, *keys;
   mixed *map, *colours;
   mapping data, key;
   int i, j;
   int distance;
   //class char_map_data tmp;

   distance = _visible_distance * visibility / 100;
   if (!distance) {
      distance = 1;
   }
   data = ([ ]);
   key = ([ ]);
   map = ({ });
   colours = ({ });
   // Returns a template for the given visiblity distance.
   map = TERRAIN_MAP_WORLD_MAP->query_map_template(distance);
   colours = TERRAIN_MAP_WORLD_MAP->query_map_template(distance);

   // Slice the map down to size.
   if (max_size && (max_size % 2) != 1) {
      max_size--;
   }
   if (max_size && sizeof(map) > max_size) {
      j = (max_size - 1) / 2;
      map = map[distance - j..distance + j];
      colours = colours[distance - j..distance + j];
      for (i = 0; i < sizeof(map); i++) {
         map[i] = map[i][distance - j..distance + j];
         colours[i] = colours[i][distance - j..distance + j];
      }
      // Need to mess with x_c and y_c
      x_c += (distance - j);
      y_c += (distance - j);
   }

   data = query_player_map_internal_template(map, key, colours, x_c, y_c, z_c,
                                    distance);
   if (data["-y"]) {
     base = find_adjacent_terrain(x_c, -1);
     if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal_template(map, key, colours, x_c,
                                            y_c + _height, z_c, distance);
         }
      }
   }
   if (data["+y"]) {
      base = find_adjacent_terrain(x_c, _height + 1);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal_template(map, key, colours, x_c,
                                            y_c - _height, z_c, distance);
         }
      }
   }
   if (data["-x"]) {
      base = find_adjacent_terrain(-1, y_c);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal_template(map, key, colours, x_c + _width,
                                            y_c, z_c, distance);
         }
      }
   }
   if (data["+x"]) {
      base = find_adjacent_terrain(_width + 1, y_c);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal_template(map, key, colours, x_c - _width,
                                            y_c, z_c, distance);
         }
      }
   }
   if (data["-y+x"]) {
      base = find_adjacent_terrain(_width + 1, -1);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal_template(map, key, colours, x_c - _width,
                                            y_c + _height, z_c,
                                            distance);
         }
      }
   }
   if (data["+y+x"]) {
      base = find_adjacent_terrain(_width + 1, _height + 1);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal_template(map, key, colours, x_c - _width,
                                            y_c - _height, z_c,
                                            distance);
         }
      }
   }
   if (data["+y-x"]) {
      base = find_adjacent_terrain(-1, _height + 1);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal_template(map, key, colours, x_c + _width,
                                            y_c - _height, z_c,
                                            distance);
         }
      }
   }
   if (data["-y-x"]) {
      base = find_adjacent_terrain(-1, -1);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal_template(map, key, colours, x_c + _width,
                                            y_c + _height, z_c,
                                            distance);
         }
      }
   }
   
   map[sizeof(map) / 2][sizeof(map) / 2] = "@";

   if(!this_player()->query_property(PLAYER_PLAIN_MAPS)) {
     colours[sizeof(map) / 2][sizeof(map) / 2] = "%^YELLOW%^";
     for(i=0; i<sizeof(map); i++) {
       current = "";
       for(j=0; j<sizeof(map); j++) {
         if(map[i][j] == "\r" || !stringp(map[i][j])) {
           map[i][j] = " ";
         }
         
         if (!current || current == " ") {
           current = "";
         }
         if(!colours[i][j] || colours[i][j] == " ") {
           colours[i][j] = "";
         }
         
         if(colours[i][j] != current) {
           if (colours[i][j] == "" || !stringp(colours[i][j])) {
             if(current != "" && current != " ") {
               map[i][j] = "%^RESET%^" + map[i][j];
               current = "";
             }
           } else if (colours[i][j] != "\r") {
             map[i][j] = colours[i][j] + map[i][j];
             current = colours[i][j];
           }
         }
         //map[i][j] = "[" + map[i][j] + "]";
         if(i == x_c && j == y_c && current != "") {
           map[i][j] += current;
         }
       }
       if(current) {
         map[i][j-1] += "%^RESET%^";
       }
     }
   }

   //ret = replace(ret, "%^", "");
   if(_key && !max_size) {
     keys = ({ });
     foreach(item in keys(key)) {
       keys += map(explode(sprintf("%s %-=40s", item, key[item]), "\n"),
                   (: "     " + $1 :));
     }
   }
   
   ret = "";
   for(i = sizeof(map) - 1; i >= 0; i--) {
     ret += implode(map(map[i], (: stringp($1) ? $1 : " " :)), "");
     if(!max_size && sizeof(map)-2-i>= 0 && sizeof(map)-2-i < sizeof(keys)) {
       ret += keys[sizeof(map)-2-i];
     }
     ret += "\n";
   }
   
   return ret;
}

/**
 * This updates the player map based on the input.
 * @ignore yes
 */
mapping query_player_map_internal(mixed* map, mapping key, mixed *colours,
                                  int x_c, int y_c, int z_c, int distance) {
   int x, y, e, u, v;
   int endx, endy, offx, offy;
   mapping data;

   data = ([ ]);

   offx = x_c - distance;
   offy = y_c - distance;
   x = 0;
   y = distance;
   u = 1;
   v = 2 * distance - 1;
   e = 0;

   while (x < y) {
      // point 1.
      endx = x_c + x;
      endy = y_c + y;
      line_in_map(data, map, key, colours, offx, offy, x_c, y_c, endx, endy, z_c);
      // point 2.
      endx = x_c + y;
      endy = y_c - x;
      line_in_map(data, map, key, colours, offx, offy, x_c, y_c, endx, endy, z_c);
      // point 3.
      endx = x_c - x;
      endy = y_c - y;
      line_in_map(data, map, key, colours, offx, offy, x_c, y_c, endx, endy, z_c);
      // point 4.
      endx = x_c - y;
      endy = y_c + x;
      line_in_map(data, map, key, colours, offx, offy, x_c, y_c, endx, endy, z_c);
      x++;
      e += u;
      u += 2;
      if (v < 2 * e) {
         y--;
         e -= v;
         v -= 2;
      }
      if (x > y) {
         break;
      }
      // point 5.
      endx = x_c + y;
      endy = y_c + x;
      line_in_map(data, map, key, colours, offx, offy, x_c, y_c, endx, endy, z_c);
      // point 6.
      endx = x_c + x;
      endy = y_c - y;
      line_in_map(data, map, key, colours, offx, offy, x_c, y_c, endx, endy, z_c);
      // point 7.
      endx = x_c - y;
      endy = y_c - x;
      line_in_map(data, map, key, colours, offx, offy, x_c, y_c, endx, endy, z_c);
      // point 8.
      endx = x_c - x;
      endy = y_c + y;
      line_in_map(data, map, key, colours, offx, offy, x_c, y_c, endx, endy, z_c);
   }

   // The algorithm for a circle misses a few so we'll fill them in here.
   // The "gap" rooms always have filled-in locations on either side so
   // test for that condition and fill-in as necessary.
   for(y=0; y<sizeof(map); y++)
     for(x=1; x<sizeof(map[y])-1; x++)
       if(map[y][x] == "\r" && map[y][x-1] != "\r" && map[y][x+1] != "\r") {
         update_map(data, map, key, colours, x + offx, y + offy, z_c, offx,
                    offy);
       }

   return data;
}

/**
 * This method returns the player line of sight view of the current terrain.
 * It depends on height and other wonderful things.
 * @param x_c the x center point
 * @param y_c the y center point
 * @param z_c the z center point
 * @param visibility the visibility percentage currently
 */
string query_player_map(int x_c, int y_c, int z_c, int visibility) {
   string ret, item, base, current, *keys;
   mixed *map, *colours;
   mapping data, key, colour_key;
   int i, j;
   int distance;
   //class char_map_data tmp;

   distance = _visible_distance * visibility / 100;
   if (!distance) {
      distance = 1;
   }
   data = ([ ]);
   key = ([ ]);
   map = ({ });
   colours = ({ });
   for (i = 0; i < distance * 2 + 1; i++) {
      map += ({ allocate(distance * 2 + 1, "\r") });
      colours += ({ allocate(distance * 2 + 1, "\r") });
      
   }

   data = query_player_map_internal(map, key, colours, x_c, y_c, z_c,
                                    distance);
   if (data["-y"]) {
     base = find_adjacent_terrain(x_c, -1);
     if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal(map, key, colours, x_c,
                                            y_c + _height, z_c, distance);
         }
      }
   }
   if (data["+y"]) {
      base = find_adjacent_terrain(x_c, _height + 1);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal(map, key, colours, x_c,
                                            y_c - _height, z_c, distance);
         }
      }
   }
   if (data["-x"]) {
      base = find_adjacent_terrain(-1, y_c);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal(map, key, colours, x_c + _width,
                                            y_c, z_c, distance);
         }
      }
   }
   if (data["+x"]) {
      base = find_adjacent_terrain(_width + 1, y_c);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal(map, key, colours, x_c - _width,
                                            y_c, z_c, distance);
         }
      }
   }
   if (data["-y+x"]) {
      base = find_adjacent_terrain(_width + 1, -1);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal(map, key, colours, x_c - _width,
                                            y_c + _height, z_c,
                                            distance);
         }
      }
   }
   if (data["+y+x"]) {
      base = find_adjacent_terrain(_width + 1, _height + 1);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal(map, key, colours, x_c - _width,
                                            y_c - _height, z_c,
                                            distance);
         }
      }
   }
   if (data["+y-x"]) {
      base = find_adjacent_terrain(-1, _height + 1);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal(map, key, colours, x_c + _width,
                                            y_c - _height, z_c,
                                            distance);
         }
      }
   }
   if (data["-y-x"]) {
      base = find_adjacent_terrain(-1, -1);
      if (base) {
         if (!load_object(base)) {
            debug_printf("Unable to load %O\n", base);
         } else {
            base->query_player_map_internal(map, key, colours, x_c + _width,
                                            y_c + _height, z_c,
                                            distance);
         }
      }
   }
   
   y_c -= y_c - distance;
   x_c -= x_c - distance;

   if(!this_player()->query_property(PLAYER_PLAIN_MAPS)) {
     colour_key = ([ ]);
     map[y_c][x_c] = "%^YELLOW%^@%^RESET%^";

     // This is going to go through the 2d map array and put in colour and
     // reset codes as necessary.
     for(i=0; i<sizeof(map); i++) {
       current = "";
       for(j=0; j<sizeof(map); j++) {
         if(map[i][j] == "\r")
           map[i][j] = " ";
         
         if(!current || current == " ") current = "";
         if(!colours[i][j] || colours[i][j] == " ") colours[i][j] = "";

         // Make a colour coded map.
         if(key[map[i][j]])
           colour_key[colours[i][j] + map[i][j] + "%^RESET%^"] =
             key[map[i][j]]; 

         if(colours[i][j] != current) {
           if(colours[i][j] == "") {
             if(current != "" && current != " ") {
               map[i][j] = "%^RESET%^" + map[i][j];
               current = "";
             }
           } else if(colours[i][j] != "\r") {
             map[i][j] = colours[i][j] + map[i][j];
             current = colours[i][j];
           }
         }
         
         //map[i][j] = "[" + map[i][j] + "]";
         if(i == x_c && j == y_c && current != "")
           map[i][j] += current;
       }
       if(current)
         map[i][j-1] += "%^RESET%^";
     }
   } else {
     map[y_c][x_c] = "@";
     colour_key = key;
   }
   
   if(_key) {
     keys = ({ });
     foreach(item in keys(colour_key)) {
       keys += map(explode(sprintf("%s %-=40s", item, colour_key[item]), "\n"),
                           (: "     " + $1 :));
     }
   }

   ret = "";
   for(i = sizeof(map) - 1; i >= 0; i--) {
     ret += implode(map[i], "");
     if(sizeof(map)-2-i>= 0 && sizeof(map)-2-i < sizeof(keys))
       ret += keys[sizeof(map)-2-i];
     ret += "\n";
   }

   // This is normally done in the colour section.
   if(this_player()->query_property(PLAYER_PLAIN_MAPS))
     ret = replace_string(ret, "\r", " ");
   
   return ret;
}

/**
 * This is used by the terrain system to work out what is beside
 * us.
 * @param x the x position
 * @param y the y position
 */
class char_map_data query_char_map_real_coord(int x, int y)
{
   class coord our;

   our = query_terrain_from_real_coord(x, y, 0);
   return query_char_map(our->x, our->y);
}

/**
* This method is used to find all rooms currently loaded within the
* specified room zone.
* @param zone the name of the room zone
* @return an array containing all the currently cloned rooms within this zone
* @see query_rooms_in_block
* @see query_rooms_in_range
*/
object *query_rooms_in_zone(string zone)

{
   if (_room_zones[zone]) {
      _room_zones[zone] -= ({ 0 });
      return _room_zones[zone];
   }
   return ({ });
}


/**
* This method displays a message in all the currently loaded rooms
* within the specified zone(s).
* @param zones a zone name as a string, or an array of zone names
* @param message the message to be displayed
* @param mess_class the message type. Normally this need not be specified
* @see query_rooms_in_zone
* @see tell_feature
*/
varargs void tell_zones(mixed zones, string message, mixed mess_class)

{
   object  *rooms = ({ });
   string  zone;

   if (!mess_class)
      mess_class = "zone_shout";

   if (stringp(zones))
      zones = ({ zones });

   foreach (zone in zones)
      rooms += query_rooms_in_zone(zone);

   if (sizeof(rooms))
      message(mess_class, message, rooms);
}


/**
* This message displays messages in all rooms within range of a feature.
* The message can be either a string, which goes to all the rooms, or an
* array of strings, in which case they get spread across the distances (in
* which case you should supply the same number of array elements as in the
* descriptions in the original add_feature. Any occurences of $D in the
* message(s) will be replaced in each room with the direction that the
* feature lies in.
* @param title the name of the feature, as used in add_feature
* @param message the message to be displayed
* @see tell_rooms_in_zone
* @see add_feature
* @see modify_feature
* @see set_feature_chats
*/
varargs void tell_feature(string title, mixed message)

{
   int     index;
   string  mess_class = sprintf("feature:%s", title);

   if (!_feature_rooms[title]) {
      return;
   }

   _feature_rooms[title] -= ({ 0 });
   if (!sizeof(_feature_rooms[title])) {
      return;
   }

   if (stringp(message)) {
      message(mess_class, message, _feature_rooms[title]);
   }

   if (arrayp(message)) {
      for (index = 0; index < sizeof(message); index++) {
         message(sprintf("%s:%d", mess_class, index), message[index],
         _feature_rooms[title]);
      }
   }
}


// Called by rooms to get the feature description that applies to them.
// It'll do direction substitution if it's supplied with a direc to subs.
/**
* @ignore
*/
varargs string* query_distant_feature_desc(string title, mapping direcs,
                                           int visibility) {
   int       night = (WEATHER->query_tod() == "night");
   string str;

   str = title->calc_feature_desc(direcs, night, visibility);
   if (str) {
      return ({ str });
   }
   return ({ });
}


// Called by rooms to get the item desc of a feature.
// It'll do direction substitution if it's supplied with a direc to subs.
/**
* @ignore
*/
varargs string* query_feature_desc(string title, mapping direcs,
                                   int visibility) {
   class feature_data   feature;
   string*   item_desc;
   string*   dirs;
   int       night = (WEATHER->query_tod() == "night");
   string    direc;
   int       range;
   mapping   bits;
   int       index;
   string    str;
   int       new_range;

   if (!feature) {
      feature = _features[title];
   }

   if (!feature) {
      return ({ });
   }

   if( visibility == 0 ) {
      return ({ });
   }

   if (!sizeof(feature->descs)) {
      str = feature->feature_ob->calc_feature_desc(direcs, night,
                                                       visibility);
      if (str) {
         return ({ str });
      }
      return ({ });
   }

   bits = ([ ]);
   foreach (direc, range in direcs) {
      if (night) {
         item_desc = feature->night_descs;
      } else {
         item_desc = feature->descs;
      }
      if (!item_desc) {
         debug_printf("Bad night description for %O", title);
         continue;
      }
      new_range = range * 100 / visibility;
      // Make sure the modified range is less the visibility
      if (feature->range > new_range && sizeof(item_desc)) {
         index = sizeof(item_desc) * new_range / feature->range;
         str = item_desc[index];
         if (str && str != "") {
            if (!bits[str]) {
               bits[str] = ({ });
            }
            bits[str] += ({ direc });
         }
      }
   }

   item_desc = ({ });
   foreach (str, dirs in bits) {
      item_desc += ({ replace_string(str, "$D", query_multiple_short(dirs)) });
   }

   return item_desc;
}

/**
 * This method returns the item description.
 * @param title the title of the feature
 * @return the feature item description
 */
varargs string query_feature_item_desc(string title) {
   class feature_data   feature;
   mixed   item_desc;
   int       night = (WEATHER->query_tod() == "night");

   if (!feature) {
      feature = _features[title];
   }

   if (!feature) {
      return "";
   }

   if (night) {
      item_desc = feature->f_item_night_desc;
   } else {
      item_desc = feature->f_item_desc;
   }

   if (!pointerp(item_desc)) {
      return item_desc;
   }
   return item_desc[0];
}


// Called by rooms when chatting, to get feature chats.
/**
* @ignore
*/
varargs string get_a_feature_chat(string title, int range, string direc)

{
   class feature_data   feature;
   mixed     chat;
   int       night = (WEATHER->query_tod() == "night");

   if (!feature) {
      feature = _features[title];
   }

   if (!feature) {
      return "";
   }

   if (night) {
      chat = feature->night_chats;
   } else {
      chat = feature->chats;
   }

   if (!chat) {
      return "";
   }

   if (arrayp(chat)) {
      chat = chat[random(sizeof(chat))];
   }

   if (arrayp(chat)) { // ranged chat
      if ((range < 0) || (range >= sizeof(chat))) {
         return "";
      }
      chat = chat[range];
   }
   if (!stringp(chat)) {// Something's wrong.
      return "";
   }

   if (direc) {
      chat = replace(chat, ({ "$D", direc }));
   }

   return chat;
}




// Called by rooms when chatting, to get outside chats.
/**
* @ignore
*/
string get_an_outside_chat(string types)

{
   class char_map_data   item;
   string    *chats = ({ });
   int       index;
   mixed     *item_chats;

   for (index = 0; index < sizeof(types); index++) {
      item = _char_map[types[index..index]];
      if (!item) {
         continue;
      }

      item_chats = item->chats;
      if (!item_chats) {
         continue;
      }

      if (stringp(item_chats))
         chats += ({ item_chats });
      else if (arrayp(item_chats))
         chats += item_chats;
   }
   if (!sizeof(chats))
      return "";

   return chats[random(sizeof(chats))];
}


/**
* This method is used to add an 'obstacle' to an area map. An obstacle is
* effectively an area on the map which cannot be entered, and can be seen
* from adjacent locations. Any occurences of $D in the description will
* be replaced with the direction(s) in which the obstacle can be seen.
* An add_item is automatically performed to allow for consistency.
* @param type the ASCII type used on the map to denote the obstacle
* @param description the description of the obstacle, as seen from
* neighbouring rooms
* @param items a string, or array of strings giving the corresponding
* add_item name(s) (identical to the 'shorts' parameter in add_item)
* @param item_desc a string, or array of strings giving the corresponding
* @param items add_item name(s) (identical to the 'shorts' parameter in add_item)
* @param item_desc a string, or array of strings giving the corresponding item desc
* @param room_desc allows a description for the obstacle in the key
* @example
*   add_obstacle("*", "Thick green bushes block your way to the $D.",
                      ({ "bush", "bushes" }), "Thick and green.", "Thick bushes");
* @see add_room_type
* @see add_road_type
* @see set_external_chats
* @see add_item
*/
varargs void add_obstacle_type(string type, string description, mixed items, mixed item_desc, string room_desc)

{
   if (!type) {
      throw("Bad type");
   }
   if (!items) {
      items = ({ });
      item_desc = ({ });
   }
   if (!room_desc) {
      room_desc = "Obstacle";
   }
   _char_map[type] = new(class char_map_data,
                         type: AREA_OBSTACLE, adjacent_desc:description,
                         room_desc : room_desc,
                         items:items, item_desc:item_desc);
}


/**
* This method is used to add a standard skip room type to the terrain map.
* Mapped to an ASCII character as used in the ASCII room map.  A skipped
* room is one in which every nth room in the terrain will be skipped
* unless it has something interesting in it.  The height is used to
* do specific sorts of checks, like if a specific direction is passable or not.
* @param type the ASCII type used on the map to denote the room
* @param base_path the full pathname of the source file for the room
* @param size the number of rooms in each clump to skip over
* @param road_skip if this is 1 then the skip type is a 'road skip', this
* will assume the path is one wide and try to follow around curves
* @param height the height of the room
* @param height_rooms is a mapping containing what rooms to use at different height levels
* @example
*    add_room_type(".", ROOMS + "field");
* @see add_room_type
* @see add_road_type
* @see add_obstacle_type
*/
void add_special_type(string type, string base_path, int size, int road_skip, int height,
                      mapping height_rooms)

{
   _char_map[type] = new(class char_map_data,
                         type: AREA_ROOM, path:base_path,
                         room_jump : size,
                         room_jump_matching : road_skip,
                         height : height,
                         height_rooms : height_rooms);
}

/**
* This method is used to add a standard room type to the terrain map,
* mapped to an ASCII character as used in the ASCII room map.
* @param type the ASCII type used on the map to denote the room
* @param base_path the full pathname of the source file for the room
* @example
*    add_room_type(".", ROOMS + "field");
* @see add_room_type
* @see add_road_type
* @see add_obstacle_type
*/
void add_room_type(string type, string base_path)

{
   _char_map[type] = new(class char_map_data,
                         type: AREA_ROOM, path:base_path);
}


/**
* This method is used to add a standard road type to the terrain map,
* mapped to an ASCII character as used in the ASCII room map.
* A road is a terrain map room which can be seen from adjacent locations.
* The handler uses 'room_desc' to add an entry to the room description,
* describing where the road goes. 'description' is added to the
* descriptions of adjacent locations, to enable the road to be seen.
* In 'room_desc' and 'description', $D will be expanded to the relevant
* directions.
* @param type the ASCII type used on the map to denote the room
* @param base_path the full pathname of the source file for the room
* @param room_desc the 'continuation' description, which added to the room description to show where the road leads
* @param description the 'adjacent' description, which is seen from adjacent locations.
* @param items add_item name(s) (identical to the 'shorts' parameter in add_item)
* @param item_desc a string, or array of strings giving the corresponding item desc
* @example
*    add_road_type("+", ROOMS + "path", "The dusty path leads $D.",
*                  "There is a dusty path to the $D.",
*                  "path", "Hey! Look! A path!");
* @see add_room_type
* @see add_road_type
* @see set_external_chats
* @see add_item
*/
varargs void add_road_type(string type, string base_path, string room_desc,
              string description, mixed items, mixed item_desc)

{
   _char_map[type] = new(class char_map_data,
                        type: AREA_ROAD, adjacent_desc:description,
                        items:items, item_desc:item_desc,
                        room_desc:room_desc, path:base_path);
}

/**
* This method is used to add a world map based file into the terrain.
* It handles all the options like a special road type, but queries all the
* details off the base room instead.
* @param type the ASCII type used on the map to denote the room
* @param base_path the full pathname of the source file for the room
* @example
*    add_world_room_type("+", ROOMS + "path", "The dusty path leads $D.",
*                  "There is a dusty path to the $D.",
*                  "path", "Hey! Look! A path!");
* @see add_room_type
* @see add_road_type
* @see set_external_chats
* @see add_item
*/
varargs void add_world_room_type(string type, string base_path) {
   mapping bits;
   string *tmp;

   // If the world type doesn't exist see if we can find a more
   // generic version instead.
   if(file_size(base_path + ".c") == -1) {
     tmp = explode(base_path, "_edge");
     if(sizeof(tmp) && file_size(tmp[0] + ".c") > 0) {
       debug_printf("%s doesn't exist, using %s",
                    base_path, tmp[0]);
       base_path = tmp[0];
     }
   }     
   
   if (file_size(base_path + ".c") > 0) {
      bits = base_path->query_adjacent_items();
      if (!mapp(bits)) {
         debug_printf("Incorrect room type %O", type);
      } else {
         _char_map[type] = new(class char_map_data,
                        type: AREA_WORLD,
                        path: base_path,
                        adjacent_desc:base_path->query_adjacent_description(),
                        items:keys(bits),
                        item_desc:values(bits),
                        //room_desc:room_desc,
                         room_jump : base_path->query_room_jump_size(),
                         height : base_path->query_default_height(),
                         room_jump_matching : base_path->query_follow_road());
      }
   } else {
      debug_printf("Unable to add %O since %O doesn't exist.", type, base_path);
   }
}

/**
* This method is used to add a standard road type to the terrain map,
* mapped to an ASCII character as used in the ASCII room map.
* A road is a terrain map room which can be seen from adjacent locations.
* The handler uses 'room_desc' to add an entry to the room description,
* describing where the road goes. 'description' is added to the
* descriptions of adjacent locations, to enable the road to be seen.
* In 'room_desc' and 'description', $D will be expanded to the relevant
* directions.
* A skipped
* room is one in which every nth room in the terrain will be skipped
* unless it has something interesting in it.
* @param type the ASCII type used on the map to denote the room
* @param base_path the full pathname of the source file for the room
* @param size the number of rooms in each clump to skip over
* @param road_skip if this is 1 then the skip type is a 'road skip', this
* will assume the path is one wide and try to follow around curves
* @param height the height of the room
* @param room_desc the 'continuation' description, which added to the room description to show where the road leads
* @param description the 'adjacent' description, which is seen from adjacent locations.
* @param items add_item name(s) (identical to the 'shorts' parameter in add_item)
* @param item_desc a string, or array of strings giving the corresponding item desc
* @param height_rooms is a mapping containing what rooms to use at different height levels
* @example
*    add_road_type("+", ROOMS + "path", "The dusty path leads $D.",
*                  "There is a dusty path to the $D.",
*                  "path", "Hey! Look! A path!");
* @see add_room_type
* @see add_road_type
* @see set_external_chats
* @see add_item
*/
varargs void add_road_special_type(string type, string base_path,
              int size, int road_skip, int height,
              string room_desc,
              string description, mixed items, mixed item_desc,
              mapping height_rooms)

{
   _char_map[type] = new(class char_map_data,
                        type: AREA_ROAD, adjacent_desc:description,
                        items:items, item_desc:item_desc,
                        room_desc:room_desc, path:base_path,
                         room_jump : size,
                         height : height,
                         height_rooms : height_rooms,
                         room_jump_matching : road_skip);
}


/**
* This method is used to add a standard exit to a specified location
* in a terrain map. The chief use is to insert exits to standard rooms
* or other terrain maps.
* @param x the x coordinate of the room (internal coordinate system)
* @param y the y coordinate of the room (internal coordinate system)
* @param direc the direction of the exit (as in add_exit)
* @param dest the path of the exit's destination (as in add_exit)
* @param type the type of exit (as in add_exit)
* @param modifiers an optional array of exit modifiers (as in modify_exit)
* @example
*    add_exit_at(8, 0, "south", PEARWOOD + "stream_entrance", "road",
*                       ({ "move mess", "You wade along the stream bed.\n",
*                          "enter mess", "$N wades in from the north.",
*                          "exit mess", "$N wades off to the $T." }));
*
* @see add_room_type
* @see add_road_type
* @see add_obstacle_type
* @see add_exit
* @see modify_exit
*/
varargs void add_exit_at(int x, int y, string direc, string dest,
                         string type, mixed modifiers)

{
   string   title = sprintf("%d:%d:%d", x, y, 0);

   if (!dest)
      return;
   if (!type)
      type = "road";

   if (!_standard_exits[title])
      _standard_exits[title] = ({ });

   _standard_exits[title] += ({ ([ "direc":direc, "dest":dest,
                                   "type":type, "modifiers":modifiers ]) });
}

/**
* This method is used to add a standard exit to a specified location
* in a terrain map. The chief use is to insert exits to standard rooms
* or other terrain maps.
* @param x the x coordinate of the room (internal coordinate system)
* @param y the y coordinate of the room (internal coordinate system)
* @param direc the direction of the exit (as in add_exit)
* @param dest the path of the exit's destination (as in add_exit)
* @param type the type of exit (as in add_exit)
* @param modifiers an optional array of exit modifiers (as in modify_exit)
* @example
*    add_exit_at(8, 0, "south", PEARWOOD + "stream_entrance", "road",
*                       ({ "move mess", "You wade along the stream bed.\n",
*                          "enter mess", "$N wades in from the north.",
*                          "exit mess", "$N wades off to the $T." }));
*
* @see add_room_type
* @see add_road_type
* @see add_obstacle_type
* @see add_exit
* @see modify_exit
*/
varargs void add_z_exit_at(int x, int y, int z, string direc, string dest,
                         string type, mixed modifiers)

{
   string   title = sprintf("%d:%d:%d", x, y, z);

   if (!dest)
      return;
   if (!type)
      type = "road";

   if (!_standard_exits[title])
      _standard_exits[title] = ({ });

   _standard_exits[title] += ({ ([ "direc":direc, "dest":dest,
                                   "type":type, "modifiers":modifiers ]) });
}


/**
* This method is used to limit the number of random descs that are
* picked out for each room.
* @param new_limit the new desc limit
* @see set_random_chat_limit
* @see add_random_desc
* @see add_random_chat
*/
void set_random_desc_limit(int new_limit)

{
   _random_desc_limit = new_limit;
}


/**
* This method is used to limit the number of random chats that are
* picked out for each room.
* @param new_limit the new chat limit
* @see set_random_desc_limit
* @see add_random_desc
* @see add_random_chat
*/
void set_random_chat_limit(int new_limit)

{
   _random_chat_limit = new_limit;
}


/**
* This method is to add to the list of random descriptions that can be added to
* specific room types. A specific random sequence is used so that the randoms
* won't vary over time.
* @param types the string of ASCII character type(s) that this desc could apply to
* @param items add_item name(s) (identical to the 'shorts' parameter in add_item)
* @param item_desc a string, or array of strings giving the corresponding item desc
* @see set_random_desc_limit
* @see set_random_chat_limit
* @see add_random_chat
* @see add_item
*/
void add_random_desc(string types, string description, mixed items, mixed item_desc)

{
   int   index;

   for (index = 0; index < sizeof(types); index++) {
      if (!_random_descs[types[index..index]])
         _random_descs[types[index..index]] = ({ });

      _random_descs[types[index..index]] += ({ ([ "desc": description,
                               "items":items, "item_desc":item_desc ]) });

   }

}


/**
* This method is to add to the list of random chats that can be added to
* specific room types. A specific random sequence is used so that the randoms
* won't vary over time.
* @param types the string of ASCII character type(s) that this desc could apply to
* @param chats an array of chats to add. Usual add_chat format.
* @see set_random_desc_limit
* @see set_random_chat_limit
* @see add_random_chat
*/
void add_random_chat(string types, string *chats)

{
   int   index;

   for (index = 0; index < sizeof(types); index++) {
      if (!_random_chats[types[index..index]])
         _random_chats[types[index..index]] = ({ });
      _random_chats[types[index..index]] += chats;
   }

}


/**
* This method is used to setup 'external' chats for a road or obstacle,
* which will be added to the room chats of adjacent locations. You
* should supply the ASCII character corresponding to the road/obstacle
* type, and an array of chat strings (in the same format as supplied to
* room_chat)
* @param type the ASCII type used on the map to denote the room
* @param base_path the full pathname of the source file for the room
*    set_external_chats("=",
*                 ({ "The stream bubbles merrily on its way.",
*                    "You hear a faint splashing sound from the stream." }));
* @see add_road_type
* @see add_obstacle_type
* @see set_feature_chats
* @see room_chat
*/
void set_external_chats(string type, mixed chats)

{
   if (!_char_map[type]) {
      return;
   }

   ((class char_map_data)_char_map[type])->chats = chats;
}


/**
* This method is called to add a new 'feature' to a terrain map.
* Features have no real existance, but add atmosphere to a whole area,
* by being visible across a large number of locations.
* You define location, size, item desc, and add an array of 'longs'
* sorted by distance, plus the usual item descriptions. Any occurrences
* of $D in the longs array will be replaced by the direction in which
* the feature lies.
* @param title the name of the feature
* @param x the x coordinate of the room (internal coordinate system)
* @param y the y coordinate of the room (internal coordinate system)
* @param width the width of the feature
* @param height the height of the feature
* @param range the distance (number of rooms) it can be seen for
* @param descs array of descriptions to be shown at different ranges.
* These are divided equally along the total range
* @param items add_item name(s) (identical to the 'shorts' parameter in add_item)
* @param item_desc a string, or array of strings giving the corresponding
* @param night_descs the night description array
* @param night_items the descriptions for the items at night
* @example
*   add_feature("northmountain", 3, 0, 5, 1, 7,
*            ({ "A huge snowy mountain towers over you to the $D.",
*               "A huge snowy mountain looms to the $D.",
*               "A fair way off to the $D, you can see a huge snowy mountain.",
*               "A long way $Dwards, you glimpse the snowy peak of a "
*               "mountain." }),
*               "mountain", "From here, it looks quite astoundingly huge.",
*            ({ "A huge black bulk obscures the sky to the $D.",
*               "A huge black bulk covers the horizon to the $D.",
*               "A black lump obscures the horizon to the $D",
*               "" }),
*               "The dark black silloette looks like a mountain.");
* @see add_road_type
* @see add_obstacle_type
* @see set_feature_chats
* @see modify_feature
* @see add_item
*/
void add_feature(string title, int x, int y, int width, int height,
                 int range, string *descs, mixed items, mixed item_desc,
                 string* night_descs,
                 mixed night_items)

{
   object region;
   object feature;

   region = clone_object(TERRAIN_MAP_REGION_RECTANGULAR);
   region->add_feature_position(x, y, width, height);
   feature = clone_object(TERRAIN_MAP_FEATURE_BASE);
   feature->set_region(region);
   feature->set_max_day_range(range);
   feature->set_max_night_range(range);
   _features[title] = new(class feature_data,
                         range:range, descs:descs,
                         f_items:items, f_item_desc:item_desc,
                         f_item_night_desc : night_items,
                         whole_sentance:1,
                         feature_ob : feature,
                         night_descs : night_descs);
}

/**
* This method is called to add a new 'feature' to a terrain map.
* Features have no real existance, but add atmosphere to a whole area,
* by being visible across a large number of locations.  This method is
* different to the normal add_feature in that the system will try
* and construct a sentance from the features instead of once sentance
* per feature.
* You define location, size, item desc, and add an array of 'longs'
* sorted by distance, plus the usual item descriptions. Any occurrences
* of $D in the longs array will be replaced by the direction in which
* the feature lies.
* @param title the name of the feature
* @param x the x coordinate of the room (internal coordinate system)
* @param y the y coordinate of the room (internal coordinate system)
* @param width the width of the feature
* @param height the height of the feature
* @param range the distance (number of rooms) it can be seen for
* @param descs array of descriptions to be shown at different ranges.
* @param items add_item name(s) (identical to the 'shorts' parameter in add_item)
* @param item_desc a string, or array of strings giving the corresponding
* These are divided equally along the total range
* @example
*   add_feature_sentance("northmountain", 3, 0, 5, 1, 7,
*            ({ "a huge snowy mountain towers over you to the $D",
*               "a huge snowy mountain looms to the $D",
*               "a fair way off to the $D, you can see a huge snowy mountain",
*               "a long way $Dwards, you glimpse the snowy peak of a "
*               "mountain" }),
*               "mountain", "From here, it looks quite astoundingly huge." );
* @see add_road_type
* @see add_obstacle_type
* @see set_feature_chats
* @see modify_feature
* @see add_item
*/
void add_feature_sentance(string title, int x, int y, int width, int height,
                 int range, string *descs, mixed items, mixed item_desc,
                 string* night_descs, mixed night_items)

{
   object region;
   object feature;

   region = clone_object(TERRAIN_MAP_REGION_RECTANGULAR);
   region->add_feature_position(x, y, width, height);
   feature = clone_object(TERRAIN_MAP_FEATURE_BASE);
   feature->set_region(region);
   feature->set_max_day_range(range);
   feature->set_max_night_range(range);
   _features[title] = new(class feature_data,
                         feature_ob : feature,
                         range:range, descs:descs,
                         f_items:items, f_item_desc:item_desc,
                         f_item_night_desc : night_items,
                         whole_sentance:0,
                         night_descs : night_descs);
}


/**
* This method is called to add a new 'feature' to a terrain map.
* Features have no real existance, but add atmosphere to a whole area,
* by being visible across a large number of locations.
* You define location, size, item desc, and add an array of 'longs'
* sorted by distance, plus the usual item descriptions. Any occurrences
* of $D in the longs array will be replaced by the direction in which
* the feature lies.
* @param title the name of the feature
* @param region the region object to use in calculating the feature
* @param range the distance (number of rooms) it can be seen for
* @param descs array of descriptions to be shown at different ranges.
* These are divided equally along the total range
* @param items add_item name(s) (identical to the 'shorts' parameter in add_item)
* @param item_desc a string, or array of strings giving the corresponding
* @param whole_sentance use a whole sentance for the descriptions or wrap
* them up into fragments
* @example
*   region = clone_object(TERRAIN_MAP_REGION_POLYGON);
*   region->add_vertex(12, 13);
*   region->add_vertex(14,15);
*   region->add_vertex(17,18);
*   add_feature_region("northmountain", region, 7,
*            ({ "A huge snowy mountain towers over you to the $D.",
*               "A huge snowy mountain looms to the $D.",
*               "A fair way off to the $D, you can see a huge snowy mountain.",
*               "A long way $Dwards, you glimpse the snowy peak of a "
*               "mountain." }),
*               "mountain", "From here, it looks quite astoundingly huge.",
*            ({ }), 0 );
* @see add_road_type
* @see add_obstacle_type
* @see set_feature_chats
* @see modify_feature
* @see add_item
*/
void add_feature_region(string title, object region,
                 int range, string *descs, mixed items, mixed item_desc,
                 string* night_descs,
                 mixed night_items,
                 int whole_sentance)

{
   object feature;

   feature = clone_object(TERRAIN_MAP_FEATURE_BASE);
   feature->set_region(region);
   feature->set_max_day_range(range);
   feature->set_max_night_range(range);
   _features[title] = new(class feature_data,
                         range:range, descs:descs,
                         f_items:items, f_item_desc:item_desc,
                         whole_sentance:whole_sentance,
                         f_item_night_desc : night_items,
                         feature_ob : feature,
                         night_descs : night_descs);
}

/**
* This method is called to add a new 'feature' to a terrain map.
* Features have no real existance, but add atmosphere to a whole area,
* by being visible across a large number of locations.
* You define location, size, item desc, and add an array of 'longs'
* sorted by distance, plus the usual item descriptions. Any occurrences
* of $D in the longs array will be replaced by the direction in which
* the feature lies.
* @param title the name of the feature
* @param region the region object to use in calculating the feature
* @param range the distance (number of rooms) it can be seen for
* @param descs array of descriptions to be shown at different ranges.
* These are divided equally along the total range
* @param items add_item name(s) (identical to the 'shorts' parameter in add_item)
* @param item_desc a string, or array of strings giving the corresponding
* @param whole_sentance use a whole sentance for the descriptions or wrap
* them up into fragments
* @example
*   region = clone_object(TERRAIN_MAP_REGION_POLYGON);
*   region->add_vertex(12, 13);
*   region->add_vertex(14,15);
*   region->add_vertex(17,18);
*   add_feature_region("northmountain", region, 7,
*            ({ "A huge snowy mountain towers over you to the $D.",
*               "A huge snowy mountain looms to the $D.",
*               "A fair way off to the $D, you can see a huge snowy mountain.",
*               "A long way $Dwards, you glimpse the snowy peak of a "
*               "mountain." }),
*               "mountain", "From here, it looks quite astoundingly huge.",
*            ({ }), 0 );
* @see add_road_type
* @see add_obstacle_type
* @see set_feature_chats
* @see modify_feature
* @see add_item
*/
void add_feature_ob(string title, object feature) {
   if (_real_coords && _real_coords->xdiff) {
      feature->set_distance_multiplier(_real_coords->xdiff);
   }
   _features[title] = new(class feature_data,
                          f_items : feature->query_items(),
                          f_item_desc : feature->query_day_items(),
                          f_item_night_desc : feature->query_night_items(),
                          feature_ob : feature);
}

/**
* This method is used to modify the descriptions and/or item descs of
* an established feature. Changes will be instantly seen in all the
* rooms the feature is visible from.
* @param title the name of the feature
* @param descs array of descriptions to be shown at different ranges.
* add_item name(s) (identical to the 'shorts' parameter in add_item)
* @param item_desc a string, or array of strings giving the corresponding
* These are divided equally along the total range
* @see set_feature_chats
* @see add_feature
* @see add_item
*/
varargs void modify_feature(string title, string *descs, mixed item_desc)

{
   if (!_features[title]) {
      return;
   }

   if (descs) {
      _features[title]->descs = descs;
   }
   if (item_desc) {
      _features[title]->f_item_desc = item_desc;
   }
}


/**
* This method is used to add room chats to locations that can view a
* particular feature. $D substitution is carried out on the chats.
* @param title the name of the feature
* @param chats an array of strings (room chats related to the feature)
* @see add_feature
* @see modify_feature
* @see room_chat
*/
void set_feature_chats(string title, mixed *chats, mixed* night_chats)

{
   if (!_features[title]) {
      return;
   }

   ((class feature_data)_features[title])->chats = chats;
   ((class feature_data)_features[title])->night_chats = night_chats;
}


/**
* This method is used to remove a named feature from the terrain map
* @param title the name of the feature
* @see add_feature
*/
void remove_feature(string title)

{
   object room;

   if (!_features[title]) {
      return;
   }

   map_delete(_features, title);
   if (!_feature_rooms[title]) {
      return;
   }

   _feature_rooms[title] -= ({ 0 });
   foreach(room in _feature_rooms[title]) {
      room->remove_feature(title);
   }

   map_delete(_feature_rooms, title);
}


/**
* This method is used to add a signpost into a specified location in a
* terrain_map. It is identical to the /std/room/ add_sign, except for
the x and y coordinates which specify where the sign should go.
* @param title the name of the feature
* @param x the x coordinate of the center point
* @param y the y coordinate of the center point
* @example
*   add_sign(4, 3, "A wooden signpost.\n",
*                  "\n-----------------\n"
*                    "| Ladder: East  |\n"
*                    "| Bridge: South |\n"
*                    "-----------------",
*                  "signpost", ({ "signpost", "sign" }), "common");
*
* @see add_sign
*/
void add_sign(int x, int y, string sign_long, mixed sign_read_mess,
              string sign_short, mixed sign_name, string sign_language)

{
   string   title = sprintf("%d:%d", x, y);

   _signposts[title] = new(class sign_data,
                         long:sign_long, read_mess:sign_read_mess,
                         short:sign_short, name:sign_name,
                         language:sign_language);
}


/**
* This method is used to map ASCII characters used in the zone map onto
* one or more actual room zones.
* @param type the ASCII character used in the zone map
* @param zones a zone name or an array of zone names
* @example
*   add_zones("*", ({ "zone2", "zone1" }));
*/
void add_zones(string type, string *zone_names)

{
   _zone_types[type] = zone_names;
}


// Dests all the rooms, and reloads all the maps.
/**
* @ignore
*/
void reset_handler()

{
   mixed *rooms;

   if (arrayp(_room_map)) {
      foreach(rooms in _room_map) {
         rooms->dest_me();
      }
   }
   _room_map = 0;
   _area_map = 0;
   setup_area_map();
}


// Load up the zone map. Based on the map_file name.
/**
* @ignore
*/
void load_zone_map()

{
   string   map_path = query_map_file() + ".zones";
   int      index = 1;
   string   line;
   int      width;
   int      height;

   _zone_map = ({ });
   while (line = read_file(map_path, index++, 1)) {
      if (width < sizeof(line))
         width = sizeof(line);
      _zone_map += ({ line });
   }
   height = index - 1;

   if ((width != _width) || (height != _height))
      _zone_map = ({ });
}


// Load up the area map. Based on the map_file name.
/**
* @ignore
*/
void load_area_map()

{
   string   map_path = query_map_file() + ".map";
   int      index = 1;
   int      height;
   string   line;

   _area_map = ({ });
   while (line = read_file(map_path, index++, 1)) {
      if (sizeof(line)) {
         line = line[0..<2];
         if (_width < sizeof(line)) {
            _width = sizeof(line);
         }
         //if (sizeof(line) < _width)
            //continue;
         _area_map = ({ line }) + _area_map;
         height++;
      }
   }
   _height = height;
   if (!height) {
      printf("Unable to open %O\n", map_path);
   }
}

/**
* @ignore
* Handles the cloning of a room when it is specified as:
* path/name:x:y
*/
object create_virtual_object(string x_str, string y_str, string z_str)

{
   int x = to_int(x_str);
   int y = to_int(y_str);
   int z = to_int(z_str);

   if (!stringp(x_str) || !stringp(y_str) || !stringp(z_str)) {
      return 0;
   }

   return query_room(x, y, z);
}

/**
 * This method finds the room at the specific real coordinate.
 * @param x the x location
 * @param y the y location
 * @return the room, or 0 on failure
 */
string find_room_at_real_coord(int x, int y, int z) {
   class char_map_data room_map;
   class coord our;

   our = query_terrain_from_real_coord(x, y, z);
   if (!our) {
      return 0;
   }
   room_map = query_char_map(our->x, our->y);
   if (!room_map) {
      debug_printf("Unable to load room at %d %d (%d %d)\n", our->x, our->y, x, y);
      return 0;
   }

   if (room_map->type == AREA_OBSTACLE) {
      return 0;
   }

   return sprintf("%s:%d:%d:%d", base_name(this_object()), our->x, our->y, our->z);
} /* find_room_at_real_coord() */

/**
 * This method returns the distance that is moved if you go in the
 * specified direction.
 * @return the distance
 */
int query_direction_distance(string dir) {
  switch (_direcs[dir][3]) {
   case X_DIFF :
      return _real_coords->xdiff;
   case Y_DIFF :
      return _real_coords->ydiff;
   case XY_DIFF :
      return _real_coords->xydiff;
   }
   return 0;
}

/** @ignore yes */
void dest_me() {
   class feature_data womble;
   string name;

   foreach (name, womble in _features) {
      if (womble->feature_ob) {
         womble->feature_ob->dest_me();
      }
   }

   destruct(this_object());
}
