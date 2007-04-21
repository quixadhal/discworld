/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: qp_eople.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: qp_eople.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* qpeople command, trial out by Turrican for a commands daemon. */
inherit "/cmds/base";

#include <peopler.h>
#include <creator.h>

mixed cmd(string str) {
   mixed *stuff;

   stuff = this_player()->query_property("qpeople list");
   if (!stuff) {
      stuff = D_DEFAULT;
   }
   return PEOPLER->do_command(stuff, str);
} /* cmd() */

mixed *query_patterns() {
   return ({ "", (: cmd(0) :),
             "<word>", (: cmd($4[0]) :) });
} /* query_patterns() */
