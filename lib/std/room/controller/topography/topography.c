/*  -*- LPC -*-  */
/*
 * $Locker: pinkfish $
 * $Id: topography.c,v 1.9 2002/02/28 23:13:52 pinkfish Exp pinkfish $
 *
 *
 */

/*
 * $Locker: pinkfish $
 * $Id: topography.c,v 1.9 2002/02/28 23:13:52 pinkfish Exp pinkfish $
 *
 *
 */

/*
** topography.c - the top-level handler that pulls together the areas,
**             irooms, and fixed rooms.
*/
/**
 * This is the top-level handler for the topography system.
 */
#include <dirs.h>
#include "path.h"

//inherit "/std/room";
inherit "/std/basic/setup";

class bounding_box {
   int* topleft;
   int* bottomright;
}

class FIXED_INFO {
  string path;
  object ob;
  int *coord;
  mixed bbox;
}

private mapping opposites = ([
  "north": "south", "south": "north", "west": "east",
  "east": "west", "northeast": "southwest", "southwest": "northeast",
  "northwest": "southeast", "southeast": "northwest", "up": "down",
  "down": "up"
]);

private int _dbg_lvl = 0;
private string _debugger;
private object *_area_handlers = ({ });
private object *_iroom_handlers = ({ });
private class FIXED_INFO *_fixed_rooms = ({ });
private mapping _fixed_room_index = ([ ]);
private class bounding_box _topo_box;

/**
 * This method sets the creator to whom debugging messages are sent.
 * @param d name of creator
 */
void set_debugger(string s) { _debugger = s; }

/**
 * This method returns the name of the creator to whom debugging messages
 * are sent.
 * @return name of creator
 */
string query_debugger() { return _debugger; }

/**
 * This method sets the level of debugging messages printed.
 * @param l debugging level (0 is off)
 */
void set_debug_level(int l) { _dbg_lvl = l; }

/**
 * This method returns the current debugging level.
 * @return current debug level (0 is off)
 */
int query_debug_level() { return _dbg_lvl; }

/**
 * @ignore
 * For debugging only
 */
//mixed query(string s) { return fetch_variable(s); }

void create() {

  //inc_setup();
  //::create();
  //dec_setup();

  do_setup();
} /* create() */

/**
 * This method returns the bounding box for the topography.
 * @return the bounding box
 */
class bounding_box query_bounding_box() {
   return _topo_box;
} /* query_bounding_box() */

/**
 * Checks to see if the co-ordinate is in the bounding box.
 * @param coord the co-ordinate to check
 * @param bbox the bounding box
 * @return 1 if it is, 0 if not
 */
int in_bbox(int *coord, class bounding_box bbox) {
  if ((coord[0] < bbox->topleft[0]) || (coord[0] > bbox->bottomright[0])) {
    return 0;
  }
  if ((coord[1] < bbox->topleft[1]) || (coord[1] > bbox->bottomright[1])) {
    return 0;
  }
  return 1;
} /* in_bbox() */

/**
 * This method makes a bounding box for a specified co ordinate and
 * a polygon.
 * @param coord the co-ordinate to get the box from
 * @param s the size (or integer square size) of the room
 * @return the bounding box
 */
class bounding_box bounding_box(int *coord, mixed s) {
  int *size;
  class bounding_box bbox;

  if (sizeof(coord) != 3) {
    return 0;
  }
  if (arrayp(s)) {
    if (sizeof(s) == 3) {
      size = s;
    } else {
      return 0;
    }
  } else if (intp(s)) {
    size = ({ s, s, s });
  } else {
    return 0;
  }
  bbox = new(class bounding_box);
  bbox->topleft = ({ coord[0]-size[0], coord[1]-size[1], coord[2]-size[2] });
  bbox->bottomright = ({ coord[0]+size[0], coord[1]+size[1], coord[2]+size[2] });
  return bbox;
} /* bounding_box() */

/**
 * This method adds in an area handler.
 * @param h the area handle to add
 */
