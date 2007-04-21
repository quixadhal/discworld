/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: terrain_room.c,v 1.58 2003/07/15 11:59:02 taffyd Exp $
 * 
*/
/**
* @main
* Base inherit for a terrain_map room.
* <p>
* This file should be inherited by any room within a terrain mapped
* virtual area.
* <p>
* When inheriting from this file, it is essential that the function
* query_handler_path() be overriden to return the full pathname of
* the handler associated with this area.
* <p>
* To inherit from this file, use the following lines:
* <ul>
* <li>#include <terrain_map.h>
* <li>
* <li>inherit TERRAIN_MAP_OUTSIDE_BASE
* </ul>
* 
* @see terrain_map_handler
* @see terrain_map_intro
* @see terrain_map_example
* @author Dek
* @started Thurs April 12 2001 Too Damn Early BST
**/

#include <room.h>
#include <login_handler.h>
#include <terrain_map.h>

inherit TERRAIN_MAP_FACTORY_BASE;

#define BLOCKING_FLAG 2
#define INSTALLED_FLAG 1

private nosave int     _x_coord = -1;
private nosave int     _y_coord = -1;
private nosave int     _z_coord = -1;
private nosave int     _flag = 0;
private nosave string  _extra_long = "";
private nosave object  _map_handler;
private nosave mapping _dist_features = ([ ]);
private nosave mapping _features = ([ ]);
private nosave string  _outside_types;
private nosave string  _random_desc = 0;
private nosave object  _terrain_chatter = 0;
private nosave string* _variable_exits;
private nosave mapping _default_exits;
private nosave string  _terrain_handler;
private nosave string  _terrain_char;

varargs int add_property(string var, mixed val, int tim);

string  query_feature_item_desc(string title);
string  query_handler_path();
varargs void terrain_chat( mixed *args, object chatobj );
int do_outside_chats();
void add_zone(string zone);
void add_extra_look(object ob);
int remove_item(string word);
varargs int add_item( mixed shorts, mixed desc, int no_plural );
int* query_co_ord();
int query_visibility();
string long(string str, int dark);
string query_terrain_handler();

class feature {
   mapping direcs;
   mixed items;
   int sentance;
   // Always the closest index.
   int index;
   int visible;
}

void create() {
   _variable_exits = ({ });
   _default_exits = ([ ]);
   this_object()->add_help_file("terrain_room");
   ::create();
} /* create() */

string long_test(string str, int dark) {
   string ret;
   string map;
   string* map_lines;

   ret = long(str, dark);
   // Now try and mangle together some formatting.
   map = query_terrain_handler()->query_player_map_template(_x_coord,
             _y_coord, _z_coord, query_visibility(), 9);
   // Now try and mangle them together.
   // For now, do this the hard way.
   map_lines = map(explode(map, "\n"), (: $1 + "   " :));
   ret = "$COLUMN$12=" + implode(map_lines, "\n") + "$COLUMN$" +
         ret;
   return ret;
}

/**
 * This method makes a deterministic hash value based on the real co-ordinates
 * of this room.
 * @param mod the modules to use on the return result
 * @return a deterministicly random number between 0..mod -1
 */
int hash(int mod) {
   mixed* my_coord; // Hmm -- there seems to be a class name called coord. 
   int x;
   int y;
   int z;
   int grid;
   int val;
   string str;

   my_coord = query_co_ord();
   if (!my_coord) {
      return 0;
   }
   grid = TERRAIN_MAP_GRID_SIZE / 10;
   x = (my_coord[0]) / grid;
   y = (my_coord[1]) / grid;
   z = (my_coord[2]) / grid;

   str = "" + x + ":" + y + ":" + z;
   //val = (x ^ 1693 + y ^ 6603 + z ^ 7919) % mod;
   val = crc32(str) % mod;
   if (val < 0) {
      return - val;
   }
   return val;
}

/**
 * This produces a deterministicly static value that only exists for a
 * specific period of time, before moving.  You can use this for movable
 * oasises for example.
 * @param mod the modulus to get the random number over
 * @param period the period the number should remain constant
 */
