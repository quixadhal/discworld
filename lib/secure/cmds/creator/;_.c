/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: ;_.c,v 1.3 2000/05/11 23:41:04 pinkfish Exp $
 * $Log: ;_.c,v $
 * Revision 1.3  2000/05/11 23:41:04  pinkfish
 * Fix up the return values.
 *
 * Revision 1.2  2000/05/11 23:40:17  pinkfish
 * Change the return*
 *
 * Revision 1.1  1998/01/06 05:10:49  ceres
 * Initial revision
 * 
*/
/* ; command, trial out by Turrican for a commands daemon. */

#include <parser.h>

inherit "/cmds/base";

mixed cmd(string str) {
  mixed res;

  PARSER->init_expr(str);
  res = PARSER->expr();
  printf("Returns: %O\n", ((pointerp(res) && sizeof(res) == 1)?res[0]:res));
  PARSER->finish_expr();
  return 1;
}
