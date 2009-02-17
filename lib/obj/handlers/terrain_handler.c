/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: terrain_handler.c,v 1.6 1999/11/10 09:50:40 taffyd Exp $
 */
/**
 * This is the terrain handler method documentation. For a more general
 * introduction to the terrain handler, please see the help for terrains.
 *
 * @author Sin
 * @author Turrican
 * @author others...
 *
 * @change 9 Nov 1997, Sin
 *    added support for inherit statements to
 *    get_room_size(), this fixes a bug in the terrain handler in
 *    regards to the placement of set_room_size() functions.  Added
 *    a limit of 4 inherits, so infinite loops aren't a problem.
 * @change 25 Nov 1997, Sin
 *    changed the calculate_exits() call so that it
 *    starts at the border of the current room instead of starting
 *    at the center of the current room.  This makes sense because
 *    it already knows the room size for the current room.  :)
 * @change 29 Nov 1997, Sin
 *    changed query_room_size() to cache results
 *    temporarily and to check the map handler before trying to
 *    parse on its own.
 * @see help::terrains
 */
#include <dirs.h>
#include <terrain.h>
#include <map.h>

#undef DEBUG_ROOM_SIZE
#undef DEBUG_CALC_EXITS
#define TP(x) tell_object(find_player("taffyd"), x);

#define BACKUP_TIME_OUT 1000000
inherit "/std/object";

string terrain_name;
mapping fixed_locations;
mixed *floating_locations;

private nosave int in_map;
private nosave mapping size_cache, cloned_locations, float_cache;
private nosave mapping std_types = ([ "north" : "path", "south" : "path",
  "east" : "path", "west" : "path", "northeast" : "hidden",
  "southwest" : "hidden", "southeast" : "hidden",
  "northwest" : "hidden", "up" : "stair", "down" : "stair" ]);

/** @ignore yes */
void setup() {
  size_cache = ([ ]);
  cloned_locations = ([ ]);
  float_cache = ([ ]);
  set_name("map");
  set_short("terrain map");
  add_adjective("terrain");
  set_long("This is a large map showing a large expanse of forest, desert, "
           "mountain or ocean.  A few locations of interest are marked on "
           "it, with most of the gaps between them blank or marked "
           "\"Here bee Draggons\" and the like, suggesting that the map-"
           "maker didn't know what was there either.\n" );
  add_adjective("terrain");
} /* setup() */

/**
 * This method checks wether there is a cloned location which matches the
 * given coordinates.
 * @param co_ords the coordinates to find the room for
 * @return the file name of the room with the given coordinates, or 0 if
 * it is not found
 * @see member_fixed_locations()
 * @see member_floating_locations()
 */
string member_cloned_locations(int *co_ords) {
  mapping tmp;

  if ((tmp = cloned_locations[terrain_name]) && (tmp = tmp[co_ords[0]]) &&
      (tmp = tmp[co_ords[1]])) {
      return tmp[co_ords[2]];
  }
  return 0;
} /* member_cloned_locations() */

/**
 * This method checks wether there is a fixed location which matches the
 * given coordinates.
 * @param co_ords the coordinates to find the room for
 * @return the file name of the room with the given coordinates, or 0 if
 * it is not found
 * @see member_cloned_locations()
 * @see member_floating_locations()
 */
string member_fixed_locations(int *co_ords) {
  int *loc_co_ords;
  string location;

  foreach (location, loc_co_ords in fixed_locations) {
    if ((co_ords[0] == loc_co_ords[0]) && (co_ords[1] == loc_co_ords[1]) &&
        (co_ords[2] == loc_co_ords[2])) {
      return location;
    }
  }
  return 0;
} /* member_fixed_locations() */

private int between(int limit1, int val, int limit2)
{
  if (limit1 < limit2) {
    return ((limit1 <= val) && (val <= limit2));
  } else {
    return ((limit2 <= val) && (val <= limit1));
  }
}

/**
 * This method checks wether there are floating locations which match the
 * given coordinates.
 * @param co_ords the coordinates to find the rooms for
 * @return an array with the file names of the rooms and the levels, or
 * the empty array if none are found
 * @see member_cloned_locations()
 * @see member_fixed_locations()
 */
