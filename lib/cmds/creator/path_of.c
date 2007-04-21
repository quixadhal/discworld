/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: path_of.c,v 1.3 2003/02/25 17:19:40 trilogy Exp $
 * $Log: path_of.c,v $
 * Revision 1.3  2003/02/25 17:19:40  trilogy
 * Got rid of some tabs.
 *
 * Revision 1.2  2001/06/01 20:56:52  ceres
 * Fixed long eval stuff
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* pathof command, trial out by Turrican for a commands daemon. */

#include <creator.h>

mixed cmd(string str) {
  object *ov, ob;

  if(strsrch(str, "in everyone") != -1)
    return notify_fail("Please don't do that.  You've been asked not to "
      "and it causes lag.\n");

  notify_fail("Pathof what?\n");
  ov = WIZ_PRESENT->wiz_present(str, this_player());
  if (!sizeof(ov)) return 0;

  foreach (ob in ov) {
    if (!objectp(ob)) continue;
    write("Path of " + WIZ_PRESENT->desc_object(ob) + " in " +
        WIZ_PRESENT->desc_object(environment(ob)) + ":\n");
    write(file_name(ob)+ "\n");
  }
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
