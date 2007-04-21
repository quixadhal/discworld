/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: aroom.c,v 1.4 2001/05/10 22:41:48 pinkfish Exp $
 *
 *
 */

/*
** area.c - file for creating areas.  NOTE: this file isn't used
**           by itself.  It is included in other files that inherit the
**           proper base room.  I HATE doing it this way, but I couldn't
**           think of a way to make the inheritance work out.
**/
/**
 * This file contains the functions used in rooms that make up "areas",
 * arbitrary polygons consisting of identical rooms.  This is similar
 * to the terrain system, but with certain advantages: the areas can
 * be any polygon, even non-convex ones; areas are integrated into the
 * topology system, which also handles roads through areas.<p>
 * Although all of the rooms in an area are cloned from the same file,
 * a number of tricks can be used to add variety (but that's beyond the
 * scope of this document).
 * @author Jeremy
 * @see area
 * @see topography
 */
#include <dirs.h>
#include <room.h>

private string _topo_handler;
private mapping _look_mess = ([ ]);

mapping dirs = ([
  "north": ({ 1, 0, 0 }), "south": ({ -1, 0, 0 }),
  "east": ({ 0, 1, 0 }), "west" : ({ 0, -1, 0 }),
  "northeast": ({ 1, 1, 0 }), "northwest": ({ 1, -1, 0 }),
  "southeast": ({ -1, 1, 0 }), "southwest": ({ -1, -1, 0 }),
  "up": ({ 0, 0, 1 }), "down": ({ 0, 0, -1 })
]);

//
// Function pre-defs.
//
int* query_co_ord();
mixed* query_dest_other(string direc);
int query_door_open(string direc);
int query_exit(string str);
void add_exit(string dir, string location, string type);

/**
 * @ignore yes
 * This method is deprecated.  Use set_topo_handler()
 */
void set_area_handler(string s) { _topo_handler = s; }

/**
 * This method sets the topology handler for this area.  A simple area
 * handler can be used rather than a topology handler in limited cases.
 * @param s pathname of topology handler
 * @see topology
 * @see area
 * @see query_topo_handler()
 */
void set_topo_handler(string s) { _topo_handler = s; }
/**
 * This method returns the topology handler set for this room.
 * @return the topology handler for the room
 * @see set_topo_handler()
 */
string query_topo_handler() { return _topo_handler; }

/**
 * This method adds or changes the message printed when looking in a
 * certain direction.  Messages set with modify_exit() have priority.
 * If neither are set, the room's description is used.
 * @param m a mapping of messages in the form ([<direc>:<mess>])
 */
void add_look_mess(mapping m) { _look_mess += m; }
/**
 * This method returns the mapping associated with the looking messages.
 * @return the look message
 */
mapping query_look_mess() { return _look_mess; }

/**
 * This method is called when the player quits, to save the current
 * room.  It uses a little-known alternate form that returns the
 * room path and the coordinates.
 * @return the iroom path and coordinates
 */
mixed query_quit_handler() {
  string s;

  s = explode(file_name(environment(this_player())), "#")[0];
  return ({ s, query_co_ord() });
} /* query_quit_handler() */

/**
 * This method is called at login, to move the player to the room.
 * Since these rooms are always clones, the master has the responsibility
 * of cloning itself when a player logs in, and moving the player to it.
 * @param player the player logging in
 */
void enter(object player) {
  object new_room;

  // I used to use clonep() here, but the "parent" of virtual rooms were
  // coming up as clones.
  if (sizeof(explode(file_name(this_object()), "#")) > 1) {
    // We somehow called enter() from a clone...
    return;
  }
  new_room = (_topo_handler)->find_room_at_coord(
                 player->query_saved_co_ords());
  if (!objectp(new_room)) {
    tell_creator("jeremy", "Couldn't find area room for %O at %O...\n",
                 player, player->query_saved_co_ords());
    player->move(player->query_start_pos());
    return;
  }
  tell_creator("jeremy", "Moving %O to %O...\n",
               player, new_room);
  player->move(new_room);
} /* enter() */

// This gives us time to clone the connecting room before the player
// moves into it.
/**
 * This method masks the standard query_dest_other() function.  It calls
 * the topology handler to find the connecting room (which is usually
 * cloned on-the-fly).
 * @param direc room exit name
 * @return structure describing the exit
 * @see /std/room
 */
varargs mixed *query_dest_other( string direc, mixed* other_res) {
  object room;
  mixed ret;

//printf("%O %O %O\n", dirs, direc, other_res);
  //tell_creator("jeremy", "%O:query_dest_other(%s)\n", this_object(), direc);
  if (!stringp(direc)) {
    return other_res;
  }
  room = (_topo_handler)->find_room_at_exit(this_object(), dirs[direc],
                                           direc);
//printf("%O %O %O\n", dirs[direc], direc, room);
  if (!objectp(room)) {
    return 0;
  }
  ret = other_res;
  if (!sizeof(ret)) {
    return 0;
  }
  ret[ROOM_DEST] = file_name(room);
  return ret;
} /* query_dest_other() */

/**
 * This method masks the standard query_look().  If a "look" message has
 * been set with modify_exit(), it is returned.  Next, the look_mess
 * mapping is checked; if there is an entry for this exit, it is evaluated
 * and returned.  Finally, the room itself is loaded and the description
 * is returned.
 * @param direc exit name
 * @return the description of the adjoining room
 * @see modify_exit()
 * @see add_look_mess()
 */
string query_look(string direc, string other_result) {
  int dark;
  string s;
  string direc_s;
  mixed dest;

  direc_s = direc;
  if (other_result) {
    return s;
  }
  s = _look_mess[direc_s];
  if (s) {
    return evaluate(s);
  }
  dest = query_dest_other(direc_s);
  if (!dest) {
    return 0;
  }
  dest = dest[ROOM_DEST];
  if (!dest) {
    return 0;
  }
  if (query_door_open(direc_s) != 0) {
    // it mustn't be dark in the destination room.
    dest->force_load();
    this_player()->set_looked( find_object( dest ) );
    dark = (int)this_player()->check_dark((int)dest->query_light());
    s = (string)dest->long( 0, dark );
    if (s[<1] == '\n') {
      // The look command automatically adds a '\n'
      return s[0..<2];
    }
    return s;
  }
  return 0;
} /* query_look() */

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
      if (!query_exit("north")) {
        add_exit("north", "/topography", other_types);
      }
      if (!query_exit("south")) {
        add_exit("south", "/topography", other_types);
      }
      if (!query_exit("east")) {
        add_exit("east", "/topography", other_types);
      }
      if (!query_exit("west")) {
        add_exit("west", "/topography", other_types);
      }
      if (s == "compass8") {
        if (!query_exit("northeast")) {
          add_exit("northeast", "/topography", other_types);
        }
        if (!query_exit("northwest")) {
          add_exit("northwest", "/topography", other_types);
        }
        if (!query_exit("southeast")) {
          add_exit("southeast", "/topography", other_types);
        }
        if (!query_exit("southwest")) {
          add_exit("southwest", "/topography", other_types);
        }
      }
    } else {
      add_exit(s, "/topography", other_types);
    }
  }
} /* add_topo_exits() */