int hash_time(int mod, int period) {
   mixed* my_coord;
   int x;
   int y;
   int z;
   int grid;
   int val;
   string str;

   my_coord = query_co_ord();
   if (!my_coord) {
      return 0;
   }
   grid = TERRAIN_MAP_GRID_SIZE / 10;
   x = (my_coord[0]) / grid;
   y = (my_coord[1]) / grid;
   z = (my_coord[2]) / grid;

   //val = (x ^ 1693 + y ^ 6603 + z ^ 7919 + (time() / period) ^ 80147) % mod;
   str = x + ":" + y + ":" + z + ":" + (time() /period);
   val = crc32(str) % mod;
   if (val < 0) {
      return - val;
   }
   return val;
}

/**
 * This method returns the terrain handler for
 * the room.
 * @return the terrain handler
 */
string query_terrain_handler() {
   return _terrain_handler;
}

/**
 * This method sets the terrain handler for the room.
 * @param handler the terrain handler
 */
void set_terrain_handler(string handler) {
   _terrain_handler = handler;
}

/**
 * This returns the terrain coordinate of the room.
 * It returns an array that contains the x, y, z values
 * of the co-ordinate.
 * @return the terrain co-ordinate
 */
mixed *query_terrain_coords() { return ({ _x_coord, _y_coord, _z_coord }); }

/**
* @ignore
*/
void create_terrain_room()

{
// We set the coords to -1 -1 here, to account for rooms that are
// not virtual. Rooms that adjoin map areas need to inherit this
// file, hence the need to do this.
   _x_coord = -1;
   _y_coord = -1;
   _z_coord = -1;

   add_property("terrain_map", 1);
   add_extra_look(this_object());
   if (do_outside_chats())
      terrain_chat(({ 60, 120, ({ "#do_a_feature_chat",
                             "#do_an_outside_chat" }) }) );
}


/**
* @ignore
*/
void terrain_setup()

{
}

/**
* @ignore
*/
void set_terrain_coords(int x, int y, int z)

{
// Setup the main coordinates for this room. Please note that these
// are not DW coords, but local ones for the use of the handler. 
   _x_coord = x;
   _y_coord = y;
   _z_coord = z;
}


/**
* @return returns the handler object associated with this terrain map.
* @see query_handler_path()
*/
object query_map_handler()

{
   return load_object(query_terrain_handler());
}


/**
* This method destroys every single room in the associated terrain map.
* It is provided for development purposes only, and should be used cautiously
* as it will currently dump every single player in the map into the void.
* Plans are afoot to make the side-effects a little less dire.
*/
void reset_handler()

{
   if (query_map_handler())
      query_map_handler()->reset_handler();
}


/**
* @ignore
*/
void receive_message(string mess_class, string message, string title)

{
   class    feature feature;
   
   if (mess_class == "feature") {
      feature = _features[title];
      if (!feature) {
         return;
      }
      
      //message = replace(message, ({ "$D", feature->direc_str }));
   } else if (mess_class == "direction") {
      // Need to figure out the direction back to the source.
   }
   tell_room(this_object(), message);
}


/**
* @ignore
*/
void do_a_feature_chat()

{
   string   *chats = ({ });
   string   feature_chat;
   string   title;
   class  feature feature;
   //object   handler = query_map_handler();
   
   if ((_x_coord < 0) || (_y_coord < 0)) {
      return;
   }
      
   foreach(title, feature in _features) {
      //feature_chat = handler->get_a_feature_chat(title, feature->index, 
      //                                                  feature->direc_str);
      if (feature_chat) {
         chats += ({ feature_chat });
      }
   }
   if (sizeof(chats)) {
      tell_room(this_object(), chats[random(sizeof(chats))] + "\n");
   }
}
      
      
/**
* @ignore
*/
void do_an_outside_chat()

{
   string chat = query_map_handler()->get_an_outside_chat(_outside_types);

   if (chat && strlen(chat)) {
      tell_room(this_object(), chat + "\n");
   }
}

/**
 * If this can be seen from this location or not.  This can do things
 * like dealing with lower levels and stuff.
 * @param char the character of the item in the map
 * @param path the path of the room (0 if an obstacle)
 * @param z the z co-ordinate of the room
 * @param direc the direction in which the obstacle faces
 */
int can_view_adjacent_desc(string char, string key_here, string path,
                           int z, string direc) {
   return char != key_here;
}


/**
* This method may be overriden to mask out feature descriptions and chats
* from one or more features in the current terrain map.
* @param name the name of the feature being queried.
* @return return 1 if the feature can be viewed from this room, 0 otherwise
* @example
*    int can_view_feature(string name) 
*    
*    {
*       switch (name) {
*          case "mountain" : return 0;
*          case "tree"     : return 0;
*          default         : return 1;
*       }
*    }
* @see add_feature
* @see modify_feature
* @see set_feature_chats
* @see remove_feature
*/
int can_view_feature(string name)

