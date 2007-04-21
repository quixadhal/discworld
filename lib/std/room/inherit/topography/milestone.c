/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: milestone.c,v 1.1 2000/05/01 03:15:00 jeremy Exp $
 *
 *
 */

/*
** milestone.c - this is the "regular" room along a road.  See interroom.c
**           for more info.  NOTE: this file isn't used
**           by itself.  It is included in other files that inherit the
**           proper base room.  I HATE doing it this way, but I couldn't
**           think of a way to make the inheritance work out.
*/
/**
 * This file contains the functions for creating "milestones", the
 * rooms that serve as the endpoints for a section of irooms.  The purpose
 * is to allow more resolution between widely-spaced rooms, such as
 * roads, without requiring the player to traverse them all.<p>
 * Between each pair of milestones is an array of irooms (short for
 * "interrooms"); irooms are generally cloned as needed, while milestones
 * are generally unique rooms.  The irooms between two milestones are
 * reached via exits with the same names as those connecting the milestones,
 * but prepended with a "motion verb" (eg, "walk", giving an iroom exit of
 * "walk east").<p>
 * <b>NOTE:<b> This file is not an object in itself.  It is meant to be
 * included in other files that implement the various types of rooms (I
 * know, I know: it's very ugly.  But it's the only way I could think to
 * handle the inheritance without copying this file in each room file).
 * @author Jeremy
 * @see iroom
 * @see interroom
 * @see topography
 */

#include <dirs.h>
#include <room.h>

mapping milestone_dirs = ([ ]);
mapping milestone_index = ([ ]);
mapping shorten = SHORTEN;

string topo_handler;

// This is the "actual" size, relative to the interrooms (is it possible
// each direction could need a different size?  If so, I could query the
// handler).
int *actual_room_size;

string motion_verb = "walk", short_mse;
string iroom_exit_mess = "$N $V$s away to the $T.";
string iroom_enter_mess = "$N $V$s in from the $F.";
// Note that move messages don't do $N subs.
string iroom_move_mess = "You $V a little to the $T.\n";
string *pass_mess = ({
  "You $V a long while.",
  "$N passes by heading $T.",
  "  On the way you pass $O.\n"

});

// These are for unspecified exits
mixed other_exits = ({ });
string *other_types = ({ });

mapping look_mess = ([ ]);

mapping dirs = ([
  "north": ({ 1, 0, 0 }), "south": ({ -1, 0, 0 }),
  "east": ({ 0, 1, 0 }), "west" : ({ 0, -1, 0 }),
  "northeast": ({ 1, 1, 0 }), "northwest": ({ 1, -1, 0 }),
  "southeast": ({ -1, 1, 0 }), "southwest": ({ -1, -1, 0 }),
  "up": ({ 0, 0, 1 }), "down": ({ 0, 0, -1 })
]);

mapping opposites = ([
  "north": "south", "south": "north", "west": "east",
  "east": "west", "northeast": "southwest", "southwest": "northeast",
  "northwest": "southeast", "southeast": "northwest", "up": "down",
  "down": "up"
]);

/**
 * @ignore
 */
void add_topo_exits_int();

/**
 * This method is deprecated.  Use set_topo_handler()
 */
void set_area_handler(string s) { topo_handler = s; }

/**
 * This method sets the topography handler for this milestone.
 * @param s pathname of topography handler
 * @see topography
 * @see query_topo_handler()
 */
void set_topo_handler(string s) { topo_handler = s; }

/**
 * This method returns the topography handler set for this milestone.
 * @return the topography handler for the milestone
 * @see set_topo_handler()
 */
string query_topo_handler() { return topo_handler; }

/**
 * This method sets the room size used by the topography handler to
 * determine if the room is entered or crossed.  This is generally
 * the same as the iroom size.  The normal room
 * size (set with set_room_size()) is used to determine the distance
 * between milestones.
 * @param s room size (three elements)
 * @see set_room_size()
 * @see query_actual_room_size()
 */
void set_actual_room_size(int *s) { actual_room_size = copy(s); }

/**
 * This method returns the actual room size.
 * @return room size (three elements)
 * @see set_actual_room_size()
 */
int *query_actual_room_size() { return actual_room_size; }

