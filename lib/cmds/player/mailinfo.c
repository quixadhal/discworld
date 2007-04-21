/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mailinfo.c,v 1.3 2003/03/25 15:58:04 trilogy Exp $
 * $Log: mailinfo.c,v $
 * Revision 1.3  2003/03/25 15:58:04  trilogy
 * Was using /secure/login
 *
 * Revision 1.2  2003/01/12 07:15:48  presto
 * Ignore case
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
/* mailinfo command, trial out by Turrican for a commands daemon. */

#include <mail.h>
#include <player_handler.h>

mixed cmd(string str) {
  if (!str || str=="")
    str = this_player()->query_name();
  else
    str = this_player()->expand_nickname(str);
  if (!PLAYER_HANDLER->test_user(lower_case(str)))
  {
    write("Sorry, but I don't know any "+capitalize(str)+".\n");
    return 1;
  }
  write("Player "+capitalize(str)+": "+(string)MAILER->finger_mail(str));
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