mixed *member_floating_locations(int *co_ords) {
  int *data;
  mixed *right_locations, *location;
  
  right_locations = ({ });
  foreach (location in floating_locations) {
    data = location[1];
    if (sizeof(data) == 6) { 
      /* element is ({ file, ({ 6 coords bounding box }), priority }) */
      if (between(data[0], co_ords[0], data[3]) &&
          between(data[1], co_ords[1], data[4]) &&
          between(data[2], co_ords[2], data[5])) {
        right_locations += ({ location[0], location[2] });
      }
    } else {
      /* element is ({ file, ({ 3 coords of location }), priority }) */
      if ((co_ords[0] == data[0]) && (co_ords[1] == data[1]) &&
          (co_ords[2] == data[2])) {
        right_locations += ({ location[0], location[2] });
      }
    }
  }
  return right_locations;
} /* member_floating_locations() */

/**
 * This method searches the top level floating location for the given
 * coordinates.
 * @param co_ords the coordinates to find the top level for
 * @return the file name of the top level room with the given coordinates,
 * or 0 if none was found
 * @see member_floating_locations()
 */
string top_floating_location(int *co_ords) {
  int i, highest_level;
  string highest_location;
  mixed *right_locations;

  right_locations = member_floating_locations(co_ords);
  if (!sizeof(right_locations))
    return 0;
  highest_level = right_locations[1];
  highest_location = right_locations[0];
  for (i = 0; i < sizeof(right_locations); i += 2) {
    if (right_locations[i + 1] > highest_level) {
      highest_level = right_locations[i + 1];
      highest_location = right_locations[i];
    }
  }
  if (highest_location == "nothing")
    return 0;
  return highest_location;
} /* top_floating_location() */

private void init_data(string word) {
  terrain_name = word;
  fixed_locations = ([ ]);
  floating_locations = ({ });
} /* init_data() */

/**
 * This method loads the data file for a given terrain.
 * @param word the terrain to load the data for
 * @return 1 if the terrain exists, 0 if it didn't
 */
int get_data_file(string word) {
  if (terrain_name != word) {
    if (file_size(RESTORE_PATH + word + ".o") > 0) {
      unguarded((: restore_object, RESTORE_PATH + word :));
    } else {
      init_data(word);
      return 0;
    }
  }
  return 1;
} /* get_data_file() */
 
private void save_data_file(string word) {
  int number, last;
  string line, *lines;

  if (file_size(RESTORE_PATH + word +".o") > 0) {
    unguarded((: rename, RESTORE_PATH + word +".o",
               RESTORE_PATH +"backups/"+ word +"."+ time() :));
    lines = unguarded((: get_dir, RESTORE_PATH +"backups/"+ word +".*" :));
    if (sizeof(lines) > 3) {
      last = time() - BACKUP_TIME_OUT;
      foreach(line in lines) {
        sscanf(line, word +".%d", number);
        if (number < last)
          unguarded((: rm, RESTORE_PATH +"backups/"+ line :));
      }
    }
  }
  unguarded((: save_object, RESTORE_PATH + word :));
} /* save_data_file() */

/**
 * This method returns the cloned_locations mapping for a given terrain.
 * @param word the terrain name
 * @return the cloned_locations mapping; keys are the file names, values are
 * the coordinates for the files. There are also keys on x coordinates to
 * facilitate faster coordinate lookups.
 * @see query_fixed_locations()
 * @see query_floating_locations()
 */
mapping query_cloned_locations(string terrain) {
  return cloned_locations[terrain];
} /* query_cloned_locations() */

/**
 * This method returns the fixed_locations mapping for the given terrain.
 * @param word the terrain name
 * @return the fixed_locations mapping; keys are the file names, values are
 * the coordinates for the files
 * @see query_cloned_locations()
 * @see query_floating_locations()
 */
mapping query_fixed_locations(string word) {
  get_data_file(word);
  return fixed_locations;
} /* query_fixed_locations() */

/**
 * This method returns the floating_locations array for the given terrain.
 * @param word the terrain name
 * @return the floating_locations array; this is an array of arrays where each
 * array consists of the file name, an array of 6 coordinates forming a
 * bounding box or normal coordinates for the location and the level of the
 * floating location
 * @see query_cloned_locations()
 * @see query_fixed_locations()
 */
mixed *query_floating_locations(string word) {
  get_data_file(word);
  return floating_locations;
} /* query_floating_locations() */

/**
 * This method returns the coordinates for a given terrain and fixed location
 * filename.
 * @param terrain the terrain name
 * @param file the file name of the location
 * @return the coordinates
 */
int *query_co_ord(string terrain, string file) {
  get_data_file(terrain);
  return fixed_locations[file];
} /* query_co_ord() */

