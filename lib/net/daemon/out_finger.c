/*  -*- LPC -*-  */
/*
 * $Id: out_finger.c,v 1.2 1999/07/15 19:34:10 turrican Exp $
 */
#include <network.h>

void do_finger(string str) {
  string name, host, realhost;

  if (sscanf(str, "%s@%s", name, host) == 2) {
    if ((realhost = INTERMUD_D->GetMudName(replace_string(host, ".", " ")))) {
      if (name == "")
        SERVICES_D->eventSendWhoRequest(realhost, this_player()->query_name());
      else
        SERVICES_D->eventSendFingerRequest(name, realhost);
      write("Intermud3 finger on its way.\n");
      return;
    }
    write(mud_name()+" does not know about the MUD "+host+".\n");
    return;
  }
/*
 * Should be a local finger...  no idea why they want to do that though.
 * shrug.
 */
} /* do_finger() */

void clean_up() {
  destruct(this_object());
}
