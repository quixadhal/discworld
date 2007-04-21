/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: resize.c,v 1.2 1999/10/25 20:51:38 tape Exp $
 * $Log: resize.c,v $
 * Revision 1.2  1999/10/25 20:51:38  tape
 * moved stuff from help() to /doc/helpdir/resize
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
#include <telnet.h>

inherit "/cmds/base";

mixed cmd(string str) {
  printf("%c%c%c", IAC, DO, TELOPT_NAWS);
  write("Okay, resizing your terminal...\n");
  return 1;
}

