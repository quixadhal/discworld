/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mkdirect_or.c,v 1.2 2003/03/21 02:02:33 ceres Exp $
 * 
*/
#include <parser.h>
#include <player_handler.h>

inherit "/cmds/base";


/*
 * This will add a director.
 */
mixed cmd(string creator) {
  seteuid("Root");
  
  if ( !PLAYER_HANDLER->test_user( creator ) ||
       !"/secure/master"->query_trustee( previous_object( -1 ) ) )
    return notify_fail("You must be a trustee to add a director.\n");

  if("/secure/master"->add_director(creator)) {
    write(creator + " promoted to directorship.\n");
    return 1;
  } else
    return notify_fail("Call to add_director failed.\n");
} /* cmd() */

mixed *query_patterns() {
  return ({ "<string>", (: cmd($4[0]) :), });
} /* query_patterns() */