/**
 * This method sets the milestone's motion verb, the verb to be used in the
 * iroom exit names; eg, "walk", "swim".  The default is "walk".
 * @param s motion verb
 * @see interroom
 * @see query_motion_verb()
 */
void set_motion_verb(string s) { motion_verb = s; }

/**
 * This method returns the motion verb.
 * @return the motion verb
 * @see set_motion_verb()
 */
string query_motion_verb() { return motion_verb; }

/**
 * This method sets the exit message for exits to irooms.  The format
 * is the same as for normal exit messages, except that $V can be used
 * to represent the motion verb.  The default is<pre><br>
 *     <code>"$N $V$s away to the $T."</code>
 * </pre>
 * @param s iroom exit message
 * @see query_iroom_exit_mess()
 * @see modify_exit()
 */
void set_iroom_exit_mess(string s) { iroom_exit_mess = s; }

/**
 * This method returns the iroom exit message.
 * @return iroom exit message
 * @see set_iroom_exit_mess()
 */
string query_iroom_exit_mess() { return iroom_exit_mess; }

/**
 * This method sets the entrance message used when entering an
 * iroom.  The format is the same as for normal entrance messages, except
 * that $V represents the motion verb.  The default is<pre><br>
 *     <code>"$N $V$s in from the $F."</code>
 * </pre>
 * @param s iroom entrance message
 * @see query_iroom_enter_mess()
 * @see modify_exit()
 */
void set_iroom_enter_mess(string s) { iroom_enter_mess = s; }

/**
 * This method returns the iroom entrance message.
 * @return iroom entrance message
 * @see set_iroom_enter_mess()
 */
string query_iroom_enter_mess() { return iroom_enter_mess; }

/**
 * This method sets the "moving message" shown to the player when
 * leaving by an iroom exit.  The format is the same as for normal moving
 * messages, except that $V represents the motion verb.  The default
 * is<pre><br>
 *     <code>"You $V a little to the $T.\n"</code>
 * </pre>
 * @param s iroom moving message
 * @see query_iroom_move_mess()
 * @see modify_exit()
 */
void set_iroom_move_mess(string s) { iroom_move_mess = s; }

/**
 * This method returns the iroom moving message.
 * @return iroom moving message
 * @see set_iroom_move_mess()
 */
string query_iroom_move_mess() { return iroom_move_mess; }

/**
 * This method sets the messages printed when passing through a number
 * of irooms in one move.  The parameter should be an array of three
 * strings: the first is the one printed to the moving player; the
 * second is printed to all living objects that are passed.  Both follow
 * the same format as the other iroom messages.  The third is a string which
 * is appended to the first if any objects are passed along the way;
 * $O can be used to represent the list of objects.  Any or all of the
 * strings can be set to 0, in which case that string will not be printed.
 * The default passing messages are<pre><br><code>
 *    ({ "You $V a long while.",
 *       "$N passes by heading $T.",
 *       "  On the way you pass $O.\n"
 * </code></pre>
 * @param s iroom passing messages (array of 3 strings)
 * @see query_pass_mess()
 * @see modify_exit()
 */
void set_pass_mess(string *s) { pass_mess = s; }

/**
 * This method returns the current iroom passing messages.
 * @return iroom move messages
 * @set_pass_mess()
 */
string *query_pass_mess() { return pass_mess; }

/**
 * This method adds or changes the message printed when looking in a
 * certain direction.  Messages set with modify_exit() have priority.
 * If neither are set, the room's description is used.
 * @param m a mapping of messages in the form ([<direc>:<mess>])
 */
void add_look_mess(mapping m) { look_mess += m; }

/**
 * This method queries the message printed when looking in a certain
 * direction.
 * @return a mapping of messages in the form ([<direc>:<mess>])
 * @see add_look_mess()
 */
mapping query_look_mess() { return look_mess; }

/**
 * @ignore
 * This method keeps track of the iroom exits associated with a handler.
 * It is called by the iroom handler.
 */
void add_milestone_dir(string handler, string dir) {
  if (undefinedp(milestone_dirs[handler])) {
    milestone_dirs[handler] = ({ dir });
  } else {
    milestone_dirs[handler] += ({ dir });
  }
}

/**
 * @ignore
 * Debugging function
 */
mixed query(string s) { return fetch_variable(s); }

