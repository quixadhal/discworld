/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: sa_y.c,v 1.11 2001/03/09 21:56:57 ceres Exp $
 * 
*/
/* command, trial out by Turrican for a commands daemon. */
#include <language.h>
#include <player.h>
#include <drinks.h>

inherit "cmds/base";
inherit "cmds/speech";

mixed cmd(string arg) {
  class message mess;
  mess = build_message(arg, 0, "say");
  return say_it(mess);
}
