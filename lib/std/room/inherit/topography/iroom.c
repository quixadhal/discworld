/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: iroom.c,v 1.1 2000/05/01 03:14:49 jeremy Exp $
 *
 *
 */

/*
** iroom.c - file for creating interrooms.  NOTE: this file isn't used
**           by itself.  It is included in other files that inherit the
**           proper base room.  I HATE doing it this way, but I couldn't
**           think of a way to make the inheritance work out.
**/
/**
 * This file contains the functions for creating "irooms" (short for
 * "interrooms"), rooms
 * that exist between two (generally much larger) rooms.  The purpose
 * is to allow more resolution between widely-spaced rooms, such as
 * roads, without requiring the player to traverse them all.<p>
 * The two rooms on either end of a set of irooms (called "milestones")
 * are reached via normal exit directions (eg, "east").  The irooms are
 * reached via exits named with the same direction, prepended with a
 * "motion verb" (eg, "walk", giving an iroom exit of "walk east").<p>
 * <b>NOTE:<b> This file is not an object in itself.  It is meant to be
 * included in other files that implement the various types of rooms (I
 * know, I know: it's very ugly.  But it's the only way I could think to
 * handle the inheritance without copying this file in each room file).
 * @author Jeremy
 * @see milestone
 * @see interroom
 * @see topography
 */

#include <dirs.h>
#include <room.h>

int *milestone_index;
string *milestone_dirs = ({ });
string iroom_handler;
mapping shorten = SHORTEN;

string topo_handler;

string motion_verb = "walk", exit_type = "road", short_mse;
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
 * This method is deprecated.  Use set_topo_handler()
 */
void set_area_handler(string s) { topo_handler = s; }

/**
 * This method sets the topology handler for this iroom.
 * @param s pathname of topology handler
 * @see topology
 * @see query_topo_handler()
 */
void set_topo_handler(string s) { topo_handler = s; }

/**
 * This method returns the topology handler set for this iroom.
 * @return the topology handler for the iroom
 * @see set_topo_handler()
 */
string query_topo_handler() { return topo_handler; }

/**
 * This method sets the iroom handler for this section of irooms
 * @param s pathname of iroom handler
 * @see interroom
 * @see query_iroom_handler()
 */
void set_iroom_handler(string s) { iroom_handler = s; }

/**
 * This method returns the iroom handler for this section of irooms
 * @return the iroom handler for this section of rooms
 * @see set_iroom_handler()
 */
string query_iroom_handler() { return iroom_handler; }

/**
 * This method sets the iroom's motion verb, the verb to be used in the
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
 * This method sets the entrance message used when entering from an
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
 * leaving an iroom.  The format is the same as for normal moving
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
 * This method sets the exit type for the iroom exits.  The default is "road".
 * @param s exit type
 * @see query_exit_type()
 * @see add_exit()
 */
void set_exit_type(string s) { exit_type = s; }

/**
 * This method returns the exit type used for iroom exits.
 * @return exit type
 * @see set_exit_type()
 */
string query_exit_type() { return exit_type; }

/**
 * @ignore
 * This method is called by the interroom handler when it clones
 * the room.  The iroom is room # <milestone_index[1]>, between
 * <milestone_index[0]> and <milestone_index[0]+1> (eg, if
 * milestone_index == ({3, 6}), the room is iroom 6 between
 * milestones 3 and 4).
 */
void set_milestone_index(int *i) { milestone_index = i; }

/**
 * @ignore
 */
int *query_milestone_index() { return milestone_index; }

/**
 * @ignore
 * This method adds an iroom exit in the direction given.  The name comes
 * from the fact that that's the direction to one of the milestones.
 */
