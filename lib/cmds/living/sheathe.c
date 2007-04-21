/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: sheathe.c,v 1.7 2002/01/28 20:28:12 tannah Exp $
 *
 *
 */

#include <move_failures.h>

inherit "/cmds/base";

int cmd( object * weapons, object * scabbards ) {
   object weapon;
   object scabbard;

   if( sizeof(weapons) > 1 ) {
      add_failed_mess( "You can only sheathe one thing at a time.\n" );
      return 0;
   }
   weapon = weapons[0];

   if ( ( scabbard = environment( weapon ) )->query_scabbard() &&
        scabbard->test_type_ok( weapon ) ) {
      add_failed_mess( weapon->the_short() + " is already sheathed.\n" );
      return 0;
   }

   if ( environment( weapon ) != this_player() ) {
      add_failed_mess( weapon->the_short() + " is not in your inventory.\n" );
      return 0;
   }

   if ( scabbards ) {
      if ( sizeof(scabbards) > 1 ) {
         add_failed_mess( "For shame! Trying to sheathe your weapon "
           "in more than one object at a time?\n" );
         return -1;
      }
      scabbard = scabbards[0];
      if ( !scabbard->query_scabbard() ) {
         add_failed_mess( scabbard->the_short() + " is not even a "
           "scabbard!\n" );
         return -1;
      }
      if( !scabbard->test_type_ok( weapon ) ) {
        return add_failed_mess( "$C$"+scabbard->the_short() + " is not a "
                                "suitable scabbard for " + weapon->a_short()
                                + ".\n" );
      }
   } else {
      scabbards = filter( all_inventory( this_player() ),
                          (: $1->query_scabbard() &&
                             $1->test_type_ok( $(weapon) ) :) );
      if ( !sizeof( scabbards ) ) {
         return add_failed_mess( "Can't find any suitable scabbards in "
           "your inventory.\n" );
      }

      scabbards = filter( scabbards, (: $1->test_add( $(weapon), 0, 1 ) :) );
      if ( !sizeof(scabbards) ) {
         return add_failed_mess( "Can't find any empty scabbards in your "
           "inventory.\n" );
      }

      scabbard = scabbards[0];
   }

   if ( weapon->query_wielded() &&
       !sizeof( this_player()->set_unhold(weapon) ) )
   {
      return add_failed_mess( "You cannot let go of " + weapon->the_short()
        + ".\n" );
   }

   if ( weapon->move(scabbard) != MOVE_OK ) {
      this_player()->add_failed_mess( scabbard, "You cannot $V $I in $D.\n",
        ({ weapon }) );
      return 0;
   }

   this_player()->add_succeeded_mess( scabbard, "$N $V $I in $D.\n",
     ({ weapon }) );
   return 1;
}

mixed * query_patterns() {
   return ({
      "<indirect:object:me'weapon'>",
        (: cmd( $1, 0 ) :),
      "<indirect:object:me'weapon'> in <indirect:object:me'scabbard'>",
        (: cmd( $1[0], $1[1]) :)
   });
}
