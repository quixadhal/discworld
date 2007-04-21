/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: interroom.c,v 1.2 2002/02/28 23:26:13 rhinehold Exp $
 *
 *
 */

/*
** interroom.c - a handler to handle being "in between" the regular rooms
**     on a long road.
**
** The milestones are the "normal" rooms that people travel between when
** moving compass directions.  milestone_coords[] is a cache of the
** milestone coordinates.
**
** irooms[][] is an array of arrays of rooms; one array of rooms for
** each segment of the road (one less than the number of milestone_coords).
**
** bases[] is an array of base rooms to clone for the interrooms (one for
** each gap between the milestones).
*/

/**
 * The interroom handler is a scheme for handling multiple levels of
 * detail along piecewise linear paths (eg, roads).  The vertices of
 * the path (called "milestones") behave like the rooms along a
 * traditional road.  However, between each milestone are a number of
 * "in-between" rooms (called "interrooms" or "irooms").  Irooms are
 * generally cloned as needed, while milestones are generally unique
 * rooms.  The irooms between two milestones are reached via exits
 * with the same names as those connecting the milestones, but
 * prepended with a "motion verb" (eg, "walk", giving iroom exits of
 * "walk east", "walk west", etc.).<p>
 *
 * The interroom handler is a sort of "mini-handler".  It is meant to
 * be used as an inheritable, with each path having its own handler.
 * Although there is no limit to the number of milestones along a
 * given path, a few points should be kept in mind:
 * <ul>
 * <li> Each milestone can be specified only once in a given handler,
 * except the first and last, which may be identical (ie, you can have
 * a loop, but not a figure-eight).
 *
 * <li> The calculations are based on the bounding box of the path.  A
 * long relatively straight road (ie, north/south or east/west is
 * fairly efficient, but a mostly diagonal road is the least
 * efficient.  So a road that's straight on one half and diagonal on
 * the other half might be better broken into two paths.
 * </ul>
 * Each handler specifies the milestones, the rooms to clone for the
 * irooms (the irooms in each segment are clones of the same room),
 * the sizes of the irooms (again, the irooms in each segment are of the
 * same size), and the save file to use for caching information (like
 * room coordinates).
 *
 * In the documentation, directions along the path are sometimes referred
 * to as "to the left" or "to the right".  These mean "toward the lower
 * numberd milestone" or "toward the higher numbered milestone",
 * respectively, regardless of the compass direction of the path.
 *
 * @author Jeremy
 * @see iroom
 * @see milestone
 * @see topography
 */

#include <map.h>

inherit "/std/room";

class IROOM_INFO {
  string base;
  int *size;
  mixed *bbox;  // Keeps track of bounding box of iroom segment
  int sgn_n;  // Keeps track of N/S direction (N = 1)
  int sgn_e;  // Keeps track of E/W direction (E = 1)
  string *exits; // In order of direction to milestone[i], milestone[i+1]
}

int closed;  // this path forms a closed loop
nosave int dbg_lvl = 0;
mixed *irooms;  // an array of arrays of objects
string *milestones;  // milestone room names
mixed *milestone_coords;  // cache of milestone coordinates
mapping milestone_idx = ([ ]);  // mapping of room name to index
class IROOM_INFO *iroom_info;
mixed *overall_bbox;
string save_file;
nosave string debugger = "jeremy";

mixed *bbox(int *coord_a, int *coord_b);
int in_bbox(int *coord_a, mixed bbox);

/**
 * This method sets the creator to whom debugging messages are sent.
 * @param d name of creator
 */
void set_debugger(string s) { debugger = s; }

/**
 * This method returns the name of the creator to whom debugging messages
 * are sent.
 * @return name of creator
 */
string query_debugger() { return debugger; }

/**
 * This method sets the level of debugging messages printed.
 * @param l debugging level (0 is off)
 */
void set_debug_level(int l) { dbg_lvl = l; }

/**
 * This method returns the current debugging level.
 * @return current debug level (0 is off)
 */
int query_debug_level() { return dbg_lvl; }

/**
 * @ignore
 * For debugging only
 */
mixed query(string s) { return fetch_variable(s); }

/**
 * @ignore
 * Integer absolute value function.
 */
int abs(int i) {
  return (i<0)?-i:i;
}

/**
 * @ignore
 * Float absolute value function.
 */
float fabs(float i) {
  return (i<0)?-i:i;
}

/**
 * This method sets the path of the file to use for saving data that
 * shouldn't have to be recalculated very often.
 * @param s pathname of save file
 */
void set_save_file(string s) { save_file = s; }

/**
 * This method returns the path of the file used for saving.
 * return pathname of save file
 */
string query_save_file() { return save_file; }

/**
 * This method is used to set the pathnames of the milestones.  Each
 * milestone should inherit one of the milestone inheritables.
 * @param array of filenames of the milestones for this path.
 * @see milestone
 */
