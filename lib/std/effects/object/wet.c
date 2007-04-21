/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: wet.c,v 1.4 2002/01/06 04:57:44 presto Exp $
 * $Log: wet.c,v $
 * Revision 1.4  2002/01/06 04:57:44  presto
 * Add message for case where wetness is less than the weight
 *
 * Revision 1.3  2001/08/15 04:09:31  presto
 * added wet_string function to help out with the 'wet' command
 *
 * Revision 1.2  1998/01/11 17:03:41  pinkfish
 * fixed_up_the_extra_look_stuff,_tomake_them_ignored.
 *
 * Revision 1.1  1998/01/06 04:19:27  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "object.wet".
 * <p>
 * Describe the arguments in here.
 * @classification object.wet
 * @see help::effects
 */
// The target object will now receive the "wet" adjective
// while it is wet - Fiona

#include <effect.h>

/** @ignore yes */
string query_classification() { return "object.wet"; }

/** @ignore yes */
void beginning( object thing, int amount, int id ) {
   thing->submit_ee( "dry_off", ({ 30, 90 }), EE_CONTINUOUS );
   thing->add_extra_look( this_object() );
   // Now we can call a towel "a wet towel" or "a wet frog green towel" etc.
   // It will not actually change its short but spells will now
   // recognize the wetness effect.
   thing->add_adjective("wet");
} /* beginning() */

/** @ignore yes */
int merge_effect( object thing, int old_amount, int new_amount, int id ) {
   return old_amount + new_amount;
} /* merge_effect() */

/** @ignore yes */
void end( object thing, int amount, int id ) {
   thing->remove_extra_look( this_object() );
   // This thing is no longer wet. Remove the adjective
   thing->remove_adjective("wet");
} /* end() */

void restart( object thing, int amount, int id ) {
   thing->add_extra_look( this_object() );
} /* restart() */

void dry_off( object thing, int amount, int id ) {
   amount -= 100 + amount / 10;
   if ( amount > 0 )
      thing->set_arg_of( (int)thing->sid_to_enum( id ), amount );
   else
      thing->submit_ee( 0, 0, EE_REMOVE );
} /* dry_off() */

/** @ignore yes */
string wet_string( object thing ) {
   int *enums;
   string wetness;

   enums = (int *)thing->effects_matching( "object.wet" );
   if ( !sizeof( enums ) )
      return "";
   switch ( (int)thing->arg_of( enums[ 0 ] ) / ( 1 +
         (int)thing->query_weight() ) ) {
      case 0 :
         return "just barely damp";
      case 1 .. 5 :
         wetness = "slightly wet";
         break;
      case 6 .. 10 :
         wetness = "wet";
         break;
      case 11 .. 30 :
         wetness = "very wet";
         break;
      case 31 .. 80 :
         wetness = "sopping wet";
         break;
      default :
          wetness = "absolutely soaking wet";
   }
   return wetness;
} /* extra_look() */

/** ignore yes */
string extra_look(object thing)  {
   return "It is " + wet_string(thing) + ".\n";
}

