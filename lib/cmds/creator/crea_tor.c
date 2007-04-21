/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: crea_tor.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: crea_tor.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* creator command, trial out by Turrican for a commands daemon. */

#include <creator.h>

mixed cmd(string str) {
  object *ov, ob;
 
  notify_fail("Creator of what?\n");
  ov = WIZ_PRESENT->wiz_present(str,this_player());
  if (!sizeof(ov)) return 0;

  foreach (ob in ov) {
    write("Creator of " + WIZ_PRESENT->desc_object(ob) + ": " +
        master()->creator_file (file_name(ob)) + ", uid: " +
        getuid(ob) + ", euid: "+geteuid(ob)+"\n");
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