void set_milestones(string *s) {
  int i;

  milestones = copy(s);
  if (milestones[0] == milestones[<1]) {
    // This is a closed loop
    closed = 1;
  }
  for (i = 0; i < sizeof(milestones) - closed; i++) {
    milestone_idx[s[i]] = i;
  }
  milestone_coords = allocate(sizeof(milestones));
  irooms = allocate(sizeof(milestones)-1);
  iroom_info = allocate(sizeof(milestones)-1);
  for (i = 0; i < sizeof(iroom_info); i++) {
    iroom_info[i] = new(class IROOM_INFO);
    iroom_info[i]->exits = allocate(2);
  }
  overall_bbox = 0;
} /* set_milestones() */

/**
 * This method returns the array of milestone filenames.
 * @return an array of milestone filenames
 * @see set_milestones()
 */
string *query_milestones() { return milestones; }

/**
 * This method returns the filename of the given milestone.
 * @param i index number of milestone
 * @returns filename of milestone i
 * @see set_milestones()
 */
string query_milestone(int i) { return milestones[i]; }

/**
 * @ignore
 * This method is used internally to set (or update) the coordinates
 * of a milestone.  It also updates related info, such as the bounding
 * boxes.
 */
void set_milestone_coords(int i, int *c) {

  if (!arrayp(c) || (sizeof(c) != 3)) {
    return;
  }
  if (!arrayp(milestone_coords[i]) || (milestone_coords[i][0] != c[0])
  || (milestone_coords[i][1] != c[1]) || (milestone_coords[i][2] != c[2])) {
    // Update iroom_info on either side of milestone
    milestone_coords[i] = copy(c);
    if ((i > 0) && arrayp(milestone_coords[i-1])) {
      iroom_info[i-1]->bbox = bbox(milestone_coords[i-1], milestone_coords[i]);
      if (milestone_coords[i-1][0] < milestone_coords[i][0]) {
        iroom_info[i-1]->sgn_n = 1;
      } else if (milestone_coords[i-1][0] > milestone_coords[i][0]) {
        iroom_info[i-1]->sgn_n = -1;
      } else {
        iroom_info[i-1]->sgn_n = 0;
      }
      if (milestone_coords[i-1][1] < milestone_coords[i][1]) {
        iroom_info[i-1]->sgn_e = 1;
      } else if (milestone_coords[i-1][1] > milestone_coords[i][1]) {
        iroom_info[i-1]->sgn_e = -1;
      } else {
        iroom_info[i-1]->sgn_e = 0;
      }
      // Update overall bounding box (do I need to add in room size?)
      if (!arrayp(overall_bbox)) {
        overall_bbox = copy(iroom_info[i-1]->bbox);
      } else {
        if (iroom_info[i-1]->bbox[0][0] < overall_bbox[0][0]) {
          overall_bbox[0][0] = iroom_info[i-1]->bbox[0][0];
        }
        if (iroom_info[i-1]->bbox[0][1] < overall_bbox[0][1]) {
          overall_bbox[0][1] = iroom_info[i-1]->bbox[0][1];
        }
        if (iroom_info[i-1]->bbox[1][0] > overall_bbox[1][0]) {
          overall_bbox[1][0] = iroom_info[i-1]->bbox[1][0];
        }
        if (iroom_info[i-1]->bbox[1][1] > overall_bbox[1][1]) {
          overall_bbox[1][1] = iroom_info[i-1]->bbox[1][1];
        }
      }
    }
    if ((i < sizeof(iroom_info)) && arrayp(milestone_coords[i+1])) {
      iroom_info[i]->bbox = bbox(milestone_coords[i], milestone_coords[i+1]);
      if (milestone_coords[i][0] < milestone_coords[i+1][0]) {
        iroom_info[i]->sgn_n = 1;
      } else if (milestone_coords[i][0] > milestone_coords[i+1][0]) {
        iroom_info[i]->sgn_n = -1;
      } else {
        iroom_info[i]->sgn_n = 0;
      }
      if (milestone_coords[i][1] < milestone_coords[i+1][1]) {
        iroom_info[i]->sgn_e = 1;
      } else if (milestone_coords[i][1] > milestone_coords[i+1][1]) {
        iroom_info[i]->sgn_e = -1;
      } else {
        iroom_info[i]->sgn_e = 0;
      }
      // Update overall bounding box (do I need to add in room size?)
      if (!arrayp(overall_bbox)) {
        overall_bbox = copy(iroom_info[i]->bbox);
      } else {
        if (iroom_info[i]->bbox[0][0] < overall_bbox[0][0]) {
          overall_bbox[0][0] = iroom_info[i]->bbox[0][0];
        }
        if (iroom_info[i]->bbox[0][1] < overall_bbox[0][1]) {
          overall_bbox[0][1] = iroom_info[i]->bbox[0][1];
        }
        if (iroom_info[i]->bbox[1][0] > overall_bbox[1][0]) {
          overall_bbox[1][0] = iroom_info[i]->bbox[1][0];
        }
        if (iroom_info[i]->bbox[1][1] > overall_bbox[1][1]) {
          overall_bbox[1][1] = iroom_info[i]->bbox[1][1];
        }
      }
    }
    //printf("Setting room %d to %O\n", i, c);
  }
} /* set_milestone_coords() */