// This should be called in setup() for each handler used by this milestone,
// with no index.  The actual index will be set by the handler
// when setup_milestone() is called.

/**
 * This method needs to initially be called in the milestone's setup()
 * to set the path for the iroom handler.  The index must be unspecified.
 * It is later called by the handler to set the index.<p>
 * The method may be called more than once if the milestone uses more
 * than one handler (eg, at a crossroads).
 * @param handler the path of the iroom handler
 * @param index <b>should not be specified</b>, except by the handler
 * @see interroom
 */
varargs void set_milestone_index(string handler, int index) {
  milestone_index[handler] = index;
}

/**
 * This method must be called *via a callout* in the room's create()
 * or setup().  Generally this is done high up in the inheritance tree.
 * This is to allow all of the parameters to be set up before the
 * iroom handler is called.
 * @see interroom
 */
void setup_milestone() {
  string handler;

  foreach (handler in keys(milestone_index)) {
    handler->setup_milestone(this_object());
  }
  add_topo_exits_int();
}

/**
 * This method should be called instead of add_exit() to connect
 * two milestones.  It handles setting up the exits to the irooms
 * as well as the adjacent milestone.
 * @param s direction
 * @param dest path to next milestone
 * @param type type of exit
 * @return status
 * @see add_exit()
 */
int add_milestone_exit(string s, string dest, string type) {
  string ss, em, xm, mm;

  add_exit(s, dest, type);
  add_exit(motion_verb + " " + s, "interroom", "hidden");
  if (!undefinedp(shorten[s])) {
    ss = shorten[s];
    add_alias(motion_verb + " " + ss, motion_verb + " " + s);
  } else {
    ss = s;
  }
  if (type != "hidden") {
    remove_exit(motion_verb + " <" + short_mse + ">");
    if (!short_mse) {
      short_mse = ss;
    } else {
      short_mse += "/" + ss;
    }
    add_exit(motion_verb + " <" + short_mse + ">", "interroom", type);
  }
  em = replace(iroom_enter_mess, ({ "$V", motion_verb, "$T", s, "$F",
                              opposites[s] }));
  xm = replace(iroom_exit_mess, ({ "$V", motion_verb, "$T", s, "$F",
                              opposites[s] }));
  mm = replace(iroom_move_mess, ({ "$V", motion_verb, "$T", s, "$F",
                              opposites[s] }));
  modify_exit(motion_verb + " " + s, ({
    "exit mess", "$N " + motion_verb + "$s away to the " + s + ".",
    "enter mess", "$N " + motion_verb + "$s in from the " + opposites[s] + ".",
    "move mess", "You " + motion_verb + " a little to the " + s + ".\n"
  }));
  return modify_exit(s, ({ "function", "passing_mess" }) );
}

/**
 * @ignore
 * This method is called when a player passes through more than one
 * iroom in a move.
 */
int passing_mess(string dir, object ob, string special_mess) {
  int i;
  object *obs, passed;
  string mess1, mess2, handler;

  foreach (handler in keys(milestone_dirs)) {
    i = member_array(dir, milestone_dirs[handler]);
    if (i < 0) {
      continue;
    }
    obs = handler->query_irooms_inv(milestone_index[handler], -1, i);
    if (stringp(pass_mess[0])) {
      mess1 = replace(pass_mess[0], ({ "$V", motion_verb, "$T", dir, "$F",
                                         opposites[dir] }));
      if (stringp(pass_mess[2]) && sizeof(obs)) {
        mess1 += replace(pass_mess[2], "$O", query_multiple_short(obs));
      }
      mess1 += "\n";
      tell_object(ob, mess1);
    }
    if (stringp(pass_mess[1])) {
      mess2 = replace(pass_mess[1], ({ "$V", motion_verb, "$T", dir, "$F",
                                         opposites[dir], "$N",
                                         ob->a_short() }));
      foreach (passed in obs) {
        if (living(passed) && ob->query_visible(passed)) {
          tell_object(passed, mess2 + "\n");
        }
      }
    }
    // No use going through the rest of the loop...
    return 1;
  }
  return 1;
}

/**
 * This method masks the standard query_dest_other() function.  It handles
 * the motion verb for iroom exits, and also the passing message
 * when moving through irooms.
 * @param direc room exit name
 * @return structure describing the exit
 * @see /std/room
 */
