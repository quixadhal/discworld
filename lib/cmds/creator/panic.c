/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: panic.c,v 1.2 2002/05/12 09:02:01 taffyd Exp $
 * $Log: panic.c,v $
 * Revision 1.2  2002/05/12 09:02:01  taffyd
 * Disabled command, can be re-enabled later.
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
#include <panic.h>

inherit "/cmds/base";

int cmd( ) {
    /* We don't need this, let's just return 0 now. */
    return 0;
/*
   if (!PLAYER_TESTER(previous_object())) {
    this_player->add_failed_mess(this_object(), "You need to be a player tester "
                                                "to use this command.\n", ({ }));
    return 0;
  }
 */
  PANIC_HANDLER->do_startup(previous_object());
  this_player()->add_succeeded_mess(this_object(),
                  "$N panic$s hopefully.", ({ }) );
  return 1;
} /* cmd() */

mixed *query_pattern() {
  return ({ "", (: cmd() :) });
} /* query_pattern() */