/**
 * @ignore
 * This method returns the array of coordinates for the milestones.
 */
mixed query_milestone_coords() { return milestone_coords; }

/**
 * This method is used to set the filenames of the rooms to clone for
 * the irooms.  Each segment of the path (ie, between two milestones)
 * can use a different base room.  Each base room should inherit one
 * of the iroom inheritables.
 * @param s array of filenames of irooms
 * @see iroom
 */
void set_bases(string *s) {
  int i;

  if (sizeof(s) != sizeof(iroom_info)) {
    error("Wrong number of bases (" + sizeof(s) + ") for given milestones (" +
          sizeof(milestones) + ").\n");
    return;
  }
  for (i = 0; i < sizeof(s); i++) {
    iroom_info[i]->base = s[i];
  }
} /* set_bases() */

/**

 * This method is used to set the sizes of the irooms.  Each segment
 * of the path (ie, between two milestones) can use a different size.
 * However, the distance between milestones must be an integer
 * multiple of the size (in each dimension).  The number of irooms in
 * each segment is calculated automatically.
 * @param s array of sizes of irooms (n x 1 or n x 3)
 * @see iroom
 */
void set_sizes(mixed *s) {
  int i;

  if (sizeof(s) != sizeof(iroom_info)) {
    error("Wrong number of sizes (" + sizeof(s) + ") for given milestones (" +
          sizeof(milestones) + ").\n");
    return;
  }
  for (i = 0; i < sizeof(s); i++) {
    iroom_info[i]->size = allocate(3);
    if (intp(s[i])) {
      iroom_info[i]->size[0] = s[i];
      iroom_info[i]->size[1] = s[i];
      iroom_info[i]->size[2] = s[i];
    } else if (arrayp(s[i])) {
      iroom_info[i]->size[0] = s[i][0];
      iroom_info[i]->size[1] = s[i][1];
      iroom_info[i]->size[2] = s[i][2];
    } else {
      error("Illegal room size given (" + i + ").\n");
      continue;
    }
  }
} /* set_sizes() */

/**
 * @ignore
 */
class IROOM_INFO *query_iroom_info() { return iroom_info; }

/**
 * This method determines if iroom j (between milestones i & i+1) is
 * just before a milestone when going in direction dir.  If dir == 0,
 * the direction is towards milestone i (ie, to the "left"); if
 * dir == 1, the direction is towards milestone i+1 (ie, to the "right").
 * @param i milestone "to the left" of the iroom
 * @param j iroom index
 * @param dir direction
 * @return true if the iroom is just before a milestone in direction dir
 * @see query_irooms_inv()
 */
int query_endpoint(int i, int j, int dir) {
  // See query_irooms_inv() for comments on dir
  if ((dir && (j == sizeof(irooms[i])-1)) || (!dir && (j == 0))) {
    return 1;
  }
  return 0;
}

/**
 * This method returns all of the objects in the rooms between this
 * room and the next milestone (non-inclusive).  This is used when the
 * player uses the exit directly to the next milestone, to inform the
 * player of what [s]he passed along the way.<p>
 * 'dir' is a bit of a kludge.  If it is 1, the movement is to milestone
 * i+1; if it is 0, the movement is to milestone i, <i>unless</i> j == -1,
 * in which case the movement is to milestone i-1 (this latter case is
 * only used when the current room is a milestone).
 * @param i milestone "to the left" of the current iroom
 * @param j iroom index (or -1 if a milestone)
 * @param dir direction
 * @return an array of the objects between this room and the next milestone
 */
object *query_irooms_inv(int i, int j, int dir) {
  // dir==1 -> moving towards milestone i+1
  // dir==0 ->
  //     if j == -1 -> moving towards milestone i-1 (only used by milestones)
  //     else       -> moving towards milestone i
  int k;
  object *invs;

  invs = ({ });
  if (dir) {
    if (!arrayp(irooms[i])) {
      return invs;
    }
    for (k = j+1; k < sizeof(irooms[i]); k++) {
      //tell_creator("jeremy", "Checking irooms[%d][%d]\n", i, k);
      if (objectp(irooms[i][k])) {
        invs += all_inventory(irooms[i][k]);
      }
    }
  } else {
    if (j == -1) {
      if (closed) {
        // Wraparound for closed path
        i = sizeof(milestones) - 2;
      } else {
        i--;
      }
      if (!arrayp(irooms[i])) {
        return invs;
      } else {
        j = sizeof(irooms[i]);
      }
    } else if (!arrayp(irooms[i])) {
      return invs;
    }
    for (k = j-1; k >= 0; k--) {
      //tell_creator("jeremy", "Checking irooms[%d][%d]\n", i, k);
      if (objectp(irooms[i][k])) {
        invs += all_inventory(irooms[i][k]);
      }
    }
  }
  return invs;
} /* query_irooms_inv() */

