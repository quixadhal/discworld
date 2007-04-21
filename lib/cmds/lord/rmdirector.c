/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: rmdirector.c,v 1.4 2003/03/21 02:23:55 ceres Exp $
 * 
*/
#include <parser.h>
#include <player_handler.h>

inherit "/cmds/base";


/*
 * This will remove a director.
 */
mixed cmd(string creator) {
  seteuid("Root");
  
  if ( !PLAYER_HANDLER->test_user( creator ) ||
       !"/secure/master"->query_trustee( previous_object( -1 ) ) )
    return notify_fail("You must be a trustee to remove a director.\n");

  if("/secure/master"->remove_director(creator)) {
    write(creator + " removed from directorship.\n");
    return 1;
  } else
    return notify_fail("Call to remove_director failed.\n");
} /* cmd() */

mixed *query_patterns() {
  return ({ "<string>", (: cmd($4[0]) :), });
} /* query_patterns() */
