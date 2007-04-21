/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: hypothermia.c,v 1.2 1999/04/02 16:47:24 taffyd Exp $
 * $Log: hypothermia.c,v $
 * Revision 1.2  1999/04/02 16:47:24  taffyd
 * Made it so you could have your own custom level of freezing. (defaults to 100).
 *
 * Revision 1.1  1998/01/06 04:08:27  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "disease.hypothermia".
 * <p>
 * Describe the arguments in here.
 * @classification disease.hypothermia
 * @see help::effects
 */
#include <effect.h>

/** @ignore yes */
string query_classification() { return "disease.hypothermia"; }

/** @ignore yes */
int beginning( object player, int index, int id ) {

    if ( !index ) 
        index = 100;

   tell_object( player, "You feel a bit chilly.\n" );
   player->submit_ee( "change_index", ({ 60, 60 }), EE_CONTINUOUS );
   player->submit_ee( "stat_adjusts", ({ 90, 90 }), EE_CONTINUOUS );

   return index;

} /* beginning() */

/** @ignore yes */
int merge_effect( object player, int index ) { return index; }

/** @ignore yes */
void end( object player ) {

   tell_object( player, "You feel a lot warmer now.\n" );

} /* end() */

void change_index( object player, int index, int id ) {
  int *enums, pwet;
  
   if ( !environment( player ) )
      return;
   if ( !environment( player )->query_property( "freezer" ) ) {
      player->submit_ee( 0, 0, EE_REMOVE );
      return;
   }
   index -= 7 - (int)player->query_con() / 7;
   if ( index < 0 ) {
      player->dest_hide_shadow();
      tell_object( player, "The cold is just too much for you.\n" );
      tell_room( environment( player ), "Frost riming "+
            (string)player->query_possessive() +" skin in a thick crust, "+
            (string)player->the_short() +" expires.\n", player );
      player->do_death();
      return;
   }
   switch( random( 4 ) ) {
      case 0 :
         player->dest_hide_shadow();
         tell_object( player, "You shiver.\n" );
         tell_room( environment( player ),
               capitalize( (string)player->short() ) +" shivers.\n", player );
         break;
      case 1 :
         player->dest_hide_shadow();
         tell_object( player, "You rub your arms briskly in an attempt to "+
               "keep warm.\n" );
         tell_room( environment( player ),
               capitalize( (string)player->short() ) +" rubs "+
               (string)player->query_possessive() +" arms briskly.\n",
               player );
         break;
      case 2 :
        enums = (int *)player->effects_matching("body.wetness"); 
         if ( sizeof( enums ) )
           pwet = (int)player->arg_of( enums[ 0 ] );
         
         if ( (int)pwet > 25 ) {
            player->dest_hide_shadow();
            tell_object( player, "You dislodge a lump of ice, which falls "+
                  "and shatters on the floor.\n" );
            tell_room( environment( player ),
                  capitalize( (string)player->short() ) +" dislodges a lump "+
                  "of ices, which falls and shatters on the floor.\n",
                  player );
            player->add_effect("/std/effects/wetness", -25 );
         }
         break;
      default :
         tell_object( player, "You're getting really cold in here.\n" );
   }
   player->set_arg_of( (int)player->sid_to_enum( id ), index );

} /* change_index() */

void stat_adjusts( object player ) {

   switch ( random( 3 ) ) {
      case 0 :
         tell_object( player, "You feel your strength being drained by the "+
               "below freezing temperatures.\n" );
         player->adjust_tmp_str( -1 );
         return;
      case 1 :
         tell_object( player, "You really don't feel at all well stuck in "+
               "here.\n" );
         player->adjust_tmp_con( -1 );
         return;
      default :
         if ( (string)player->query_race() == "troll" ) {
            tell_object( player, "You feel a tingling between your ears and "+
                  "new and strangely clever thoughts race along the "+
                  "superconducting neural pathways of your brain.\n" );
            player->adjust_int( 1 );
         }
   }

} /* stat_adjusts() */
