/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: dir_s.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: dir_s.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* dirs command, trial out by Turrican for a commands daemon. */
inherit "/cmds/base";

#include <peopler.h>
#include <creator.h>

mixed cmd(string str) {
   mixed *stuff;

   stuff = this_player()->query_property("dir list");
   if (!stuff) {
      stuff = D_DEFAULT;
   }
   return PEOPLER->do_command(stuff, str);
} /* cmd() */

mixed *query_patterns() {
   return ({ "", (: cmd(0) :),
             "<word>", (: cmd($4[0]) :) });
} /* query_patterns() */