void add_milestone_dir(string s) {
  int i;
  string ss, em, xm, mm;

  i = sizeof(milestone_dirs);
  milestone_dirs += ({ s });
  if (!(iroom_handler)->query_endpoint(milestone_index[0],
                                       milestone_index[1], i)) {
    modify_exit(s, ({ "function", "passing_mess" }) );
  }
  add_exit(motion_verb + " " + s, "interroom", "hidden");
  if (!undefinedp(shorten[s])) {
    ss = shorten[s];
    add_alias(motion_verb + " " + ss, motion_verb + " " + s);
  } else {
    ss = s;
  }
  remove_exit(motion_verb + " <" + short_mse + ">");
  if (!short_mse) {
    short_mse = ss;
  } else {
    short_mse += "/" + ss;
  }
  add_exit(motion_verb + " <" + short_mse + ">", "interroom", exit_type);
  em = replace(iroom_enter_mess, ({ "$V", motion_verb, "$T", s, "$F",
                              opposites[s] }));
  xm = replace(iroom_exit_mess, ({ "$V", motion_verb, "$T", s, "$F",
                              opposites[s] }));
  mm = replace(iroom_move_mess, ({ "$V", motion_verb, "$T", s, "$F",
                              opposites[s] }));
  modify_exit(motion_verb + " " + s, ({
    "exit mess", xm,
    "enter mess", em,
    "move mess", mm
  }));
} /* add_milestone_dir() */

/**
 * @ignore
 * This method returns all of the milestone directions.
 */
string *query_milestone_dirs() { return milestone_dirs; }

/**
 * @ignore
 * This method is called when a player passes through more than one
 * iroom in a move.
 */
int passing_mess(string dir, object ob, string special_mess) {
  int i;
  object *obs, passed;
  string mess1, mess2;

  i = member_array(dir, milestone_dirs);
  if (i < 0) {
    return 0;
  }
  obs = (iroom_handler)->query_irooms_inv(milestone_index[0],
                                          milestone_index[1], i);
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
                                       opposites[dir], "$N", ob->a_short() }));
    foreach (passed in obs) {
      if (living(passed) && ob->query_visible(passed)) {
        tell_object(passed, mess2 + "\n");
      }
    }
  }
  return 1;
} /* passing_mess() */

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
 * of cloning one when a player logs in, and moving the player to it.
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
  new_room = (iroom_handler)->find_room_at_coord(
                 player->query_saved_co_ords());
  if (!objectp(new_room)) {
    tell_creator("jeremy", "Couldn't find iroom for %O at %O...\n",
                 player, player->query_saved_co_ords());
    player->move(player->query_start_pos());
    return;
  }
  tell_creator("jeremy", "Moving %O to %O...\n",
               player, new_room);
  player->move(new_room);
} /* enter() */

/**
 * This method masks the standard query_dest_other() function.  It handles
 * the motion verb for moving between irooms, and also the passing message
 * when moving through irooms.
 * @param direc room exit name
 * @return structure describing the exit
 * @see /std/room
 */
// This gives us time to clone the connecting room before the player
// moves into it.
varargs mixed *query_dest_other( string direc ) {
  int i;
  string verb, rest, *words;
  object room;
  mixed ret;

  //tell_creator("jeremy", "%O:query_dest_other(%s)\n", this_object(), direc);
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
  i = member_array(rest, milestone_dirs);
  if (i < 0) {
    notify_fail("You can't " + motion_verb + " that way.\n");
    return 0;
  }
  room = (iroom_handler)->find_room_at_index(milestone_index[0],
                                              milestone_index[1]+2*i-1);
  if (!objectp(room)) {
    return 0;
  }
  ret = ::query_dest_other(direc);
  if (!sizeof(ret)) {
    return 0;
  }
  ret[ROOM_DEST] = file_name(room);
  return ret;
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
  string s, direc_s;
  mixed dest;

  direc_s = direc;
  i = member_array(direc_s, milestone_dirs);
  if (i >= 0) {
    direc_s = motion_verb + " " + direc_s;
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
} /* query_dest_other() */

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
} /* add_topo_exits_int() */
