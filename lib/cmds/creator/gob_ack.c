/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: gob_ack.c,v 1.5 1999/07/03 19:56:33 sin Exp $
 * $Log: gob_ack.c,v $
 * Revision 1.5  1999/07/03 19:56:33  sin
 * Fixed another bug in my terrain support.
 *
 * Revision 1.4  1999/07/02 18:42:38  sin
 * Fixed an error.
 *
 * Revision 1.3  1999/07/02 18:37:28  sin
 * Added some support for terrains.
 *
 * Revision 1.2  1999/06/19 03:05:54  ceres
 * Slight tweak
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* goto command, trial out by Turrican for a commands daemon. */
#include <terrain.h>

mixed cmd(string str) {
  mixed dest;
  string tname;

  dest = this_player()->query_last_location();
  if(!dest)
    return notify_fail("No stored location.\n");
  if (stringp(dest)) {
    /* Player was in a normal room */
    dest = load_object(dest);
  } else if (arrayp(dest) && stringp(dest[0]) && arrayp(dest[1])) {
    /* Player was in a terrain */
    dest = TERRAIN_MAP->find_location(dest[0], dest[1]);
  } else {
    return notify_fail("I don't understand your stored location.\n");
  }
  tname = environment(this_player())->query_property("terrain name");
  if (tname) {
    /* Player is in a terrain.  Store terrain name and coords. */
    this_player()->set_last_location(
      ({ tname, environment(this_player())->query_co_ord() })
      );
  } else {
    /* Player was in a normal room.  Store file name. */
    this_player()->set_last_location(
      base_name(environment(this_player()))
      );
  }
  this_player()->move_with_look( dest,
                                 this_player()->query_mmsgin(),
                                 this_player()->query_mmsgout() );
  return 1;
}

void dest_me() {
  destruct(this_object());
}

void clean_up() {
  dest_me();
}

void reset() {
  dest_me();
}
