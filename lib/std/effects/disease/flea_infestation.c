/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: flea_infestation.c,v 1.6 2001/06/01 21:17:44 ceres Exp $
 * $Log: flea_infestation.c,v $
 * Revision 1.6  2001/06/01 21:17:44  ceres
 * Stopped massive outbreaks
 *
 * Revision 1.5  2000/11/18 07:25:08  ceres
 * Made fleas disappear more quickly.
 *
 * Revision 1.4  2000/01/07 22:17:54  ceres
 * Added a chance that an NPC will stop having fleas.
 *
 * Revision 1.3  1998/04/21 02:15:24  terano
 * Changed % infection per EE from 15 to 2..
 *
 * Revision 1.2  1998/04/20 18:20:03  hobbes
 * changed set_infection_chance() from 1 to 15
 *
 * Revision 1.1  1998/01/06 04:08:27  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "misc.curse.smurf".
 * <p>
 * Describe the arguments in here.
 * @classification misc.curse.smurf
 * @see help::effects
 */
/* Flea infestation, use 10000 as standard severity  -*- LPC -*-
 */
#include <effect.h>
#include <disease.h>
inherit SIMPLE_DISEASE;

void setup() {
   set_name( "Fleas" );
   set_classification( "disease.parasite.external.flea" );
  set_infection_chance( 2 );
   /* Something can catch fleas if it's alive, and */
   set_infection_check( (: living( $1 ) &&
         /* is an NPC that is not unique or */
               ( ( !userp( $1 ) && !$1->query_property( "unique" ) ) ||
         /* is a mature player, and is not dead. */
               $1->query_mature() ) && !$1->query_property( "dead" ) :) );
} /* setup() */

int disease_start( object player, int arg, int ) {
   string race_ob;

   race_ob = (string)player->query_race_ob();
   /* Trolls can't have fleas. */
   if ( ( (string)race_ob->query_name() == "troll" ) ||
         player->query_property( "no fleas" ) ) {
      player->submit_ee( 0, 0, EE_REMOVE );
      return 0;
   }
   return arg;
} /* disease_start() */

void disease_action( object player, int, int ) {
   switch( random( 14 ) ) {
      case 0 .. 3 :
         player->remove_hide_invis( "hiding" );
         tell_object( player, "Something itches.  You scratch yourself.\n" );
         tell_room( environment( player ), (string)player->one_short() +
               " scratches "+ (string)player->query_objective() +"self.\n",
               player );
         break;
      case 4 .. 7 :
         player->remove_hide_invis( "hiding" );
         tell_object( player, "Ouch!  Something bit you!\n" );
         tell_room( environment( player ), (string)player->one_short() +
               " starts as if bitten.\n", player );
         /* Should this use ac? */
         player->adjust_hp( -10 - random( 10 ) );
         break;
      case 8 .. 11 :
         tell_object( player, "You notice a small red welt on your skin.\n" );
         break;
      default:
   }

   if(!random(10) && !userp(player))
     player->submit_ee(0, 0, EE_REMOVE);
   
} /*  disease_action( */

int test_remove( object player, int arg, int enum, int bonus ) {
   log_file( "OLD_CURE", file_name( previous_object() ) +" "+
            file_name( this_object() ) +"\n" );
   if ( arg + bonus < 30 ) {
      player->set_arg_of( enum, arg + bonus );
      return 0;
   }
   return 1;
} /* test_remove() */
