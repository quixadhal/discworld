/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ob_ject.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: ob_ject.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* object command, trial out by Turrican for a commands daemon. */
/* obvar stuff -> Ember 12/4/93 */
/* obvar changed slighty by pinkfish... 17/4/93 (New call syntax added) */

#include <creator.h>

mixed cmd(string str) {
  int i, j, cols;
  string obvarname, start, end;
  mixed *names, *objects;

  if (str) {
    if (sscanf(str, "%s=%s", obvarname, str) != 2)
      return notify_fail("USAGE: object ObVarName=ObjectSpec\n");
    notify_fail("Can't find object.\n");
    objects = WIZ_PRESENT->wiz_present(str, this_player());
    if (!sizeof(objects))
      return 0;
    if (sizeof(objects) > 1) 
      write("Only using first object of several matched.\n");
    this_player()->set_obvar(obvarname, objects[0]);
    write("Var "+obvarname+" now attached to "+
        WIZ_PRESENT->desc_object(objects[0])+".\n");
    return 1;
  }
  objects = m_values((mapping)this_player()->get_obvars());
  names = m_indices((mapping)this_player()->get_obvars());
  j = sizeof(objects);
  cols = this_player()->query_cols();
  for (i=0; i<sizeof(objects); i++) {
    if (!objects[i]) {
      j--;
      this_player()->del_obvar(names[i]);
      break;
    }
    if (objectp(names[i]))
      start = WIZ_PRESENT->desc_object(names[i]);
    else
      start = sprintf("%O", names[i]);
    if (objectp(objects[i]))
      end = WIZ_PRESENT->desc_object(objects[i]);
    else {
/* Need to do something cunning... */
      end = sprintf("%O", objects[i]);
      end = replace(end, "\n", "");
    }
    printf("%s; %*-=s\n", start, cols-strlen(start)-2, end);
  }

  if (!j) write("No obvars set.\n");
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