{
   return 1;
}

/**
* This method may be overriden to mask out feature descriptions and chats
* from one or more features in the current terrain map.  It allows more
* detailed blocking, based on distance and direction.
* @param name the name of the feature being queried.
* @param direc the direction to the feature
* @param distance the range to the feature
* @return return 1 if the feature can be viewed from this room, 0 otherwise
* @example
*    int can_view_feature_from(string name, string direc, int distance) 
*    
*    {
*       switch (name) {
*          case "mountain" :
*             switch (direct) {
*             case "east" :
*                return 1;
*             default :
*                return 0;
*             }
*          case "tree"     : return 0;
*          default         : return 1;
*       }
*    }
* @see add_feature
* @see modify_feature
* @see set_feature_chats
* @see remove_feature
*/
int can_view_feature_from(string name, string direc, int distance)

{
   return 1;
}


/**
* This method may be overriden to prevent the handler from setting up
* exits to one or more types of location.
* @param type the type of location the exit would be to. This is the same as 
* the character used in the ASCII map.
* @return return 1 if the exits should be made, 0 otherwise
* @example
*    int can_exit_to(string type) 
*    
*    {
*       switch (type) {
*          case "#" : return 0;
*          case "*" : return 0;
*          default  : return 1;
*       }
*    }
* @see can_enter_from
* @see add_room_type
* @see add_road_type
* @see query_to_same
* @see query_to_other
* @see query_from_other
*/
int can_exit_to(string type, string direc, string base_path)

{
   return 1;
}


/**
* This method may be overriden to prevent the handler from setting up
* exits from one or more types of location.
* @param type the type of location the exit would be from. This is the same as 
* the character used in the ASCII map.
* @return return 1 if the exits should be made, 0 otherwise
* @example
*    int can_enter_from(string type) 
*    
*    {
*       switch (type) {
*          case "#" : return 0;
*          case "*" : return 0;
*          default  : return 1;
*       }
*    }
* @see can_exit_to
* @see add_room_type
* @see add_road_type
* @see query_to_same
* @see query_to_other
* @see query_from_other
*/
int can_enter_from(string type, string direc, string base_path)

{
   return 1;
}


/**
* This method may be overriden to allow the handler to setup diagonal exits
* between terrain types that are different.  What this does, here is
* a small explaination.  If you have a map below:
* <pre>
*    ff-
*    f-f
*    -ff
* <pre>
* Where the '-'s are roads, you do not want to let someone diagonaly
* jump the road.  So you make it so that you cannot go from the
* f on one side of the road to the other.  THis is the default behaviour,
* however you want people to be able to actualy walk diagonaly on the road
* so you need to override this to allow the road to be walked on.
* exits from one or more types of location.
* <p>
* The values passed in are the paths to the final object files.  Not the
* key values in the map.
* @param my_type our type
* @param direc the direction we are checking in
* @param first_type the first neighbouring path
* @param second_type the second neighbouring path
* @param new_base_path the path of the thing that is the destination
* @return return 1 if the exits should be made, 0 otherwise
* @see can_exit_to
* @see add_room_type
* @see add_road_type
* @see query_to_same
* @see query_to_other
* @see query_from_other
*/
int can_move_between(string my_type, string direc,
                     string first_type, string second_type,
                     string new_base_path)
{
   return 1;
}

/**
 * This method is used to setup any unusual sorts of entrances to 
 * specific room types.  For example so that you have to actually swim
 * to get into a river rather than acidentally walking into it.  If
 * this does return a string a default exit is not setup for this direction
 * so just going 'east' doesn't drop you in the river.
 * @param key the key of the map location we are coming from
 * @param key_here my key of the character in the map
 * @param direc the direction the exit is in
 * @param room the room being setup
 * @param base_path the base path of the object we are coming from
 * @return 0 to do nothing, a string to replace the main exit
 */
string query_enter_exit_direction(string key, string key_here,
                                  string direc, object room, string base_path) {
   return 0;
}


/**
* This method may be overriden to prevent the handler from setting up
* additional room_chats from appropriate adjacent locations.
* @return return 1 if the exits should be made, 0 otherwise
* @see set_external_chats
*/
int do_outside_chats()