// This gives us time to clone the connecting room before the player
// moves into it.
varargs mixed *query_dest_other( string direc ) {
  int i;
  string verb, rest, *words, handler;
  object room;
  mixed ret;

  if (!stringp(direc)) {
    return ::query_dest_other(direc);
  }
  words = explode(direc, " ");
  verb = words[0];
  if (verb != motion_verb) {
    ret = ::query_dest_other(direc);
    if (!stringp(ret[ROOM_DEST]) || (ret[ROOM_DEST] == "/") ||
    (ret[ROOM_DEST] == "/terrain") || (ret[ROOM_DEST] == "/topography")) {
      room = (topo_handler)->find_room_at_exit(this_object(), dirs[direc],
                                               direc);
      if (!objectp(room)) {
        return 0;
      }
      ret[ROOM_DEST] = file_name(room);
    }
    return ret;
  }
  if (sizeof(words) < 2) {
    notify_fail(motion_verb + " which way?\n");
    return 0;
  }
  rest = implode(words[1..], " ");
  foreach (handler in keys(milestone_dirs)) {
    i = member_array(rest, milestone_dirs[handler]);
    if (i < 0) {
      continue;
    }
    if (i == 0) {
      room = handler->find_room_at_index(milestone_index[handler] - 1, -2);
    } else {
      room = handler->find_room_at_index(milestone_index[handler], 0);
    }
    if (!objectp(room)) {
      return 0;
    }
    ret = ::query_dest_other(direc);
    if (!sizeof(ret)) {
      return 0;
    }
    ret[ROOM_DEST] = file_name(room);
    return ret;
  }
  return 0;
} /* query_dest_other() */

/**
 * This method masks the standard query_look().  If the exit is along
 * an iroom path, motion_verb is appended to it.  If a "look" message has
 * been set with modify_exit(), it is returned.  Next, the look_mess
 * mapping is checked; if there is an entry for this exit, it is evaluated
 * and returned.  Finally, the room itself is loaded and the description
 * is returned.
 * @param direc exit name
 * @return the description of the adjoining room
 * @see modify_exit()
 * @see add_look_mess()
 */
string query_look(string direc) {
  int i, dark;
  string s, direc_s, h;
  mixed dest;

  direc_s = direc;
  foreach (h in keys(milestone_dirs)) {
    i = member_array(direc_s, milestone_dirs[h]);
    if (i >= 0) {
      direc_s = motion_verb + " " + direc_s;
      break;
    }
  }
  if (s = ::query_look(direc_s)) {
    return s;
  }
  if (s = look_mess[direc_s]) {
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
 * This method adds exits for the iroom if they don't already exist.
 * "compass4" and "compass8" can be specified as exit names to add
 * exits for all of the normal 4 or 8 compass directions, respectively.
 * Actually, the add_exit is deferred to add_topo_exits_int, so that these can
 * be added after the milestone exits.
 * @param e array of exit names
 * @param type type of exit ("path", "road", etc)
 */
void add_topo_exits(string *e, string type) {
  other_exits += ({ e });
  other_types += ({ type });
}

/**
 * @ignore
 * A dummy exit name of "/topography" is used to avoid null strings.
 */
void add_topo_exits_int() {
  int i;
  string s;

  for (i = 0; i < sizeof(other_exits); i++) {
    foreach (s in other_exits[i]) {
      if ((s == "compass4") || (s == "compass8")) {
        if (!query_exit("north")) add_exit("north", "/topography",
                                           other_types[i]);
        if (!query_exit("south")) add_exit("south", "/topography",
                                           other_types[i]);
        if (!query_exit("east")) add_exit("east", "/topography",
                                          other_types[i]);
        if (!query_exit("west")) add_exit("west", "/topography",
                                          other_types[i]);
        if (s == "compass8") {
          if (!query_exit("northeast")) add_exit("northeast", "/topography",
                                                 other_types[i]);
          if (!query_exit("northwest")) add_exit("northwest", "/topography",
                                                 other_types[i]);
          if (!query_exit("southeast")) add_exit("southeast", "/topography",
                                                 other_types[i]);
          if (!query_exit("southwest")) add_exit("southwest", "/topography",
                                                 other_types[i]);
        }
      } else {
        add_exit(s, "/topography", other_types[i]);
      }
    }
  }
}