/**
 * This method returns the connecting room for the given coordinates and
 * direction.
 * @param terrain the terrain name
 * @param co_ords the coordinates
 * @param direc the direction to find the room for
 * @return the file name of the connecting room or 0 if none was found
 * @see query_connected()
 */
string query_connection(string terrain, int *co_ords, string direc) {
  mapping connection_info, tmp;
  string connections;

  if (!float_cache[terrain] ||
      !(connection_info = float_cache[terrain][co_ords[0]])) {
    if (file_size(RESTORE_PATH + terrain) != -2) {
      return 0;
    }
    if (file_size(RESTORE_PATH + terrain +"/"+ co_ords[0]) == -1) {
      return 0;
    }
    connections = unguarded((: read_file,
                              RESTORE_PATH + terrain +"/"+ co_ords[0] :));
    connection_info = restore_variable(connections);
    if (!mapp(float_cache[terrain])) {
      float_cache[terrain] = ([ co_ords[0] : connection_info ]);
    } else {
      float_cache[terrain][co_ords[0]] = connection_info;
    }
  }
  if ((tmp = connection_info[co_ords[1]]) && (tmp = tmp[co_ords[2]])) {
    return tmp[direc];
  }
  return 0;
} /* query_connection() */

/**
 * This method returns wether the given coordinates is connected to another
 * room in the terrain handler system.
 * @param terrain the terrain name
 * @param co_ords the coordinates
 * @return 1 if it is found, 0 otherwise
 * @see query_connection()
 */
int query_connected(string terrain, int *co_ords) {
  mapping connection_info, tmp;
  string connections;

  if (!float_cache[terrain] ||
      !(connection_info = float_cache[terrain][co_ords[0]])) {
    if (file_size(RESTORE_PATH + terrain) != -2) {
      return 0;
    }
    if (file_size(RESTORE_PATH + terrain +"/"+ co_ords[0]) == -1) {
      return 0;
    }
    connections = unguarded((: read_file,
                              RESTORE_PATH + terrain +"/"+ co_ords[0] :));
    connection_info = restore_variable(connections);
    if (!mapp(float_cache[terrain])) {
      float_cache[terrain] = ([ co_ords[0] : connection_info ]);
    } else {
      float_cache[terrain][co_ords[0]] = connection_info;
    }
  }
  if ((tmp = connection_info[co_ords[1]]) && tmp[co_ords[2]]) {
    return 1;
  }
  return 0;
} /* query_connected() */

/**
 * This method adds a new fixed location for a terrain.
 * @param terrain the terrain name
 * @param file the file name
 * @param co_ords the coordinates for the location
 * @return 1 if it succeeds, 0 if it fails (wrong coordinates or location
 * already present)
 * @see add_floating_location()
 * @see modify_fixed_location()
 */
int add_fixed_location(string terrain, string file, int *co_ords) {
  get_data_file(terrain);
  if (fixed_locations[file]) {
    return 0;
  }
  if (sizeof(co_ords) != 3) {
    return 0;
  }
  fixed_locations[file] = co_ords;
  save_data_file(terrain_name);
  return 1;
} /* add_fixed_location() */

private int add_connection(string terrain, int *co_ords, string direc,
                           string file) {
  mapping connection_info, tmp;
  string connections;

  if (!query_connection(terrain, co_ords, direc)) {
    if (file_size(RESTORE_PATH + terrain) != -2) {
      unguarded((: mkdir, RESTORE_PATH + terrain :));
    }
    if (file_size(RESTORE_PATH + terrain +"/"+ co_ords[0]) == -1) {
      connection_info = ([ co_ords[1] : ([ co_ords[2] : ([ direc : file ]) ])
                        ]);
      unguarded((: write_file, RESTORE_PATH + terrain + "/" + co_ords[0],
                   save_variable(connection_info), 1 :));
    } else {
      connections = unguarded((: read_file,
                                 RESTORE_PATH + terrain +"/"+ co_ords[0] :));
      connection_info = restore_variable(connections);
      if ((tmp = connection_info[co_ords[1]])) {
        if ((tmp = tmp[co_ords[2]])) {
          tmp[direc] = file;
        } else {
          connection_info[co_ords[1]][co_ords[2]] = ([ direc : file ]);
        }
      } else {
        connection_info[co_ords[1]] = ([ co_ords[2] : ([ direc : file ]) ]);
      }
      unguarded((: write_file, RESTORE_PATH + terrain + "/" + co_ords[0],
                   save_variable(connection_info), 1 :));
    }
    if (!mapp(float_cache[terrain])) {
      float_cache[terrain] = ([ co_ords[0] : connection_info ]);
    } else {
      float_cache[terrain][co_ords[0]] = connection_info;
    }
  }
} /* add_connection() */