{
   return 1;
}


/**
* @ignore
*/
varargs void terrain_chat( mixed *args, object chatobj ) {
   if (!args)  {
      /* This can happen if you have day chats w/o night chats, */
      /* and vice-versa */
      return;
   }
   if ( !pointerp( args[ 2 ] ) ) {
      write( "Error: second argument of room_chat args is not an array.\n" );
      return;
   }
  if ( _terrain_chatter ) {
    _terrain_chatter->setup_chatter( this_object(), args );
    return;
  }
  if (objectp(chatobj)) {
    _terrain_chatter = chatobj;
    log_file("CHATTER", "%s cloned set terrain chatter: %O\n",
             file_name(this_object()), chatobj);
  } else {
    _terrain_chatter = clone_object( CHATTER_OBJECT );
    log_file("CHATTER", "%s cloned terrain chatter: %O\n",
             file_name(this_object()), _terrain_chatter);
  }
  _terrain_chatter->setup_chatter( this_object(), args );
}


/**
* @ignore
*/
void add_random_desc(string desc)

{
   if (!_random_desc) {
      _random_desc = "";
   }
   
   _random_desc += desc + " ";
}


/**
* @ignore
*/
void terrain_add_zone(string zone)

{
   add_zone(zone);
   
   query_map_handler()->add_room_to_zone(this_object(), zone);
}


/**
 * This function allows you to change the destination location of the room
 * if nessessary.  This is useful for bridges and other things which need to
 * move people up or down one z-coordinate when they cross the line.
 * @param x the x coordinate
 * @param y the y coordinate
 * @param z the z coordinate
 * @param direct the direction used to enter this location
 * @peram key the key of the room that is coming to us
 */
string query_dest_coordinate_string(int x, int y, int z,
                                    string direc, string key) {
   return ":" + x + ":" + y + ":" + z;
}


/**
* The handler calls this function when setting up exits to allow the
* room to set up any modify_exit parameters which should be applied
* to exits leading to other locations of the same type as this. 
* Override it to provide any extra exit modifiers.
* @param type the ASCII character representing this room type
* @return an array of exit modifiers. This should take the same form as
* used in modify_exit
* @example
* mixed *query_to_same(string type)
* 
* { 
*    return ({ "move mess", "You wade along the stream bed.\n",
*              "enter mess", "$N wades in from the $F.",
*              "exit mess", "$N wades off to the $T.",
*              "hidden", "1 });
* }
* @see modify_exit
* @see query_to_other
* @see query_from_other
*/
varargs mixed *query_to_same(string type, string to_direc, string from_direc, string path_to) {
   return 0;
}


/**
* The handler calls this function when setting up exits to allow the
* room to set up any modify_exit parameters which should be applied
* to exits leading to other locations of different types to this. 
* Override it to provide any extra exit modifiers.
* @param type the ASCII character representing the room type the exit leads to
* @return an array of exit modifiers. This should take the same form as
* used in modify_exit
* @example
* mixed *query_to_other(string type)
* 
* { 
*    return ({ "move mess", "You scramble up the stream bank.\n",
*              "enter mess", "$N scrambles up the stream bank from the $F.",
*              "exit mess", "$N scrambles up the stream bank to the $T.",
*              "hidden", "1 });
* }
* @see modify_exit
* @see query_to_same
* @see query_from_other
*/
varargs mixed *query_to_other(string type, string to_direc, string from_direc, string path_to) {
   return 0;
}


/**
* The handler calls this function when setting up exits to allow the
* room to set up any modify_exit parameters which should be applied
* to exits leading from other locations of different types to this. 
* Override it to provide any extra exit modifiers.
* @param type the ASCII character representing the room type the exit leads from
* @return an array of exit modifiers. This should take the same form as
* used in modify_exit
* @example
* mixed *query_from_other(string type)
* 
* { 
*    return ({ "move mess", "You scramble down the bank into the stream.\n",
*              "enter mess", "$N scrambles down the $F bank into the stream.",
*              "exit mess", "$N scrambles down the stream bank to the $T" });
* }
* @see modify_exit
* @see query_to_same
* @see query_from_other
*/
varargs mixed *query_from_other(string type, string to_direc, string from_direc, string path_from) {
   return 0;
}


// The next three are prettification functions for the above, and should
// never be called directly.
/**
* @ignore
*/
mixed *get_to_same(string from, string to, string type, string path_to)

