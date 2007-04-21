/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: terms.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: terms.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* terms command, trial out by Turrican for a commands daemon. */
inherit "/cmds/base";

#include <peopler.h>
#include <creator.h>

mixed cmd(string str) {
   mixed *stuff;

   stuff = this_player()->query_property("term list");
   if (!stuff) {
      stuff = T_DEFAULT;
   }
   return PEOPLER->do_command(stuff, str);
} /* cmd() */

mixed *query_patterns() {
   return ({ "", (: cmd(0) :),
             "<word>", (: cmd($4[0]) :) });
} /* query_patterns() */
