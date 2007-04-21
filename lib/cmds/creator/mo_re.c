/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mo_re.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: mo_re.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* more command, trial out by Turrican for a commands daemon. */

#include <creator.h>

mixed cmd(string str) {
  object *things;

  seteuid(geteuid(this_player()));

  if(!str) {
    notify_fail("More which file(s)/object?\n");
    return 0;
  }
  if(sizeof(things = WIZ_PRESENT->wiz_present(str, this_player()))) {
    str = file_name(things[0]);
    sscanf(str, "%s#%*d", str);
    if (file_size(str) <= 0)
      str += ".c";
  }
  return this_player()->more_file(str);
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
