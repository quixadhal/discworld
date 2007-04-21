/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: fixed_shadow.c,v 1.2 2002/12/17 05:03:28 rhinehold Exp $
 *
 *
 */

/*
** fixed_shadow.c - shadow for fixed rooms in topographies, so the rooms
**                  can find area rooms and such.
**/
/**
 * This is a shadow that gets attached to "normal" (ie, fixed) rooms that
 * used in a region controlled by a topography handler.  It masks
 * query_dest_other() so that exits from the room get handled correctly.
 * It also adds certain other functions used by the topography system.
 * This allows a fixed room to be converted for use in a topography with
 * minimal changes.
 * @author Jeremy
 * @see topography
 */

#include <dirs.h>
#include <room.h>

string topo_handler;
object fixed_room;
mapping topo_barriers = ([ ]);

mapping dirs = ([
  "north": ({ 1, 0, 0 }), "south": ({ -1, 0, 0 }),
  "east": ({ 0, 1, 0 }), "west" : ({ 0, -1, 0 }),
  "northeast": ({ 1, 1, 0 }), "northwest": ({ 1, -1, 0 }),
  "southeast": ({ -1, 1, 0 }), "southwest": ({ -1, -1, 0 }),
  "up": ({ 0, 0, 1 }), "down": ({ 0, 0, -1 })
]);

/**
 * This method is deprecated.  Use set_topo_handler().
 */
void set_area_handler(string s) { topo_handler = s; }

/**
 * This method sets the topography handler for this fixed room.
 * @param s pathname of topography handler
 * @see topography
 * @see query_topo_handler()
 */
void set_topo_handler(string s) { topo_handler = s; }

/**
 * @ignore
 */
string query_area_handler() { return topo_handler; }

/**
 * This method returns the topography handler set for this fixed room.
 * @return the topography handler
 * @see set_topo_handler()
 */
string query_topo_handler() { return topo_handler; }

/**
 * This method adds directions from which the room cannot be reached.
 * The parameter is a mapping, giving the direction as the key and a
 * message to be printed upon failure as the value.<p>
 * NOTE: the direction is the direction the person is going (ie, the
 * command given); e.g., to block access from the west, use "east" as the
 * key.
 * @param m a mapping representing the direction(s) from which to block
 * access (the key) and a message to be printed (the value).
 * @see query_topo_barriers()
 * @see query_topo_barrier()
 */
void add_topo_barriers(mapping m) { topo_barriers += m; }

/**
 * This method returns the current mapping of barriers.
 * @return the current mapping of barred directions and messages.
 * @see add_topo_barriers()
 * @see query_topo_barrier()
 */
mapping query_topo_barriers() { return topo_barriers; }

/**
 * This method returns the message associated with the given barred
 * direction.
 * @param direc the direction to be queried
 * @return the failure message associated with the given direction
 * @see add_topo_barriers()
 * @see query_topo_barriers()
 */
mixed query_topo_barrier(string direc) { return topo_barriers[direc]; }

/**
 * This method can be used to determine if this is a fixed room.
 * @return 1
 */
int query_fixed_room() { return 1; }

/**
 * This method masks the standard query_dest_other() function.  It calls
 * the topology handler to find the connecting room (which is usually
 * cloned on-the-fly).
 * @param direc room exit name
 * @return structure describing the exit
 * @see /std/room
 */
// This gives us time to clone the connecting room before the player
// moves into it.
varargs mixed *query_dest_other( string direc ) {
  object room;
  mixed ret;

  //tell_creator("jeremy", "%O:query_dest_other(%s)\n", this_object(), direc);
  if (!stringp(direc)) {
    return fixed_room->query_dest_other(direc);
  }
  ret = fixed_room->query_dest_other(direc);
  if (!sizeof(ret)) {
    return 0;
  }
  if (!stringp(ret[ROOM_DEST]) || (ret[ROOM_DEST] == "/") ||
  (ret[ROOM_DEST] == "/terrain") || (ret[ROOM_DEST] == "/topography")) {
    room = (topo_handler)->find_room_at_exit(this_object(),
                           dirs[direc], direc);
    if (!objectp(room)) {
      return 0;
    }
    ret[ROOM_DEST] = file_name(room);
  }
  return ret;
} /* query_dest_other() */

/**
 * @ignore
 */
void attach(object ob) {
  shadow(ob);
  fixed_room = ob;
} /* attach() */


/**
 * This method adds exits for the room if they don't already exist.
 * "compass4" and "compass8" can be specified as exit names to add
 * exits for all of the normal 4 or 8 compass directions, respectively.
 * @param e array of exit names
 * @param type type of exit ("path", "road", etc)
 */
// A dummy exit name of "/topography" is used to avoid null strings.
void add_topo_exits(string *other_exits, string other_types) {
  string s;

  foreach (s in other_exits) {
    if ((s == "compass4") || (s == "compass8")) {
      if (!this_object()->query_exit("north"))
        this_object()->add_exit("north", "/topography", other_types);
      if (!this_object()->query_exit("south"))
        this_object()->add_exit("south", "/topography", other_types);
      if (!this_object()->query_exit("east"))
        this_object()->add_exit("east", "/topography", other_types);
      if (!this_object()->query_exit("west"))
        this_object()->add_exit("west", "/topography", other_types);
      if (s == "compass8") {
        if (!this_object()->query_exit("northeast"))
          this_object()->add_exit("northeast", "/topography", other_types);
        if (!this_object()->query_exit("northwest"))
          this_object()->add_exit("northwest", "/topography", other_types);
        if (!this_object()->query_exit("southeast"))
          this_object()->add_exit("southeast", "/topography", other_types);
        if (!this_object()->query_exit("southwest"))
          this_object()->add_exit("southwest", "/topography", other_types);
      }
    } else {
      this_object()->add_exit(s, "/topography", other_types);
    }
  }
} /* add_topo_exits() */
