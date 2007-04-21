/*  -*- LPC -*-  */
/*
 * $Locker: pinkfish $
 * $Id: bob.c,v 1.1 2000/05/10 07:55:11 pinkfish Exp pinkfish $
 * $Log: bob.c,v $
 * Revision 1.1  2000/05/10 07:55:11  pinkfish
 * Initial revision
 *
 * Revision 1.1  1998/01/06 04:11:58  ceres
 * Initial revision
 * 
*/
/*
 * $Locker: pinkfish $
 * $Id: bob.c,v 1.1 2000/05/10 07:55:11 pinkfish Exp pinkfish $
 * $Log: bob.c,v $
 * Revision 1.1  2000/05/10 07:55:11  pinkfish
 * Initial revision
 *
 * Revision 1.1  1998/01/06 04:11:58  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "fighting.combat.special.punch".
 * <p>
 * Describe the arguments in here.
 * <p>
 * This effect has a shadow associated with it.
 * @classification fighting.combat.special.punch
 * @see help::effects
 */
#include "path.h"
#include <effect.h>
#include <player.h>

/** @ignore yes */
void beginning( object player, object *things, int id ) {
   tell_object( player, "You prepare to bob "+
         (string)things[ 0 ]->the_short() +".\n" );
   player->set_data( ({ "unarmed", "blunt", "hands" }) );
   player->submit_ee( 0, 10 * COMBAT_SPEED, EE_REMOVE );
} /* beginning() */

/** @ignore yes */
void end( object player, object *things, int id ) {
   if ( sizeof( things ) )
      tell_object( player, "You missed your opportunity!\n" );
} /* end() */

void restart( object player, object *things, int id ) {
   player->submit_ee( 0, 0, EE_REMOVE );
} /* restart() */

/** @ignore yes */
string query_classification() { return "fighting.combat.special.punch"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS +"bob"; }
