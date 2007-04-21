/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: flip.c,v 1.4 2002/01/21 12:32:12 taffyd Exp $
 * $Log: flip.c,v $
 * Revision 1.4  2002/01/21 12:32:12  taffyd
 * Changed to not allow you to flip currency which has a type of 'paper'.
 *
 * Revision 1.3  1999/10/25 23:05:41  pinkfish
 * Some bug fixes.
 *
 * Revision 1.2  1999/05/19 04:34:54  pinkfish
 * Fix some bugs and clean up the formating.
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
#include <money.h>

#define EFFECTS "/obj/handlers/mag_eff_handler"
inherit "/cmds/base";

mixed cmd( object *things ) {
   int *effect;
   object place;
   mixed *stuff, *details;

   if ( sizeof( things ) > 1 ) {
      add_failed_mess("You can only flip one thing at once.\n");
      return 0;
   }
   stuff = (mixed *)things[ 0 ]->query_money_array();
   if ( !sizeof( stuff ) ) {
      add_failed_mess( "You must have a coin to flip.\n" );
      return 0;
   }
   if ( ( sizeof( stuff ) > 2 ) || ( stuff[ 1 ] != 1 ) ) {
      add_failed_mess( "You can only flip one coin at a time.\n" );
      return 0;
   }

   details = (mixed *)MONEY_HAND->query_details_for( stuff[ 0 ] );
   
   if ( details[ MONEY_DETAILS_COMPOSITION ] == "paper" ) {
       add_failed_mess( "You cannot flip $I as you would a coin, because "
        "it is made of paper.\n", things );
       return -1;
   }

   write( "You flip "+ (string)things[ 0 ]->the_short() +".\n" );
   say( (string)this_player()->one_short() +" flips "+
         (string)things[ 0 ]->a_short() +".\n" );

   effect = (int *)EFFECTS->choose_effect(
         (int)EFFECTS->ambient_enchantment( things[ 0 ] ) );
   place = environment( this_player() );
   if ( random( 1000 ) < effect[ 0 ] ) {
      switch( effect[ 1 ] ) {
         case 0 :
            things[ 0 ]->move( place );
            tell_room( place, (string)things[ 0 ]->the_short() +
                  " lands on its edge.\n" );
            break;
         default :
            things[ 0 ]->move( "/room/rubbish" );
      }
      EFFECTS->do_effect( effect[ 1 ], (string)things[ 0 ]->the_short(),
            place );
      return 1;
   }
   things[ 0 ]->move( place );

   tell_room( place, (string)things[ 0 ]->the_short() +" falls "+
         (string)place->query_property( "here" ) +", "+
         details[ random( 2 ) ] +" up.\n" );
   return 1;
} /* cmd() */

mixed *query_patterns() {
   return ({ "<indirect:object:me>", (: cmd( $1 ) :) });
} /* query_patterns() */
