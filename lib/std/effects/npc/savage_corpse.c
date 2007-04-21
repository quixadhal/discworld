/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: savage_corpse.c,v 1.1 1998/01/06 04:16:50 ceres Exp $
 * $Log: savage_corpse.c,v $
 * Revision 1.1  1998/01/06 04:16:50  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "npc.eat.savage".
 * <p>
 * Describe the arguments in here.
 * <p>
 * This effect has a shadow associated with it.
 * @classification npc.eat.savage
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
void beginning( object player, string dest, int id ) {
} /* beginning() */

/** @ignore yes */
int merge_effect( object player, int time1, int time2, int id ) {
   return time1;
} /* merge_effect() */
 
/** @ignore yes */
string query_classification() { return "npc.eat.savage"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS + "savage_corpse"; }