void add_area_handler(mixed h) {
  object oh;

  if (stringp(h)) {
    oh = load_object(h);
    oh->set_handler(base_name(this_object()));
    if (objectp(oh)) {
      _area_handlers += ({ oh });
    } else {
      error(sprintf("Couldn't find area handler %s.\n", h));
    }
  } else if (objectp(h)) {
    h->set_handler(base_name(this_object()));
    _area_handlers += ({ h });
  } else {
    error(sprintf("Couldn't find area handler %O.\n", h));
  }
} /* add_area_handler() */

/**
 * This method adds in an inter-room handler.  An interroom is something like
 * a path that has a rectangular bounding box allowing multiple rooms to
 * connect to it.
 * @param h the interoom handler
 */
void add_iroom_handler(mixed h) {
  object oh;

  if (stringp(h)) {
    oh = load_object(h);
    oh->set_handler(base_name(this_object()));
    if (objectp(oh)) {
      _iroom_handlers += ({ oh });
    } else {
      error(sprintf("Couldn't find interroom handler %s.\n", h));
    }
  } else if (objectp(h)) {
    h->set_handler(base_name(this_object()));
    _iroom_handlers += ({ h });
  } else {
    error(sprintf("Couldn't find interroom handler %O.\n", h));
  }
} /* add_iroom_handler() */

/**
 * This is the internal function to add a fixed location room.
 * @param path the path to the object
 * @param ob the object itself
 * @param coord the coordinate of the object
 * @param bbox the bounding box of the object
 */
private void add_internal_fixed_room(string path,
                                     object ob,
                                     int* coord,
                                     class bounding_box bbox) {
  class FIXED_INFO cf;

  cf = new(class FIXED_INFO);
  cf->path = path;
  cf->ob = ob;
  cf->coord = coord;
  cf->bbox = bbox;
  _fixed_room_index[cf->path] = sizeof(_fixed_rooms);
  _fixed_rooms += ({ cf });
} /* add_internal_fixed_room() */

/**
 * This method adds a room in a fixed location.  It gets the co-ordinate
 * and size information from the room itself.
 * @param f the room to add
 * @add add_fixed_room_object()
 * @add add_fixed_room_coord()
 */
void add_fixed_room(string f) {
  object of;

  of = load_object(f);
  if (!objectp(of)) {
    error(sprintf("Couldn't load fixed room %s.\n", f));
    return;
  }
  add_internal_fixed_room(f, of, of->query_co_ord(),
                      bounding_box(of->query_co_ord(), of->query_room_size()));
} /* add_fixed_room() */

/**
 * This method adds a fixed room from an object.
 * @param of the object file
 * @add add_fixed_room()
 * @add add_fixed_room_coord()
 */
void add_fixed_room_object(object of) {
  add_internal_fixed_room(file_name(of), of, of->query_co_ord(),
                    bounding_box(of->query_co_ord(), of->query_room_size()));
} /* add_fixed_room() */

/**
 * This method adds a fixed room with a room and a co-ordinate.
 * @param f the file name of the room
 * @param coord the coordinate of the room
 * @param size the size of the room
 * @add add_fixed_room_object()
 * @add add_fixed_room()
 */
void add_fixed_room_coordinate(string f, int* coord, int* size) {
    add_internal_fixed_room(f, 0, coord, bounding_box(coord, size));
} /* add_fixed_room_coordinate() */

/**
 * This method is called from a loaded object to setup the fixed room
 * into the topology.
 */
void setup_fixed_room() {
  object shad, r;
  int i;

  r = previous_object();
  shad = clone_object(FIXED_SHADOW);
  if (!objectp(shad)) {
    error(sprintf("Couldn't create shadow for fixed room %O.\n", r));
    return;
  }
  shad->attach(r);
  shad->set_area_handler(file_name(this_object()));
  if (undefinedp(i = _fixed_room_index[file_name(r)])) {
    error(sprintf("Couldn't find entry for fixed room %O.\n", r));
  }
  r->set_co_ord(_fixed_rooms[i]->coord);
  //tell_creator("jeremy", "%O coords = %O\n", r, r->query_co_ord());
} /* setup_fixed_room() */