/**
 * This method returns the coordinates of a rectangle that bounds the
 * given points.  The parameters are each arrays of three integers.  The
 * returned value is a 2-element array, each element being an array of
 * three integers.  The first element is the minimum values (ie, the
 * lower-southwestern corner) and the second is the maximum values (the
 * upper-northeastern corner).
 * @param coord_a the coordinates of one point
 * @param coord_b the coordinates of the other point
 * @return the bounding box
 */
mixed *bbox(int *coord_a, int *coord_b) {
  // Finds rectangle that bounds the given points.  Returns a 2-element
  // array of coordinates (min, max);
  int c;
  mixed *bbox;

  bbox = allocate(2);
  bbox[0] = allocate(3);
  bbox[1] = allocate(3);
  for (c = 0; c < 3; c++) {
    if (coord_a[c] < coord_b[c]) {
      bbox[0][c] = coord_a[c];
      bbox[1][c] = coord_b[c];
    } else {
      bbox[0][c] = coord_b[c];
      bbox[1][c] = coord_a[c];
    }
  }
  return bbox;
} /* bbox() */

/**
 * This method returns 1 if the given coordinates lie within the given
 * bounding box.  coord_a is an array of three integers (north,
 * east, up).  Currently, the "up" value is not checked.  bbox is a
 * two-element array of coordinates.
 * @param coord_a the coordinates of the point to check
 * @param bbox the bounding box to check the point against
 * @return true if the point lies within the bounding box
 */
int in_bbox(int *coord_a, mixed bbox) {
  // Note: this does not check the height (3rd coordinate)
  if ((coord_a[0] < bbox[0][0]) || (coord_a[0] > bbox[1][0])
  || (coord_a[1] < bbox[0][1]) || (coord_a[1] > bbox[1][1]))
    return 0;
  return 1;
} /* in_bbox() */

/**
 * This method calculates the number of irooms between milestone i and
 * milestone i+1.
 * @param i "left-hand" milestone
 * @return the number of irooms between the milestones
 */
int number_of_irooms(int i) {
  int n;
  mixed bbox;
  class IROOM_INFO ir;

  ir = iroom_info[i];
  bbox = ir->bbox;
  if (ir->sgn_n == 0) {
    n = (bbox[1][1] - bbox[0][1])/(ir->size[1]*2);
  } else if (ir->sgn_e == 0) {
    n = (bbox[1][0] - bbox[0][0])/(ir->size[0]*2);
  } else {
    // Diagonal
    n = (bbox[1][1] - bbox[0][1])/(ir->size[1]*2);
    if (n != (bbox[1][0] - bbox[0][0])/(ir->size[0]*2)) {
      error("Interroom sizes don't match diagonal.\n");
      return 0;
    }
  }
  return n-1;
} /* number_of_irooms() */

/**
 * This method clones an iroom at the requested location, and sets up
 * the pertinent details.
 * @param i milestone "to the left" of the iroom
 * @param idx index of this iroom within its segment
 * @param icoord the coordinates of the new iroom
 * @return the cloned iroom
 */
object clone_iroom(int i, int idx, int *icoord) {
  object iroom;

  iroom = clone_object(iroom_info[i]->base);
  // Set up new room
  iroom->set_room_size(iroom_info[i]->size);
  iroom->set_co_ord(icoord);
  iroom->set_milestone_index( ({ i, idx }) );
  iroom->add_exit(iroom_info[i]->exits[0], milestones[i], "road");
  iroom->add_exit(iroom_info[i]->exits[1], milestones[i+1], "road");
  iroom->add_milestone_dir(iroom_info[i]->exits[0]);
  iroom->add_milestone_dir(iroom_info[i]->exits[1]);
  // This adds the topo exits that were set up with add_topo_exits();
  // this way, everything else is all set up.
  iroom->add_topo_exits_int();
  return iroom;
} /* clone_iroom() */

/**
 * This method is called to set up a milestone.  It sets up the coordinates
 * and some of the exit information.
 * @param ms milestone to be set up
 */
