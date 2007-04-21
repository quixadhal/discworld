/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mks_enior.c,v 1.2 2003/03/21 01:59:43 ceres Exp $
 * $Log: mks_enior.c,v $
 * Revision 1.2  2003/03/21 01:59:43  ceres
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
 * This will make a creator a senior.
 */
mixed cmd(string creator) {
  seteuid("Root");
  
  if ( !PLAYER_HANDLER->test_user( creator ) ||
       !"/secure/master"->query_lord( previous_object( -1 ) ) )
    return notify_fail("You must be a lord to create a senior creator.\n");

  if("/secure/master"->add_senior(creator)) {
    write(creator + " promoted to senior.\n");
    return 1;
  } else
    return notify_fail("Call to add_senior failed.\n");
} /* cmd() */

mixed *query_patterns() {
  return ({ "<string>", (: cmd($4[0]) :), });
} /* query_patterns() */