/**
 * This method finds the room as the specified co-ordinates.
 * @param coord the coordinate to look up the room at
 * @return the room
 */
object find_room_at_coord(int *coord) {
  int i;
  object ob;

  if (_dbg_lvl > 0) {
    tell_creator(_debugger, "%s::find_room_at_coord()\n"
                 "  coord: %O\n",
                 file_name(this_object()), coord);
  }
  // Check fixed rooms first...
  for (i = 0; i < sizeof(_fixed_rooms); i++) {
    if (in_bbox(coord, _fixed_rooms[i]->bbox)) {
      if (!objectp(_fixed_rooms[i]->ob)) {
        _fixed_rooms[i]->ob = load_object(_fixed_rooms[i]->path);
      }
      return _fixed_rooms[i]->ob;
    }
  }
  // ...then irooms...
  for (i = 0; i < sizeof(_iroom_handlers); i++) {
    if (objectp(ob = _iroom_handlers[i]->find_room_at_coord(coord))) {
      return ob;
    }
  }
  // ...finally, check areas...
  for (i = 0; i < sizeof(_area_handlers); i++) {
    if (objectp(ob = _area_handlers[i]->find_room_at_coord(coord))) {
      return ob;
    }
  }
  return 0;
} /* find_room_at_coord() */

/**
 * This method does a virtual object lookup for the room.
 * @param x the x coord
 * @param y the y coord
 * @param z the z coord
 */
object create_virtual_object(int x, int y, int z) {
   if (undefinedp(x) || undefinedp(y) || undefinedp(z)) {
      return 0;
   }

   return find_room_at_coord(({ x, y, z }));
} /* create_virtual_object() */

/**
 * This method finds the room at the specified exit in the specified room.
 * @param r the room to look up
 * @param d the dimension of the room
 * @param direc the direction we are moving in
 * @return the room at the exit
 */
object find_room_at_exit(object r, int *d, string direc) {
  int i, s, dist, max_dist, max_iroom, *coord, *coord_r, *coord_ob;
  string msg;
  object ob, *irooms = ({ });
  mixed size_r;
  class bounding_box bbox_r;

