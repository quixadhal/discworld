/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: iloc_ate.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: iloc_ate.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* locate command, trial out by Turrican for a commands daemon. */

#include <network.h>

mixed cmd(string who) {
  if (!who) {
    notify_fail("Syntax: locate <name>\n");
    return 0;
  }
  SERVICES_D->eventSendLocateRequest(who);
  printf("Locating %s. Don't explode !\n", who);
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