void setup_milestone(object ms) {
  int *coord, i, j, idir;
  string *dir;

  if (undefinedp(i = milestone_idx[file_name(ms)])) {
    error("This room (" + file_name(ms) + ") is not a milestone.\n");
    return;
  }
  ms->set_milestone_index(file_name(this_object()), i);
  coord = ms->query_co_ord();
  if (arrayp(coord)) {
    set_milestone_coords(i, coord);
  } else {
    if (arrayp(milestone_coords[i])) {
      coord = milestone_coords[i];
    } else {
      coord = MAP->static_query_co_ord(file_name(ms));
      set_milestone_coords(i, coord);
    }
    ms->set_co_ord(coord);
  }
  if (closed && (i == 0)) {
    milestone_coords[<1] = milestone_coords[0];
  }
  dir = ms->query_dest_dir();
  if ((i > 0) || closed) {
    // Left-hand room exit
    if (closed && i == 0) {
      // Wraparound for closed path
      j = sizeof(milestones) - 2;
    } else {
      j = i - 1;
    }
    if ((idir = member_array(milestones[j], dir)) < 0) {
      // This is no longer an error, but we still need an entry.
      //error("No exit to " + milestones[j] + " from " + milestones[i] +
      //      ".\n");
      ms->add_milestone_dir(file_name(this_object()), 0);
    } else {
      iroom_info[j]->exits[0] = dir[idir-1];
      ms->add_milestone_dir(file_name(this_object()), dir[idir-1]);
    }
  } else {
    // Need a dummy entry
    ms->add_milestone_dir(file_name(this_object()), 0);
  }
  if (i < sizeof(irooms)) {
    // Right-hand room exit
    j = i + 1;
    if ((idir = member_array(milestones[j], dir)) < 0) {
      // This is no longer an error, but we still need an entry.
      //error("No exit to " + milestones[j] + " from " + milestones[i] +
      //      ".\n");
      ms->add_milestone_dir(file_name(this_object()), 0);
    } else {
      iroom_info[i]->exits[1] = dir[idir-1];
      ms->add_milestone_dir(file_name(this_object()), dir[idir-1]);
    }
  }
} /* setup_milestone() */

/**
 * This method finds the room along the path that contains the given point.
 * Obviously, things will go wrong if the rooms somehow overlap.
 * @param coord_a the coordinates of the point
 * @param hint_idx indicates that only this index should be checked
 * (optional)
 * @return the room that contains coord_a, or 0 if no matches
 */
varargs object find_room_at_coord(int *coord_a, int hint_idx) {
  int i, idx, in_bbox;
  int start_i, stop_i;
  int *icoord = ({ 0, 0, 0 });
  mixed bbox;
  float slope;
  class IROOM_INFO ir;

  if (dbg_lvl > 0) {
    tell_creator(debugger, "%s::find_room_at_coord()\n"
                 "  coord_a: %O\n"
                 "  hint_idx: %s\n",
                 file_name(this_object()), coord_a,
                 undefinedp(hint_idx)?"NULL":hint_idx+"");
  }
  // The only way I can think to do this is check every pair of
  // adjacent milestones; I originally planned a binary-search type of
  // check, but I think I'd still need to scan all the milestones to find
  // the bounding boxes.
  // First check that we're in the neighborhood
  if (!in_bbox(coord_a, overall_bbox)) {
    return 0;
  }
  in_bbox = 0;
  // See if the choice has been narrowed down for us
  if (undefinedp(hint_idx)) {
    start_i = 0;
    stop_i = sizeof(iroom_info);
  } else {
    start_i = hint_idx;
    stop_i = hint_idx + 1;
  }
  for (i = start_i; i < stop_i; i++) {
    ir = iroom_info[i];
    bbox = ir->bbox;
    if (dbg_lvl > 1) {
      tell_creator(debugger, "%s::find_room_at_coord()\n"
                   "  Checking bbox(%d,%d): %O\n",
                   file_name(this_object()), i, i+1, bbox);
    }
    if (in_bbox(coord_a, bbox)) {
      if (dbg_lvl > 1) {
        tell_creator(debugger, "%s::find_room_at_coord()\n"
                     "  In bbox(%d,%d)...\n",
                     file_name(this_object()), i, i+1);
      }
      // Check that the coordinates lie on the line
      if (ir->sgn_e == 0) {
        // N/S (vertical) line
        if (abs(coord_a[1]-milestone_coords[i][1]) < ir->size[1]) {
          if (dbg_lvl > 2) {
            tell_creator(debugger, "%s::find_room_at_coord()\n"
                         "  On vline between (%d,%d)...\n",
                         file_name(this_object()), i, i+1);
          }
          break;
        }
      } else {
        slope = to_float(milestone_coords[i][0] - milestone_coords[i+1][0])
          / to_float(milestone_coords[i][1] - milestone_coords[i+1][1]);
        if (abs(slope * (coord_a[1] - milestone_coords[i+1][1]) +
            milestone_coords[i+1][0] - coord_a[0]) < ir->size[0]) {
          if (dbg_lvl > 2) {
            tell_creator(debugger, "%s::find_room_at_coord()\n"
                         "  On line between (%d,%d)...\n",
                         file_name(this_object()), i, i+1);
          }
          break;
        }
      }
    }
  }
  if (i == stop_i) {
    // Couldn't find one
    if (dbg_lvl > 2) {
      tell_creator(debugger, "%s::find_room_at_coord()\n"
                   "  Couldn't find a room...\n",
                   file_name(this_object()));
    }
    return 0;
  }
  // We now know what rooms the iroom is between...
  if (ir->sgn_n == 0) {
    // East/west road
    idx = abs(coord_a[1] - milestone_coords[i][1]
              + ir->size[1]*ir->sgn_e)/(ir->size[1]*2);
    icoord[0] = milestone_coords[i][0];
    icoord[1] = milestone_coords[i][1] + idx*ir->sgn_e*ir->size[1]*2;
  } else {
    idx = abs(coord_a[0] - milestone_coords[i][0]
              + ir->size[0]*ir->sgn_n)/(ir->size[0]*2);
    icoord[0] = milestone_coords[i][0] + idx*ir->sgn_n*ir->size[0]*2;
    if (ir->sgn_e == 0) {
      icoord[1] = milestone_coords[i][1];
    } else {
      icoord[1] = milestone_coords[i][1] + idx*ir->sgn_e*ir->size[1]*2;
    }
  }
  if (dbg_lvl > 2) {
    tell_creator(debugger, "%s::find_room_at_coord()\n"
                 "  idx: %O\n",
                 file_name(this_object()), idx);
  }
  if (idx == 0) {
    // We hit right on a milestone
    return load_object(milestones[i]);
  }
  if (!arrayp(irooms[i])) {
    irooms[i] = allocate(number_of_irooms(i));
  }
  if (idx == sizeof(irooms[i])+1) {
    // We hit on the other end
    return load_object(milestones[i+1]);
  }
  // Decrement idx to index irooms[i] (since idx==0 is the milestone)
  idx--;
  if (!objectp(irooms[i][idx])) {
    icoord[2] = (milestone_coords[i+1][2] - milestone_coords[i][2])
      * idx / sizeof(irooms[i]) + milestone_coords[i][2];
    // Clone new iroom
    irooms[i][idx] = clone_iroom(i, idx, icoord);
  }
  return irooms[i][idx];
} /* find_room_at_coord() */