  if (_dbg_lvl > 0) {
    tell_creator(_debugger, "%s::find_room_at_exit()\n"
                 "  r: %O\n"
                 "  d: %O\n"
                 "  direc: %O\n",
                 file_name(this_object()), r, d, direc);
  }
  if (!arrayp(d)) {
    return 0;
  }
  coord = r->query_co_ord();
  // Some rooms (like milestones) have "fake" sizes.
  size_r = r->query_actual_room_size();
  if (!arrayp(size_r)) {
    size_r = r->query_room_size();
    if (!arrayp(size_r)) {
      // Must be a single int for all 3 dimensions.
      size_r = ({ size_r, size_r, size_r });
    }
  }
  coord_r = allocate(3);
  for (i = 0; i < 3; i++) {
    // This assumes the rooms are adjacent, either on an edge or at
    // a corner (note to self: figure out why this seems to hit areas
    // before irooms).
    //coord_r[i] = coord[i] + (size_r[i] + 1) * d[i];
    coord_r[i] = coord[i] + 2 * size_r[i] * d[i];
  }
  // Check fixed rooms first...
  for (i = 0; i < sizeof(_fixed_rooms); i++) {
    if (in_bbox(coord_r, _fixed_rooms[i]->bbox)) {
      if (!objectp(_fixed_rooms[i]->ob)) {
        _fixed_rooms[i]->ob = load_object(_fixed_rooms[i]->path);
      }
      // Check that the fixed room allows entrances from this direction.
      msg = evaluate(_fixed_rooms[i]->ob->query_topo_barrier(direc));
      if (!undefinedp(msg)) {
        if (!stringp(msg)) {
          msg = "You can't go that way.\n";
        }
        notify_fail(msg);
        return 0;
      }
      return _fixed_rooms[i]->ob;
    }
  }
  // ...then irooms...
  // This is a bit complicated.  I need to check the distance to
  // the rooms returned by all the handlers and pick the closest.
  bbox_r = bounding_box(coord, size_r);
  for (i = 0; i < sizeof(_iroom_handlers); i++) {
    ob = _iroom_handlers[i]->find_room_at_crossing(coord, coord_r);
    tell_creator("rhinehold", "ob = %O\n", ob);
    if (objectp(ob /*(= _iroom_handlers[i]->find_room_at_crossing(coord, coord_r)*/)
        && (ob != r)) {
      if (_dbg_lvl > 1) {
        tell_creator(_debugger, "%s::find_room_at_exit()\n"
                     "  %O->find_room_at_crossing() returned %O\n",
                     file_name(this_object()), _iroom_handlers[i], ob);
      }
      // This is a kludge, but I can't think of a way around it.  Milestones
      // (like all normal rooms), get their coords set in a callout, so
      // I can't calculate the distance here.  I'll have to assume that
      // a room without coordinates is the "correct" one.
      if (!arrayp(ob->query_co_ord())) {
        return ob;
      }
      // Ignore irooms that are in the current room's bounding box, if
      // the current room is a fixed room.
      // *** NOTE: I'm not sure if this is sufficient.  There are cases
      // where I want irooms to take precedence (like the path around
      // the maze), but there are others where the current room should
      // take precedence (like s_cara03b).  Hopefully, the latter case
      // is always a fixed room.
      if (r->query_fixed_room() && in_bbox(ob->query_co_ord(), bbox_r)) {
        continue;
      }
      irooms += ({ ob });
    }
  }
  if ((s = sizeof(irooms)) > 0) {
    if (s == 1) {
      return irooms[0];
    } else {
      // Hopefully this won't happen often...
      max_dist = 0;
      max_iroom = -1;
      for (i = 0; i < s; i++) {
        coord_ob = irooms[i]->query_co_ord();
        dist = (coord[0] - coord_ob[0])*(coord[0] - coord_ob[0]) +
          (coord[1] - coord_ob[1])*(coord[1] - coord_ob[1]);
        if (dist > max_dist) {
          max_dist = dist;
          max_iroom = i;
        }
      }
      if (max_iroom >= 0) {
        return irooms[max_iroom];
      }
    }
  }
  // ...finally, check areas...
  for (i = 0; i < sizeof(_area_handlers); i++) {
    if (objectp(ob = _area_handlers[i]->find_room_at_exit(r, d))) {
      if (_dbg_lvl > 1) {
        tell_creator(_debugger, "%s::find_room_at_exit()\n"
                     "  %O->find_room_at_exit() returned %O\n",
                     file_name(this_object()), _area_handlers[i], ob);
      }
      return ob;
    }
  }
  return 0;
} /* find_room_at_exit() */

// This is a testing convenience function
/**
 * This method allows you to be moved quickly to a room at the specified
 * co-ordinate.
 * @param coord the coordinate to go to
 */
int goto_room_at_coord(int *coord) {
  return this_player()->move_with_look(find_room_at_coord(coord));
} /* goto_room_at_coord() */

/**
 * This method will make a map of the specified width based on the data in
 * the topography area.
 * @param width the width of the map
 * @return the map
 */
string query_text_map(int width) {
   int x;
   int y;
   int maxy;
   string map;
   object room;

   map = "";
   for (y = 0; y < maxy; y++) {
      for (x = 0; x < width; x++) {
         room = find_room_at_coord( ({ x, y, 0 }) );
         if (!room) {
            map += " ";
         } else if (!room->query_room_letter()) {
            map += "?";
         } else {
            map += room->query_room_letter();
         }
      }
      map += "\n";
   }
   return map;
} /* query_text_map() */
