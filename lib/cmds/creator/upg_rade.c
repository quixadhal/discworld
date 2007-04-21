/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: upg_rade.c,v 1.2 2003/03/21 19:53:29 ceres Exp $
 * $Log: upg_rade.c,v $
 * Revision 1.2  2003/03/21 19:53:29  ceres
 * Modified for new login object
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* upgrade command, trial out by Turrican for a commands daemon. */

mixed cmd(string str) {
  seteuid(geteuid(this_player()));
  (clone_object("/secure/login"))->do_upgrade(this_player());
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
