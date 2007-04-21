/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: godmother.c,v 1.6 2003/07/16 19:12:03 pinkfish Exp $
 * $Log: godmother.c,v $
 * Revision 1.6  2003/07/16 19:12:03  pinkfish
 * Make it use the players start pos before the nationality start pos.
 *
 * Revision 1.5  2003/07/16 19:08:21  pinkfish
 * Make it work to get out of limbo.
 *
 * Revision 1.4  2002/06/12 21:54:40  vashti
 * Modified the "no godmother" checking on rooms to allow custom messages.
 *
 * Revision 1.3  2002/02/23 22:16:20  ceres
 * Fixed runtime
 *
 * Revision 1.2  1999/11/01 20:49:13  tape
 * + added a checked for "no godmother" on the person doing the summoning as well,
 *   not just the environment
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
#include <panic.h>
#include <move_failures.h>
inherit "/cmds/base";

void create() {
  seteuid(getuid());
} /* create() */

int cmd( ) {
  mixed why_not;
  string destination;
  int ret;
  string str;

  if (!environment(this_player())) {
     // Very bad, just throw them to their start location.
     // First we try the last saved location.
      destination = this_player()->query_last_pos();

      str = catch(ret = this_player()->move_with_look(destination, 
                             "$N appears out of nowhere.",
                             "$N disappears with a pop."));
      if (str || ret != MOVE_OK) {
         destination = this_player()->query_start_pos();


         str = catch(ret = this_player()->move_with_look(destination,
                                "$N appears out of nowhere.",
                                "$N disappears with a pop."));
      }
      if (str || ret != MOVE_OK) {
         destination = this_player()->query_nationality()->query_default_start_location();


         str = catch(ret = this_player()->move_with_look(destination,
                                "$N appears out of nowhere.",
                                "$N disappears with a pop."));
      }
      if (str || ret != MOVE_OK) {
         add_failed_mess("Unable to move you out of limbo.  Try again "
                         "soon.\n");
         return 0;
      }
     add_succeeded_mess("You escape from limbo.\n");
     return 1;
  }

  if (environment() ||
      (why_not = environment(this_player())->query_property("no godmother"))) {
      /* Make sure they cannot do it in some places.  ie: start rooms */
          if ( why_not && stringp( why_not ) ) {
              add_failed_mess( why_not );
          } else {
              add_failed_mess("You cannot summon the godmother here.\n");
          }
          return 0;
   }

   why_not = this_player()->query_property( "no godmother" );
   
   if( why_not )
   {
      if( stringp( why_not ) && sizeof( why_not ) ) {
         add_failed_mess( why_not );
      } else {
         add_failed_mess( "You cannot summon a godmother at this moment.\n" );
      }
      
      return 0;
   }
   
   if (PANIC_HANDLER->do_startup(previous_object())) {
      this_player()->add_succeeded_mess(this_object(),
                      ({ "",
                         "$N stares into space and looks pensive.\n" }),
                      ({ }) );
   } else {
      /* They are busy */
      return 0;
   }
   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "help", (: cmd() :) });
} /* query_patterns() */
