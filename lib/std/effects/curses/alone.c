/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: alone.c,v 1.1 1998/01/06 04:07:14 ceres Exp $
 * $Log: alone.c,v $
 * Revision 1.1  1998/01/06 04:07:14  ceres
 * Initial revision
 * 
*/
/**
 * Make the player feel they are alone on the disc.  The classification of this
 * effect is "misc.curse.alone".
 * <p>
 * The argument to the effect does nothing at all.  The effect
 * can only be removed by a call to remove effect.  It does not
 * time out.
 * <p>
 * This effect has a shadow associated with it.
 * @classification misc.curse.alone
 * @example
 * add_effect("/std/effects/curses/alone");
 */
#include <effect.h>
#include "path.h"

/** @ignore yes */
string query_classification() { return "misc.curse.alone"; }
/** @ignore yes */
string query_shadow_ob() { return SHADOWS+"alone"; }

/** @ignore yes */
int beginning( object player, int time ) { 
   player->add_extra_look( this_object() );
} /* beginning() */

/** @ignore yes */
void merge_effect( object player, int time1, int time2 ) {
} /* merge_effect() */

/** @ignore yes */
void end(object player) { 
   player->remove_extra_look( this_object() );
} /* end() */

/** @ignore yes */
string extra_look( object player ) {
   if ( player == this_player() )
     return "";
   return capitalize( (string)player->query_pronoun() )+
     " can't see you.\n";
} /* extra_look() */

