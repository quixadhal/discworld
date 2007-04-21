/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: rms_enior.c,v 1.2 2003/03/21 02:00:44 ceres Exp $
 * $Log: rms_enior.c,v $
 * Revision 1.2  2003/03/21 02:00:44  ceres
 * Modified to use player handler
 *
 * Revision 1.1  1998/01/06 05:29:21  ceres
 * Initial revision
 * 
*/
#include <parser.h>
#include <player_handler.h>

inherit "/cmds/base";


/*
 * This will remove a senior creator.
 */
mixed cmd(string creator) {
  seteuid("Root");
  
  if ( !PLAYER_HANDLER->test_user( creator ) ||
       !"/secure/master"->query_lord( previous_object( -1 ) ) )
    return notify_fail("You must be a lord to remove a senior creator.\n");

  if("/secure/master"->remove_senior(creator)) {
    write(creator + " removed from senior.\n");
    return 1;
  } else
    return notify_fail("Call to remove_senior failed.\n");
} /* cmd() */

mixed *query_patterns() {
  return ({ "<string>", (: cmd($4[0]) :), });
} /* query_patterns() */
