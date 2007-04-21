/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: malloc.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: malloc.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* malloc command, trial out by Turrican for a commands daemon. */

mixed cmd(string str) {
  printf("%s", malloc_status());
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