/**
 * This method returns the room at the given index, or 0 if there is
 * no such room.  Generally, idx is the index of the iroom within its
 * segment; the following are for special cases:<ul>
 * <li> -1 means milestone i
 * <li> sizeof(irooms[i]) means milestone i+1
 * <li> -2 means the last iroom of this segment
 * <ul>
 * @param i milestone "to the left" of the room (but see above)
 * @param idx index of iroom (but see above)
 * @return the room at the given index
 */
object find_room_at_index(int i, int idx) {
  // A couple of kludges:
  //    idx == -1 means milestone i
  //    idx == sizeof(irooms[i]) means milestone i+1
  //    idx == -2 means the last iroom of this segment
  int *icoord = ({ 0, 0, 0 });
  class IROOM_INFO ir;

  if (dbg_lvl > 0) {
    tell_creator(debugger, "%s::find_room_at_index()\n"
                 "  i: %O, idx: %O\n",
                 file_name(this_object()), i, idx);
  }
  if (closed && (i == -1)) {
    // Assume we wrapped around on a closed path
    i = sizeof(milestones) - 2;
  }
  if ((i < 0) || (i >= sizeof(irooms))) {
    return 0;
  }
  if (idx == -1) {
    return load_object(milestones[i]);
  }
  if (!arrayp(irooms[i])) {
    irooms[i] = allocate(number_of_irooms(i));
  }
  if (idx == sizeof(irooms[i])) {
    return load_object(milestones[i+1]);
  }
  if (idx == -2) {
    idx = sizeof(irooms[i]) - 1;
  }
  if ((idx < 0) || (idx >= sizeof(irooms[i]))) {
    return 0;
  }
  if (!objectp(irooms[i][idx])) {
    ir = iroom_info[i];
    if (ir->sgn_n == 0) {
      // East/west road
      icoord[0] = milestone_coords[i][0];
      icoord[1] = milestone_coords[i][1] + (idx+1)*ir->sgn_e*ir->size[1]*2;
    } else {
      icoord[0] = milestone_coords[i][0] + (idx+1)*ir->sgn_n*ir->size[0]*2;
      if (ir->sgn_e == 0) {
        icoord[1] = milestone_coords[i][1];
      } else {
        icoord[1] = milestone_coords[i][1] + (idx+1)*ir->sgn_e*ir->size[1]*2;
      }
    }
    icoord[2] = (milestone_coords[i+1][2] - milestone_coords[i][2])
      * idx / sizeof(irooms[i]) + milestone_coords[i][2];
    // Clone new iroom
    irooms[i][idx] = clone_iroom(i, idx, icoord);
  }
  return irooms[i][idx];
} /* find_room_at_index() */