{ 
   mixed *result = query_to_same(type, to, from);
   
   if (arrayp(result)) {
      return map(result, (: stringp($1)?replace($1, ({ "$T", $2, "$F", $3 }) ):$1 :), to, from);
   }
   
   return 0;
}


/**
* @ignore
*/
mixed *get_to_other(string from, string to, string type, string path_to)

{ 
   mixed *result = query_to_other(type, to, from);
   
   if (arrayp(result)) {
      return map(result, (: stringp($1)?replace($1, ({ "$T", $2, "$F", $3 }) ):$1 :), to, from);
   }
   
   return 0;
}


/**
* @ignore
*/
mixed *get_from_other(string from, string to, string type, string path_from)

{ 
   mixed *result = query_from_other(type, to, from);
   
   if (arrayp(result)) {
      return map(result, (: stringp($1)?replace($1, ({ "$T", $2, "$F", $3 }) ):$1 :), to, from);
   }
   
   return 0;
}


// Called by the handler to set our extra_look strings.
/**
* @ignore
*/
void set_extra_long(string extra)

{
   _extra_long = extra;
}


/**
* @ignore
*/
void set_outside_types(string types)

{
   _outside_types = types;
}

/** 
 * This method returns the actual file name of this object.
 */
string query_my_real_file_name() {
   return __FILE__;
}

// This handles all the extra parts of the room desc which are
// inserted by the handler.
/**
* @ignore
*/
string extra_look() {
   string   result = _extra_long;
   string   title;
   string*   tmp;
   class  feature feature;
   int new_line;
   //int index;
   //string* direc;
   string* descs;
   int visibility;
   
   new_line = query_map_handler()->query_newline_mode();
   descs = ({ });
   visibility = query_visibility();
   foreach (title, feature in _features) {
      tmp = query_map_handler()->query_feature_desc(title,
                                          feature->direcs, visibility);
      if (sizeof(tmp)) {
         feature->visible = 1;
         descs += tmp;
      }
   }

   foreach (title, feature in _dist_features) {
      tmp = query_map_handler()->query_distant_feature_desc(title,
                                        feature, visibility);
      if (sizeof(tmp)) {
         //feature->visible = 1;
         descs += tmp;
      }
   }

   if (sizeof(descs)) {
      result += "  " + capitalize(query_multiple_short(descs)) + ".";
   }

   if (_random_desc) {
      result = result + _random_desc;
   }

   if (new_line) {
      return result;
   } else { 
      return result + "\n";
   }
}


// Add a new visible feature to the room, so it knows to query the handler
// for descriptions and so on. The zone info is needed as an easy way for
// the handler to get access to which rooms see which features. 
/**
* @ignore
*/
void add_feature(string title, mapping bits, mixed items, int sentance)

{
   //string* descs;

   _features[title] = new(class feature,
                          items : items, direcs : bits);
   if (items) {
      add_item(items, (: query_feature_item_desc($(title)) :));
   }
}

// Add a new visible feature to the room, so it knows to query the handler
// for descriptions and so on. The zone info is needed as an easy way for
// the handler to get access to which rooms see which features. 
/**
* @ignore
*/
void add_distant_feature(string title, mapping bits) {
   _dist_features[title] = bits;
}


/**
* @ignore
*/
void remove_feature(string title)

{
   class feature  feature = _features[title];
   mixed    items;
   
   if (!feature) {
      return;
   }
   
   map_delete(_features, title);
   items = feature->items;
   if (stringp(items)) {
      remove_item(items);
   } else {
      remove_item(items[0]);
   }
}


/**
* @ignore
int query_feature_range(string title)

{
   class feature   feature = _features[title];
   
   if (!feature) {
      return -1;
   }
   
   return feature->index;
}
*/


/**
* @ignore
*/
string query_feature_item_desc(string title)
{
   if (_features[title]->visible) {
      return query_map_handler()->query_feature_item_desc(title);
   }
   return 0;
}


/**
* @ignore
*/
mapping query_features() { return _features; }

/**
* @ignore
*/
int query_installed() { return _flag & INSTALLED_FLAG; }
/**
* @ignore
*/
void set_installed() { _flag |= INSTALLED_FLAG; }

/**
 * This method sets if the particular room is see through or not.
 * @param blocking sets the blocking flag
 */
