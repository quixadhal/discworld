/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: sne_tstat.c,v 1.2 1999/10/28 02:35:54 ceres Exp $
 * $Log: sne_tstat.c,v $
 * Revision 1.2  1999/10/28 02:35:54  ceres
 * can't remember
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* goto command, trial out by Turrican for a commands daemon. */
inherit "/cmds/base";

#include <creator.h>
#include <peopler.h>

mixed cmd(string str) {
   mixed *bing;

   bing = (mixed *)this_player()->query_property("netstat list");
   if (!bing) {
     bing = SN_DEFAULT;
   }
   (int)PEOPLER->do_command( bing, str,
                         (: strcmp(query_ip_number($1), query_ip_number($2)) :) );
   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "", (: cmd(0) :),
             "<word>", (: cmd($4[0]) :) });
} /* query_patterns() */
