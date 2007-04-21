/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: tra_ns.c,v 1.2 2003/04/11 00:46:24 wodan Exp $
 * $Log: tra_ns.c,v $
 * Revision 1.2  2003/04/11 00:46:24  wodan
 * Forcibly released due to inactivity
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* trans command, trial out by Turrican for a commands daemon. */

#include <creator.h>

mixed cmd(string str) {
  object *obs, ob;

  if (!str || !(sizeof(obs = WIZ_PRESENT->wiz_present(str, this_player())))) {
    write("Transport who ?\n");
    return 1;
  }
  foreach (ob in obs) {
    if (environment(ob) == environment(this_player())) {
      write(WIZ_PRESENT->desc_object(ob)+" is already here.\n");
      continue;
    }
    tell_object( ob, "You are magically transferred somewhere.\n" );
      ob->move_with_look( environment( this_player() ),
            (string)ob->query_mmsgin(), (string)ob->query_mmsgout() );
    log_file("/d/admin/log/trans", "%s transed %s to %s\n", 
             this_player()->query_name(), str, 
             base_name(environment(this_player())));
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