/**
 * This method determines which side(s) of bbox the given point is on,
 * setting a bit in the return value as follows:<ul>
 * <li> bit 0: south
 * <li> bit 1: north
 * <li> bit 2: west
 * <li> bit 3: east
 * <\ul>
 * This effectively divides the plane into nine regions.  If two points
 * have the same region number, then their line can't cross bbox.  An
 * easy way to check this is to bitwise-and ("&") the query_region()
 * for two points: if the result is non-zero, then the line between
 * the points can't cross bbox (note however that a result of zero doesn't
 * guarantee that they do cross bbox).
 * @param coord the coordinates of the point
 * @param bbox the bounding box
 * @return an int indicating on which side of bbox the point lies
 */
int query_region(int *coord, mixed bbox) {
  // This checks which side(s) of the box the point is on, setting
  // a different bit for each side; the idea is that if two points are
  // on the same side, they don't cross the box.
  int ret;

  if (coord[0] < bbox[0][0]) {
    ret += 1;
  } else if (coord[0] > bbox[1][0]) {
    ret += 2;
  }
  if (coord[1] < bbox[0][1]) {
    ret += 4;
  } else if (coord[1] > bbox[1][1]) {
    ret += 8;
  }
  return ret;
} /* query_region() */

/**
 * This method returns the room on the path that lies along the line
 * between coord_a and coord_b.  Note that if the line crosses more than
 * one room, the one closest to coord_a is returned.
 * @param coord_a the coordinates of the first point
 * @param coord_b the coordinates of the second poing
 * @return the room between the two points, or 0 if no such room exists
 */
object find_room_at_crossing(int *coord_a, int *coord_b) {
  int i, a, b, c, d, dist2, ret_dist2;
  float e, f, det;
  int *isect;
  object ret;
  mixed bbox, road_a, road_b;

  if (dbg_lvl > 0) {
    tell_creator(debugger, "%s::find_room_at_crossing()\n"
                 "  coord_a: %O\n"
                 "  coord_b: %O\n",
                 file_name(this_object()), coord_a, coord_b);
  }
  // First check that we're in the neighborhood
  if (query_region(coord_a, overall_bbox)
      & query_region(coord_b, overall_bbox)) {
    return 0;
  }
  for (i = 0; i < sizeof(iroom_info); i++) {
    // Check if the line crosses the bounding box
    bbox = iroom_info[i]->bbox;
    if (query_region(coord_a, iroom_info[i]->bbox)
        & query_region(coord_b, iroom_info[i]->bbox)) {
      //tell_creator("jeremy", "Boxes don't overlap (%d).\n", i);
      continue;
    }
    // Find intersection of lines.
    // I really wanted to avoid floats, but the sizes of some of the
    // coordinates made it a necessity when multiplying them together.
    road_a = allocate(3);
    road_b = allocate(3);
    if (iroom_info[i]->sgn_n > 0) {
      road_a[0] = bbox[0][0];
      road_b[0] = bbox[1][0];
    } else {
      road_a[0] = bbox[1][0];
      road_b[0] = bbox[0][0];
    }
    if (iroom_info[i]->sgn_e > 0) {
      road_a[1] = bbox[0][1];
      road_b[1] = bbox[1][1];
    } else {
      road_a[1] = bbox[1][1];
      road_b[1] = bbox[0][1];
    }
    a = coord_b[0] - coord_a[0];
    b = coord_a[1] - coord_b[1];
    c = road_b[0] - road_a[0];
    d = road_a[1] - road_b[1];
    e = to_float(a)*coord_a[1] + to_float(b)*coord_a[0];
    f = to_float(c)*road_a[1] + to_float(d)*road_a[0];
    det = a*d - b*c;
    // The compiler apparently doesn't recognize scientific notation
    if (fabs(det) < 0.0000000000001) {
      //tell_creator("jeremy", "Determinant is 0.\n");
      continue;
    }
    isect = allocate(3);
    isect[0] = to_int(floor((a*f - e*c)/det + 0.5));
    isect[1] = to_int(floor((e*d - b*f)/det + 0.5));
    if (!in_bbox(isect, bbox) || !in_bbox(isect, bbox(coord_a, coord_b))) {
      // They intersect, but not between the endpoints
      continue;
    }
    dist2 = (isect[0]-coord_a[0])*(isect[0]-coord_a[0]) +
      (isect[1]-coord_a[1])*(isect[1]-coord_a[1]);
    if (objectp(ret) && (dist2 >= ret_dist2)) {
      // This one is further away
      continue;
    }
    ret = find_room_at_coord(isect, i);
    if (dbg_lvl > 0) {
      tell_creator(debugger, "%s::find_room_at_crossing()\n"
                   "  find_room_at_coord() returned %O\n",
                   file_name(this_object()), ret);
    }
    ret_dist2 = dist2;
    // I don't think there's any way this should happen, but I'll check anyway
    if (!objectp(ret)) {
      error("Couldn't find a room where there should be one!\n");
    }
  }
  return ret;
} /* find_room_at_crossing() */

