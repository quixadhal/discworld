/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: security.c,v 1.2 2002/04/19 20:44:22 pinkfish Exp $
 * $Log: security.c,v $
 * Revision 1.2  2002/04/19 20:44:22  pinkfish
 * *** empty log message ***
 *
 * Revision 1.1  1998/01/06 05:08:31  ceres
 * Initial revision
 * 
*/
#include <security.h>

varargs nomask mixed call_unguarded(string func, mixed a1,
                                           mixed a2, mixed a3, mixed a4) {

  return call_other(previous_object(), func, a1, a2, a3, a4);
} /* call_unguarded() */
