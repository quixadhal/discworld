/*  -*- LPC -*-  */
/*
 * $Id: dup_licate.c,v 1.4 2001/02/02 20:13:02 pinkfish Exp $
 */
/* Makes an exact duplicate of an object */

#include <creator.h>

inherit "/cmds/base";

protected int do_duplicate(object *ov) {
   string pname, dummy;
   int i;
   object dup;
   mixed static_arg, dynamic_arg;

   for (i = 0; i < sizeof(ov); i++) {
      if (!ov[i]) {
         continue;
      }
      pname = file_name(ov[i]);
      if (sscanf(pname, "%s#%d", pname, dummy) != 2) { /* a room ? */
        // Can't duplicate a unique item.  Need some sort of message here.
        write("Can't duplicate " + ov[i]->short() + " (not a clone).\n");
        continue;
      } else {
         static_arg = ov[i]->query_static_auto_load();
         dynamic_arg = ov[i]->query_dynamic_auto_load();

         dup = clone_object(pname);
         if (dup && ov[i]) {
            ov[i] = dup;
            if (static_arg) {
              ov[i]->init_static_arg(copy(static_arg));
            }
            if (dynamic_arg) {
              ov[i]->init_dynamic_arg(copy(dynamic_arg));
            }
         }
         ov[i]->add_property("cloned by", this_player()->query_name());
      }

     if (!ov[i]) {
        printf("I seem to have lost your object.\n");
        return 1;
     }

     if (!ov[i]->move(this_player())) {
       write(ov[i]->short() + " duplicated and put in you.\n");
     } else if (!ov[i]->move(environment(this_player()))) {
       write(ov[i]->short() + " duplicated and put in here.\n");
     } else if (!ov[i]->move("/room/broken")) {
       write(ov[i]->short() + " duplicated and put in /room/broken.\n");
     } else {
       // I hope this will never happen...
       write("Couldn't find anyplace to put " + ov[i]->short() + "!\n");
       ov[i]->move("/room/rubbish");
     }
   }
   return 1;
}

mixed cmd(string str) {
  object *val;

  notify_fail("No such object.\n");
  val = WIZ_PRESENT->wiz_present(str, this_player());
  if(!sizeof(val)) {
    notify_fail("No matching objects\n");
    return 0;
  }
  return do_duplicate(val);
}
