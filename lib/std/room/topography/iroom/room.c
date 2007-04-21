/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: room.c,v 1.1 2000/05/01 03:13:34 jeremy Exp $
 *
 *
 */

/*
** room.c - iroom implementation of /std/room.
*/

inherit "/std/room";

// My apologies for including .c files.  If anyone else has a better way,
// I'm open to suggestions (the main problem is I need to mask
// query_dest_other() in the base room).

#include "/std/room/inherit/topography/iroom.c"

void create() {

  do_setup++;
  ::create();
  do_setup--;

  if ( !do_setup ) {
    this_object()->setup();
    this_object()->reset();
  }

  add_exit(motion_verb, "interroom", "hidden");
} /* create() */
