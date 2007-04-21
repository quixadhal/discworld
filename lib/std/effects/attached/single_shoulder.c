/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: single_shoulder.c,v 1.4 2000/05/10 03:03:03 pinkfish Exp $
 * $Log: single_shoulder.c,v $
 * Revision 1.4  2000/05/10 03:03:03  pinkfish
 * Make sure it works correctly.
 *
 * Revision 1.3  2000/05/08 17:56:09  pinkfish
 * Fix up some runtimes.
 *
 * Revision 1.2  2000/05/08 03:55:32  pinkfish
 * *** empty log message ***
 *
 * Revision 1.1  1998/01/06 04:06:32  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "mudlib.clothing.single-shoulder".
 * <p>
 * Describe the arguments in here.
 * <p>
 * This effect has a shadow associated with it.
 * @classification mudlib.clothing.single-shoulder
 * @see help::effects
 */
#include "path.h"
#include <effect.h>

/** @ignore yes */
string query_classification() { return "mudlib.clothing.single-shoulder"; }

int query_indefinite() { return 1; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS +"single_shoulder"; }

/** @ignore yes */
int merge_effect( object, int old_arg, int new_arg ) {
   if (objectp(new_arg)) {
      new_arg = 0;
   }
   if (objectp(old_arg)) {
      old_arg = 0;
   }
   return old_arg + new_arg;
} /* merge_effect() */

void restart( object player, int handicap ) {
   player->adjust_bonus_dex( -handicap );
} /* restart() */

/** @ignore yes */
void end( object player, int handicap ) {
   if (intp(handicap)) {
      player->adjust_bonus_dex( handicap );
   }
} /* end() */
