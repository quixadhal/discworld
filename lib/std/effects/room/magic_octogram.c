/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: magic_octogram.c,v 1.1 1998/01/06 04:15:00 ceres Exp $
 * $Log: magic_octogram.c,v $
 * Revision 1.1  1998/01/06 04:15:00  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "magic.octogram".
 * <p>
 * Describe the arguments in here.
 * @classification magic.octogram
 * @see help::effects
 */
#include <effect.h>

/** @ignore yes */
string query_classification() { return "magic.octogram"; }

/** @ignore yes */
void beginning( object thing, int number, int id ) {
   tell_room( thing, "An octogram begins to glow "+
         (string)thing->query_property( "here" ) +".\n" );
   thing->submit_ee( 0, number, EE_REMOVE );
   thing->add_extra_look( this_object() );
/*
 * Something nasty until a better solution appears.
*/
   thing->add_item( "octogram", "The octogram glows in eldritch fashion, "
         "and that has nothing to do with being oblong." );
} /* beginning() */

/** @ignore yes */
int merge_effect( object thing, int old_arg, int new_arg, int id ) {
   tell_room( thing, "The octogram pulses for a moment.\n" );
   thing->submit_ee( 0, new_arg + (int)thing->expected_tt(), EE_REMOVE );
   return old_arg + new_arg;
} /* merge_effect() */

/** @ignore yes */
void end( object thing, int number, int id ) {
   if ( !sizeof( filter_array( previous_object( -1 ),
         (: $1->query_magic_spell() :) ) ) )
      tell_room( thing, "The octogram flickers and disappears.\n" );
   thing->remove_extra_look( this_object() );
} /* end() */

string extra_look( object thing ) {
   return "An octogram "+ (string)thing->query_property( "here" ) +
         " glows in eldritch fashion.\n";
} /* extra_look() */