/**
 * @ignore
 */
void create() {
  string std_euid;

  std_euid = "/secure/master"->creator_file(file_name(this_object()));
  seteuid(std_euid);
  do_setup++;
  ::create();
  do_setup--;

  if ( !do_setup ) {
    this_object()->setup();
    this_object()->reset();
  }
  // Keeps us from being unloaded (one of the drawbacks of making the
  // handler a room; I wonder if that was a smart move...).
  set_keep_room_loaded(1);
} /* create() */

/**
 * @ignore
 */
void dest_me() {

  if (stringp(save_file)) {
    unguarded( (: save_object, save_file :) );
  }
  ::dest_me();
} /* dest_me() */

// These are testing convenience functions
/**
 * @ignore
 */
int goto_room_at_coord(int *coord) {
  return this_player()->move_with_look(find_room_at_coord(coord));
}

/**
 * @ignore
 */
int goto_room_at_index(int i, int j) {
  return this_player()->move_with_look(find_room_at_index(i, j));
}

/**
 * @ignore
 */
int goto_room_at_crossing(int *a, int *b) {
  return this_player()->move_with_look(find_room_at_crossing(a, b));
}

/**
 * @ignore
 * This function isn't quite for public use yet.
 */
void recalc_milestones(int idx, int callouts, object tp) {
  // This should initially be called with both parameters 0 (or unspecified);
  // It callouts itself until it gets through all the milestones.
  int *c;
  string ms;

  if (callouts > 10) {
    error(sprintf("Too many callouts in %s:recalc_iroom_info(%d, %d)\n",
          file_name(this_object()), idx, callouts));
    return;
  }
  if (!idx && !callouts && !tp) {
    // Reinitialize everything
    rm(save_file);
    this_object()->setup();
    idx = 0;
    tp = this_player();
  }
  ms = milestones[idx];
  if (!arrayp(c = ms->query_co_ord())) {
    // The call_out is necessary to give time for the coords to be set
    call_out("recalc_milestones", 0, idx, callouts+1, tp);
    tell_object(tp, sprintf("Waiting on %s (%d, %d)...\n",
                          ms, idx, callouts));
    return;
  }
  ms->setup_milestone(ms);
  tell_object(tp, sprintf("Loaded room %s at (%d, %d, %d)...\n",
                          ms, c[0], c[1], c[2]));
  idx++;
  if (idx < sizeof(milestones)) {
    // This could be a direct call, but hey, this won't be run that
    // often, and it makes the code more uniform.
    call_out("recalc_milestones", 0, idx, 0, tp);
    return;
  }
  tell_object(tp, "IRoom info successfully recalculated.\n");
} /* recalc_milestones() */

/**
 * @ignore
 */
void clear_map_handler() {
  // This is necessary if the rooms move to new coordinates, since
  // the map handler doesn't seem to update itself very often.
  // For right now, this has to be called manually.
  int i;
  string ms, directory, troom;

  for (i = 0; i < sizeof(milestones); i++) {
    ms = milestones[i];
    // These are needed for the map handler
    directory = implode( explode( ms, "/" )[ 0..<2 ], "/" );
    troom = explode( ms, "/" )[ <1 ];
    if ( troom[ <2.. ] == ".c" ) {
      troom = troom[ 0..<3];
    }
    MAP->del(directory, troom);
    printf("Deleting %s in %s from map handler.\n", directory, troom);
  }
} /* clear_map_handler() */

/**
 * @ignore
 */
int sanity_checks() {
  // This does a few sanity checks, like making sure all milestones
  // have coordinates and sizes, the interroom areas have bounding
  // boxes, etc.  It returns 1 on success.
  int i, success = 1;

  for (i = 0; i < sizeof(milestone_coords); i++) {
    if (sizeof(milestone_coords[i]) != 3) {
      printf("Milestone %d (%s) doesn't have proper coordinates: %O\n",
             i, milestones[i], milestone_coords[i]);
      success = 0;
    }
  }
  for (i = 0; i < sizeof(iroom_info); i++) {
    if (sizeof(iroom_info[i]->size) != 3) {
      printf("Interroom segment %d has improper size: %O\n",
             i, iroom_info[i]->size);
      success = 0;
    }
    if ((sizeof(iroom_info[i]->bbox) != 2) ||
        (sizeof(iroom_info[i]->bbox[0]) != 3) ||
        (sizeof(iroom_info[i]->bbox[1]) != 3)) {
      printf("Interroom segment %d has improper bounding box: %O\n",
             i, iroom_info[i]->bbox);
      success = 0;
    }
  }
  return success;
}
