/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: common_cold.c,v 1.1 1998/01/06 04:08:27 ceres Exp $
 * $Log: common_cold.c,v $
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
/* -*- LPC -*-
 * with a cure_rate of 6, use 20000 as "standard" severity
 * since a cold normally takes a week, 
 */
#include <disease.h>
#include <effect.h>
inherit SIMPLE_DISEASE;

void setup() {
   set_name( "Common Cold" );
   set_cure_rate( 6 );
   set_classification( "disease.infection.respiratory.cold" );
} /* setup() */

int disease_start( object player, int arg, int sid ) {
   tell_object( player, "You realise that your nose is running.\n" );
   player->submit_ee2( sid, "sap_strength", ({ 600, 600 }), EE_CONTINUOUS );
   return arg;
} /* disease_start() */

/** @ignore yes */
void disease_end( object player, int arg, int sid ) {
   if ( !player->query_property( "dead" ) )
      tell_object( player, "You realise that your nose is clear again.\n" );
} /* disease_end() */

void disease_action( object player, int arg ) {
   switch ( random( 3 ) ) {
      case 0 :
         player->new_parser( "sniff" );
         break;
      case 1 :
         player->new_parser( "cough" );
         break;
      default :
         player->new_parser( "sneeze" );
   }
} /* cold_symptoms() */

void sap_strength( object player ) {
   tell_object( player, "You feel your strength being sapped by the cold.\n" );
   player->adjust_tmp_str( -1 );
} /* sap_strength() */