/**
 * This method adds a new floating location for a terrain.
 * @param terrain the terrain name
 * @param file the file name
 * @param co_ords the coordinates for the location (either a single coordinate
 * or 2 forming a bounding rectangle)
 * @param level the level of this location
 * @return 1 if it succeeds, 0 if it fails (wrong coordinates or location
 * already present)
 * @see add_fixed_location()
 */
int add_floating_location(string terrain, string file, int *co_ords,
                          int level) {
  get_data_file(terrain);
  if ((sizeof(co_ords) != 6) && (sizeof(co_ords) != 3)) {
    return 0;
  }
  if (member_array(({ file, co_ords, level }), floating_locations) != -1) {
    return 0;
  }
  floating_locations += ({ ({ file, co_ords, level }) });
  save_data_file(terrain_name);
  return 1;
} /* add_floating_location() */

private void add_cloned_location(string terrain, string file, int *co_ords) {
  mapping tmp, location_m;

  if (!(location_m = cloned_locations[terrain])) {
    cloned_locations[terrain] = ([ file : co_ords,
                                   co_ords[0] : ([ co_ords[1] :
                                                   ([ co_ords[2] : file ]) ])
                                ]);
  } else {
    location_m[file] = co_ords;
    if ((tmp = location_m[co_ords[0]])) {
      if ((tmp = tmp[co_ords[1]])) {
        tmp[co_ords[2]] = file;
      } else {
        location_m[co_ords[0]][co_ords[1]] = ([ co_ords[2] : file ]);
      }
    } else {
      location_m[co_ords[0]] = ([ co_ords[1] : ([ co_ords[2] : file ]) ]);
    }
  }
} /* add_cloned_location() */

/**
 * This method modifies an existing fixed location for a terrain.
 * @param terrain the terrain name
 * @param file the file name
 * @param co_ords the coordinates for the location
 * @return 1 if it succeeds, 0 if it fails (wrong coordinates or location
 * not found)
 * @see add_fixed_location()
 */
int modify_fixed_location(string terrain, string file, int *co_ords) {
  get_data_file(terrain);
  if (!fixed_locations[file]) {
    return 0;
  }
  if (sizeof(co_ords) != 3) {
    return 0;
  }
  fixed_locations[file] = co_ords;
  save_data_file(terrain_name);
  return 1;
} /* modify_fixed_location() */

/**
 * This method deletes a cloned location for a terrain from the cache.
 * It is meant to be called when a cloned location is destructed.
 * @param terrain the terrain name
 * @param file the file name
 * @return 1 if it succeeds, 0 if it fails (location not present)
 * @see delete_fixed_location()
 * @see delete_floating_location()
 */
int delete_cloned_location(string terrain, string file) {
  int *co_ords;
  mapping tmp, location_m;
  
  if (!((location_m = cloned_locations[terrain]) &&
        (co_ords = location_m[file]))) {
    return 0;
  }
  map_delete(location_m, file);
  tmp = location_m[co_ords[0]][co_ords[1]];
  map_delete(tmp, co_ords[2]);
  if (!sizeof(tmp)) {
    tmp = location_m[co_ords[0]];
    map_delete(tmp, co_ords[1]);
    if (!sizeof(tmp)) {
      map_delete(location_m, co_ords[0]);
      if (!sizeof(location_m)) {
        map_delete(cloned_locations, terrain);
      }
    }
  }
  return 1;
} /* delete_cloned_location() */

/**
 * This method deletes a fixed location for a terrain.
 * @param terrain the terrain name
 * @param file the file name
 * @return 1 if it succeeds, 0 if it fails (location not present)
 * @see delete_cloned_location()
 * @see delete_floating_location()
 */
int delete_fixed_location(string terrain, string file) {
  get_data_file(terrain);
  if (!fixed_locations[file]) {
    return 0;
  }
  map_delete(fixed_locations, file);
  save_data_file(terrain_name);
  return 1;
} /* delete_fixed_location() */

/**
 * This method deletes a floating location for a terrain.
 * @param terrain the terrain name
 * @param file the file name
 * @param co_ords the coordinates for the location (either a single coordinate
 * or 2 forming a bounding rectangle)
 * @return 1 if it succeeds, 0 if it fails (location not present)
 * @see delete_cloned_location()
 * @see delete_fixed_location()
 */
