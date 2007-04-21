/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: dont_move.c,v 1.1 2000/07/26 23:58:04 pinkfish Exp $
 * $Log: dont_move.c,v $
 * Revision 1.1  2000/07/26 23:58:04  pinkfish
 * Initial revision
 *
 * Revision 1.1  1998/01/06 04:16:50  ceres
 * Initial revision
 * 
*/
/**
 * This an effect that will stop an npc from moving for a while.  It
 * has a classification of "npc.move.no".
 * <p>
 * The argument is the amount of time the npc is to be frozen for.
 * <p>
 * This effect has a shadow associated with it.
 * @classification npc.move.no
 * @see help::effects
 */
#include <effect.h>
#include <route.h>
#include <wander.h>

#include "path.h"

/*
 * This effect will make the cute little critters eat things.
 */
/** @ignore yes */
void beginning( object player, int arg, int id ) {
   player->submit_ee(0, arg, EE_REMOVE);
} /* beginning() */

/** @ignore yes */
int merge_effect( object player, int time1, int time2, int id ) {
   player->submit_ee( 0, time1 + time2 - player->expected_tt(),
          EE_REMOVE );
   return time1 + time2;
} /* merge_effect() */
 
/** @ignore yes */
string query_classification() { return "npc.move.no"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS + "dont_move"; }
