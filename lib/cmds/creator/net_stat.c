/*  -*- LPC -*-  */
/*
 * $Locker: taffyd $
 * $Id: ne_tstat.c,v 1.1 1998/01/06 05:25:41 ceres Exp taffyd $
 * $Log: ne_tstat.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
#include <creator.h>
inherit "/cmds/base";

#include <peopler.h>
#include <creator.h>

mixed cmd(string str) {
   mixed *stuff;

   stuff = this_player()->query_property("netstat list");
   if (!stuff) {
      stuff = N_DEFAULT;
   }
   return PEOPLER->do_command(stuff, str);
} /* cmd() */

mixed *query_patterns() {
   return ({ "", (: cmd(0) :),
             "<word>", (: cmd($4[0]) :) });
} /* query_patterns() */