void set_terrain_map_block(int blocking) {
   if (blocking) {
      _flag |= BLOCKING_FLAG;
   } else {
      _flag &= ~BLOCKING_FLAG;
   }
}

/**
 * This method returns if the room is blocking or not.
 * @return if the room is blocking or not
 */
int query_terrain_map_block() {
   return _flag & BLOCKING_FLAG;
}

/**
 * This method sets the character to use for this terrain in the player
 * visible map.
 * @param char the character to use in the map
 */
void set_terrain_map_character(string terr) {
   _terrain_char = terr;
}

/**
 * This method returns the character to use for this terrain in the player
 * visible map.
 * @return the terrain character
 */
string query_terrain_map_character() {
   return _terrain_char;
}


/**
* @ignore
* God only knows whether anyone'll ever use this. But the standard
* terrain stuff implements it, so what the hell :-)
*/
string query_cloning_info()
{
  return file_name(this_object());
}



/**
* @ignore
* Handles the cloning of a room when it is specified as:
* path/name:x:y
*/
object create_virtual_object(string x_str, string y_str)
{
   int x = to_int(x_str);
   int y = to_int(y_str);
   
   return query_map_handler()->load_room_at(x, y);
}

/**
 * This method adds a variable exit into the list of variable exits.
 * @param exit the exit to make variable
 */
void add_variable_exit(string exit) {
   _variable_exits += ({ exit });
}

/**
 * This method adds a default exit into the list of default exits.
 * @param exit the exit to make variable
 * @param location this is the walk exit location
 */
void add_default_exit(string exit, string location) {
   _default_exits[exit] = location;
}

/**
 * This method returns the default exits mapping.
 * @return the default exits mapping
 */
mapping query_default_exits() {
   return _default_exits;
}

/**
 * This method returns the prefix to use for 'walk' exits in this room.
 */
string query_terrain_map_walk_exit() {
   return TERRAIN_MAP_WALK_EXIT;
}

/**
 * This method returns the prefix to use for 'journey' exits in this room.
 */
string query_terrain_map_journey_exit() {
   return TERRAIN_MAP_JOURNEY_EXIT;
}

/**
 * This method returns the path at the specific co-ordinate.
 * @param x the x co-ordinate
 * @param y the y co-ordinate
 * @param z the z co-ordinate
 * @return the file name of the new room
 */
string query_room_path(int x, int y, int z) {
   return sprintf("%s:%d:%d:%d", query_terrain_handler(), x, y, z);
}

/**
 * This method returns a string which says how far they have moved.
 * @param num how many steps in the direction
 * @param dir the direction
 */
string query_direction_distance_str(int num, string dir) {
   int dist;
   string dist_mess;

   dist = query_map_handler()->query_direction_distance(dir);
   switch (dist) {
   case 0..2 :
      if (dist > 1) {
         dist_mess = query_num(dist) + " foot section";
      } else {
         dist_mess = "foot";
      }
      break;
   case 3..TERRAIN_MAP_ONE_MILE / 2 - 1 :
      dist /= 3;
      if (dist > 1) {
         dist_mess = query_num(dist) + " yard section";
      } else {
         dist_mess = "yard";
      }
      break;
   case TERRAIN_MAP_ONE_MILE / 2..TERRAIN_MAP_ONE_MILE - 1 :
      dist /= TERRAIN_MAP_ONE_MILE / 2;
      if (dist > 1) {
         dist_mess = query_num(dist) + " miles section";
      } else {
         dist_mess = "mile";
      }
      break;
   default:
      dist /= TERRAIN_MAP_ONE_MILE;
      if (dist > 1) {
         dist_mess = query_num(dist) + " miles section";
      } else {
         dist_mess = "mile";
      }
      break;
   }
   switch(num) {
   case 0:
      return dist_mess + " " + dir;
   case 1:
      return query_num(num) + " " + dist_mess + " " +dir;
   default:
      return query_num(num) + " " + dist_mess + "s " + dir;
   }
}

/**
 * This messes with the return values of the rooms.
 * @param exit the exit to look at
 * @param data the actual data from the room code
 * @return the messed with data
 */
