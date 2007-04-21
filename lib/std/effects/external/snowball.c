/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: snowball.c,v 1.1 1998/01/06 04:10:22 ceres Exp $
 * $Log: snowball.c,v $
 * Revision 1.1  1998/01/06 04:10:22  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "mudlib.snowball".
 * <p>
 * Describe the arguments in here.
 * @classification mudlib.snowball
 * @see help::effects
 */
#include <effect.h>

/** @ignore yes */
string query_classification() { return "mudlib.snowball"; }

/** @ignore yes */
void beginning( object player, int amount, int id ) {
   player->add_extra_look( this_object() );
   player->submit_ee( "decrease_amount", 15, EE_CONTINUOUS );
} /* beginning() */

void restart( object player, int amount, int id ) {
   player->add_extra_look( this_object() );
} /* restart() */

/** @ignore yes */
int merge_effect( object player, int old_amount, int new_amount, int id ) {
   return old_amount + new_amount;
} /* merge_effect() */

/** @ignore yes */
void end( object player, int amount, int id ) {
   player->remove_extra_look( this_object() );
} /* end() */

void decrease_amount( object player, int amount, int id ) {
   amount -= 3 + random( random(amount) );
   if ( amount < 0 ) {
      amount = 0;
      player->submit_ee( 0, 1, EE_REMOVE );
   }
   player->set_arg_of( (int)player->sid_to_enum( id ), amount );
   player->add_effect("/std/effects/other/wetness", roll_MdN( 10, 10 ) );
} /* decrease_amount() */

string extra_look( object player ) {
   int *enums;
   string desc;
   enums = (int *)player->effects_matching( "mudlib.snowball" );
   if ( !sizeof( enums ) )
      return "";
   switch ( (int)player->arg_of( enums[ 0 ] ) ) {
      case 0 .. 99 :
         desc = " has a little bit of snow";
         break;
      case 100 .. 199 :
         desc = " has a bit of snow";
         break;
      case 200 .. 299 :
         desc = " has a few small lumps of snow";
         break;
      case 300 .. 399 :
         desc = " has a few lumps of snow";
         break;
      case 400 .. 499 :
         desc = " has a number of lumps of snow";
         break;
      case 500 .. 599 :
         desc = " has quite a lot of snow";
         break;
      case 600 .. 699 :
         desc = " has a lot of snow";
         break;
      default :
/* They should be blind with this much... */
         desc = " has a thick layer of snow";
   }
   return capitalize( (string)player->query_pronoun() ) + desc +" on "+
         (string)player->query_possessive() +" face.\n";
} /* extra_look() */