int delete_floating_location(string terrain, string file, int *co_ords) {
  int i, j, flag_d, flag_m, *data;

  get_data_file(terrain);
  for (i = 0; i < sizeof(floating_locations); i++) {
    if (floating_locations[i][0] == file) {
      data = floating_locations[i][1];
      if (sizeof(data) != sizeof(co_ords)) {
        continue;
      }
      flag_m = 0;
      for (j = 0; j < sizeof(data); j++) {
        if (data[j] != co_ords[j]) {
          flag_m = 1;
          break;
        }
        if (!flag_m) {
          floating_locations = delete(floating_locations, i, 1);
          flag_d = 1;
        }
      }
    }
  }
  save_data_file(terrain_name);
  return flag_d;
} /* delete_floating_location() */

/**
 * This method clears the cloned locations cache for a given terrain.
 * @param terrain the terrain name
 * @see clear_connections()
 */
void clear_cloned_locations(string terrain) {
  map_delete(cloned_locations, terrain);
} /* clear_cloned_locations() */

/**
 * This method clears all connections for a given terrain. This needs to
 * be done when a terrain was modified.
 * @param terrain the terrain name
 * @see clear_cloned_locations()
 */
void clear_connections(string terrain) {
  string line, *lines;

  if (file_size(RESTORE_PATH + terrain) != -2) {
    return;
  }
  lines = unguarded((: get_dir, RESTORE_PATH + terrain +"/*" :)) -
    ({ ".", ".." });
  foreach(line in lines) {
    unguarded((: rm, RESTORE_PATH + terrain +"/"+ line :));
  }
} /* clear_connections() */

private int right_co_ords(int *new_co_ords, int *co_ords, int delta,
                          int *vector) {
  int i;

  for (i = 0; i < 3; i++) {
    if (new_co_ords[i] + delta * vector[i] != co_ords[i]) {
      return 0;
    }
  }
  return 1;
} /* right_co_ords() */

/**
 * This method returns the room size for a given room. It uses a cache
 * to speed things up.
 * @param file the file name of the room
 * @param level the recursion level
 * @return the size of the room
 */
int get_room_size(string file, int level) {
  int i, number, roomsize, *mapsize;
  string bname, parent, *lines;

  bname = base_name(file);
  if (size_cache[bname]) {
#ifdef DEBUG_ROOM_SIZE
    TP(sprintf("GRS says (cached) %s is %d\n", file, size_cache[bname]));
#endif
    return size_cache[bname];
  }

  if (find_object(file)) {
    size_cache += ([ bname : file->query_room_size() ]);
#ifdef DEBUG_ROOM_SIZE
    TP(sprintf("GRS says (loaded) %s is %d\n", file, size_cache[bname]));
#endif
    return size_cache[bname];
  }

  if (!in_map) {
    in_map = 1;
    mapsize = MAP_HANDLER->query_room_size(bname);
    in_map = 0;
  } else {
#ifdef DEBUG_ROOM_SIZE
    TP("GRS says recursion!\n");
#endif
    mapsize = 0;
  }
  if (mapsize) {
    size_cache += ([ bname : mapsize[0] ]);
#ifdef DEBUG_ROOM_SIZE
    TP(sprintf("GRS says (map) %s is %d\n", file, size_cache[bname]));
#endif
    return mapsize[0];
  }
  
  /* May I please throw up violently? - Turrican */
  file = bname + ".c";
  if (file_size(file) < 0) {
    return 10;
  }
  lines = explode(read_file(file), "\n");
  roomsize = 10;
  for (i = 0; i < sizeof(lines); i++) {
    if (level < 4 &&
        sscanf(lines[i], "%*sinherit%*s\"%s\"%*s;", parent) == 4) {
      if (parent[<2..] != ".c") {
        parent += ".c";
      }
      roomsize = get_room_size(parent, level + 1);
    } else if (sscanf(lines[i], "%*sset_room_size(%*s%d%*s", number) == 4) {
      roomsize = number;
      break;
    }
  }
  size_cache += ([ bname : roomsize ]);
#ifdef DEBUG_ROOM_SIZE
  TP(sprintf("GRS says (parsed) %s is %d\n", file, size_cache[bname]));
#endif
  return roomsize;
} /* get_room_size() */

