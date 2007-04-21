/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: controlled_monster.c,v 1.1 1998/01/06 04:16:50 ceres Exp $
 * $Log: controlled_monster.c,v $
 * Revision 1.1  1998/01/06 04:16:50  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "npc.controlled_monster".
 * <p>
 * Describe the arguments in here.
 * <p>
 * This effect has a shadow associated with it.
 * @classification npc.controlled_monster
 * @see help::effects
 */
#include "path.h"

/* This effect will make the critter controllable.
 */
/** @ignore yes */
string query_shadow_ob() { return SHADOWS + "controlled_monster"; }

/** @ignore yes */
void beginning( object player, string dest, int id ) {
} /* beginning() */

/** @ignore yes */
int merge_effect( object player, int time1, int time2, int id ) {
   return time2;
} /* merge_effect() */
 
/** @ignore yes */
string query_classification() { return "npc.controlled_monster"; }

