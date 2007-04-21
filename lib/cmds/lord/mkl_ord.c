/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: mkl_ord.c,v 1.1 2003/03/21 02:01:13 ceres Exp $
 * $Log: mkl_ord.c,v $
 * Revision 1.1  2003/03/21 02:01:13  ceres
 * Initial revision
 *
 * Revision 1.1  1998/01/06 05:29:21  ceres
 * Initial revision
 * 
*/
#include <parser.h>
#include <player_handler.h>

inherit "/cmds/base";


/*
 * This will add a diector.
 */
mixed cmd(string creator) {
  seteuid("Root");
  
  if ( !PLAYER_HANDLER->test_user( creator ) ||
       !"/secure/master"->high_programmer( previous_object( -1 ) ) )
    return notify_fail("You must be a highlord to add a lord.\n");

  if("/secure/master"->add_director(creator)) {
    write(creator + " promoted to director.\n");
    return 1;
  } else
    return notify_fail("Call to add_director failed.\n");
} /* cmd() */

mixed *query_patterns() {
  return ({ "<string>", (: cmd($4[0]) :), });
} /* query_patterns() */