private void add_exit(object place, string direc, string dest) {
  string type;

  type = (string)place->query_exit_type(direc, dest);
  if (!type) {
    type = std_types[direc];
  } else {
    if (type == "none") {
      return;
    }
  }
  place->add_exit(direc, dest, type);
} /* add_exit() */

private void calculate_exits(object place, int *co_ords) {
  int i, j, k, connected, delta, *new_co_ords;
  string actual, *exit_dirs;

  connected = query_connected(terrain_name, co_ords);
  exit_dirs = (string *)place->query_direc();
  for (i = 0; i < 20; i += 2) {
    if (member_array(STD_ORDERS[i], exit_dirs) != -1) {
      continue;
    }
    actual = query_connection(terrain_name, co_ords, STD_ORDERS[i]);
    if (actual) {
#ifdef DEBUG_CALC_EXITS
      TP(sprintf("actual room %s found\n", actual));
#endif
      add_exit(place, STD_ORDERS[i], actual);
      continue;
    }
    if (connected) {
#ifdef DEBUG_CALC_EXITS
      TP("connected, but no actual room\n");
#endif
      continue;
    }
    new_co_ords = copy(co_ords);
    for (k = 0; k < 3; k++) {
      new_co_ords[k] -= place->query_room_size() * STD_ORDERS[i+1][k];
    }
    reset_eval_cost();
    for (j = 0; j < 100; j++) {
      for (k = 0; k < 3; k++) {
        new_co_ords[k] -= 5 * STD_ORDERS[i+1][k];
      }
      if ((actual = member_fixed_locations(new_co_ords)) ||
          (actual = member_cloned_locations(new_co_ords)) ||
          (actual = top_floating_location(new_co_ords))) {
        delta = (int)place->query_room_size() + get_room_size(actual, 0);
        if (!right_co_ords(new_co_ords, co_ords, delta, STD_ORDERS[i+1])) {
          continue;
        }
#ifdef DEBUG_CALC_EXITS
        TP(sprintf("adding connection for %s\n", actual));
#endif
        add_connection(terrain_name, co_ords, STD_ORDERS[i],
                       base_name(actual));
        add_exit(place, STD_ORDERS[i], actual);
        break;
      }
    }
  }
} /* calculate_exits() */

/**
 * This method is the main entry point for the terrain handler. It loads
 * the room that is associated with the given terrain and coordinates.
 * @param terrain the terrain name
 * @param co_ords the coordinates to look up
 * @return the room that was found or loaded
 * @see setup_location()
 */
object find_location(string terrain, int *co_ords) {
  string dest_name;
  object destination;
   
  if (!(get_data_file(terrain)) || (sizeof(co_ords) != 3)) {
    return 0;
  }
  reset_eval_cost();
  if ((dest_name = member_fixed_locations(co_ords))) {
    if (!(destination = find_object(dest_name))) {
      destination = load_object(dest_name);
    }
    return destination;
  }
  if (dest_name = member_cloned_locations(co_ords)) {
    if ((destination = find_object(dest_name))) {
      return destination;
    } else {
      delete_cloned_location(terrain, dest_name);
    }
  }
  if (dest_name = top_floating_location(co_ords)) {
    destination = clone_object(dest_name);
    destination->set_co_ord(co_ords);
    destination->set_terrain(terrain_name);
    calculate_exits(destination, co_ords);
    add_cloned_location(terrain, file_name(destination), co_ords);
    return destination;
  }
  return 0;
} /* find_location() */

/**
 * This method is called when a normal room is loaded which is part of
 * a terrain. The terrain handler needs to know about this to be able
 * to calculate the exits into the floating portion of the terrain.
 * It also sets the coordinates of the room to the values as saved in the
 * terrain.
 * @param place the room object which is a fixed location in the terrain
 * @param terrain the terrain name
 * @see find_location()
 */
void setup_location(object place, string terrain) {
  int *co_ords;

  get_data_file(terrain);
  if (!fixed_locations[base_name(place)]) {
    return;
  }
  co_ords = fixed_locations[base_name(place)];
  place->set_co_ord(co_ords);
  calculate_exits(place, co_ords);
} /* setup_location() */

/** @ignore yes */
mixed *stats() {
  return ::stats() + ({
    ({ "roomsize cache size", sizeof(size_cache) }),
      ({ "cloned_locations", sizeof(cloned_locations) }),
        ({ "float_cache", sizeof(float_cache) }),
          ({ "terrain name", terrain_name }),
            ({ "fixed locations", sizeof(fixed_locations) }),
              ({ "floating locations", sizeof(floating_locations) }),
  });
}