mixed* query_dest_other(string exit, mixed* data) {
   mixed* bing;
   string cur;
   string dir;
   int num;
   string* bits;
   int dist;
    

   // We need to return data here because data comes from 
   // /std/room->query_dest_other() lower down in the inheritance tree.
   if(!stringp(exit) || !_default_exits) {
     return data;
   }
   
   if (_default_exits[exit]) {
      if (this_player() &&
          this_player()->query_property(TERRAIN_MAP_LONG_JUMP_PROPERTY) &&
          member_array(query_terrain_map_journey_exit() + exit, _variable_exits) != -1) {
         exit = query_terrain_map_journey_exit() + exit;
      } else {
         data[ROOM_DEST] = _default_exits[exit];
         return data;
      }
   }

   if (member_array(exit, _variable_exits) != -1) {
      // Mess with it!
      bing = query_map_handler()->find_next_room_from(_x_coord, _y_coord, _z_coord, exit[strlen(query_terrain_map_journey_exit())..]);
      if (bing) {
         if (data[ROOM_DEST] != bing[0]) {
            data[ROOM_DEST] = bing[0];
            bits = ({ });
            foreach (dir in bing[1..]) {
               if (dir == cur) {
                  num++;
               } else {
                  if (cur) {
                     bits += ({ query_direction_distance_str(num, cur) });
                  }
                  cur = dir;
                  num = 1;
               }
            }
            dist = query_map_handler()->query_direction_distance(dir);
            bits += ({ query_direction_distance_str(num, dir) });
            if (sizeof(bits) > 1) {
               data[ROOM_MESS] = "You arrive after having journeyed " +
                                 implode(bits[0..<2], ", ") +
                                 " and " + bits[<1] +
                                 ".\n";
            } else {
               data[ROOM_MESS] = "You arrive after having journeyed " +
                                 bits[0] + 
                                 ".\n";
            }
            if (!arrayp(data[ROOM_ENTER]) ||
                !data[ROOM_ENTER][0]) {
               data[ROOM_ENTER] = "$N journey$s in from " +
                        ROOM_HANDLER->query_opposite_direction(bing[<1]) + ".\n";
            }
            if (!data[ROOM_EXIT]) {
               data[ROOM_EXIT] = "$N journey$s to the " + bing[1] + ".\n";
            }
         }
      }
   }
   return data;
}

/**
 * Mess with the data.
 */
mixed* query_dest_dir(mixed* data) {
   int i;
   int jump;
   mixed* bits;

   if (this_player() &&
       this_player()->query_property(TERRAIN_MAP_LONG_JUMP_PROPERTY)) {
      jump = 1;
   }

   if (sizeof(_variable_exits)) {
      for (i = 0; i < sizeof(data); i += 2) {
         if (_default_exits[data[i]]) {
            if (jump) {
               bits = query_map_handler()->find_next_room_from(_x_coord, _y_coord, _z_coord, data[i]);
               if (bits) {
                  data[i+1] = bits[0];
               }
            } else {
               data[i+1] = _default_exits[data[i]];
            }
         } else if (member_array(data[i], _variable_exits) != -1) {
            bits = query_map_handler()->find_next_room_from(_x_coord, _y_coord, _z_coord, data[i][strlen(query_terrain_map_journey_exit())..]);
            if (bits) {
               data[i+1] = bits[0];
            }
         }
      }
   }
   return data;
}

/**
 * This method draws a little debug map for the creator.
 */
int do_map() {
   string map;

   map = query_map_handler()->query_debug_map(_x_coord, _y_coord, 13, _x_coord, _y_coord);
   write(map);
   add_succeeded_mess("");
   return 1;
}

/**
 * This method draws the map of the entire terrain.
 */
int do_map_terrain() {
   string map;

   map = query_map_handler()->query_debug_map(20, 20, 40, _x_coord, _y_coord);
   write(map);
   add_succeeded_mess("");
   return 1;
}


/**
* @ignore
*/
void init()

{
   tell_creator(this_player(), "Coords: (%d, %d, %d)\n", _x_coord, _y_coord, _z_coord);
   if (this_player()->query_creator()) {
      add_command("cremap", "", (: do_map :));
      add_command("cremap", "all", (: do_map_terrain :));
   }
}

mixed stats() {
   return ::stats() +
          ({ ({ "file", query_my_real_file_name() }),
             ({ "blocking", query_terrain_map_block() }) });
}

void dest_me() {
  if(_terrain_chatter) {
    log_file("CHATTER", "%s dested terrain chatter: %O\n",
             file_name(this_object()), _terrain_chatter);
    _terrain_chatter->dest_me();
    
  }
}
