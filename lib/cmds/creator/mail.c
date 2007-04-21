/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mail.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: mail.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* mail command, trial out by Turrican for a commands daemon. */

#include <mail.h>

mixed cmd(string str) {
  return (int)MAIL_TRACK->mail(str);
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
