/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: whois.c,v 1.5 2000/04/23 03:18:23 presto Exp $
 * $Log: whois.c,v $
 * Revision 1.5  2000/04/23 03:18:23  presto
 * Added an expand_nickname call so it works with nicknames
 *
 * Revision 1.4  1998/10/11 01:59:20  presto
 * Fixed so whois on yourself works again.  Tired of the complaints. :(
 *
 * Revision 1.3  1998/09/17 02:21:15  presto
 * changed cmd pattern from <indirect:any-living> to <indirect:player>
 * Removed check for interactive() on the matched object
 *
 * Revision 1.2  1998/04/14 02:30:40  pinkfish
 * Clean up a little.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
/* whois command, trial out by Turrican for a commands daemon. */

#include <command.h>

inherit "/cmds/base";

mixed cmd(object *obs) {
  write("That is " + (string)(DIR_PLAYER_CMDS +"/who")->
        who_line(this_player()->expand_nickname(obs[0]),
                 this_player()->query_creator(),
                 this_player()->query_cols()) +".\n" );
  return 1;
} /* cmd() */

mixed *query_patterns() {
  return ({ "<indirect:player>", (: cmd($1) :),
            this_player()->query_name(), (: cmd(({this_player()})) :) });
} /* query_patterns() */
