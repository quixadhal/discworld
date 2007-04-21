/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: set.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: set.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* set command, trial out by Turrican for a commands daemon. */

#include <creator.h>

mixed cmd(string str) {
  return (int)PEOPLER->set_var(str);
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
