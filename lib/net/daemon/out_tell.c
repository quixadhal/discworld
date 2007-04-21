/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: out_tell.c,v 1.1 1998/01/06 05:17:08 ceres Exp $
 * $Log: out_tell.c,v $
 * Revision 1.1  1998/01/06 05:17:08  ceres
 * Initial revision
 * 
*/
#include <network.h>

void do_tell(string str) {
  string name, host, mess, newhost;

  if (sscanf(str, "%s@%s %s", name, host, mess) == 3) {
    if ((newhost = INTERMUD_D->GetMudName(replace_string(host, ".", " ")))) {
      SERVICES_D->eventSendTell(name, newhost, mess);
      write(sprintf("You tell %s@%s: %-=*s\n", capitalize(name), host, (int)this_player()->query_cols()
                     -strlen(name)-strlen(host)-13, mess));
      return;
    }
    write(mud_name()+" does not know about the MUD "+host+".\n");
    return;
  }
} /* do_tell() */

void clean_up() {
  destruct(this_object());
} /* clean_up() */
