/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: no_follow.c,v 1.1 1998/01/06 04:16:50 ceres Exp $
 * $Log: no_follow.c,v $
 * Revision 1.1  1998/01/06 04:16:50  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "npc.follow.guard_duty".
 * <p>
 * Describe the arguments in here.
 * <p>
 * This effect has a shadow associated with it.
 * @classification npc.follow.guard_duty
 * @see help::effects
 */
#include <effect.h>

#include "path.h"
#define SHADOWS  CROWD

/*
 * This effect will make npcs return to the crowd
 * arg() My home object
 */

/** @ignore yes */
mixed beginning( object player, mixed arg, int id ) {
  call_out( (: $1->crowd_merge() :), 5, player );
} /* beginning() */

/** @ignore yes */
mixed merge_effect( object player, mixed oldarg, mixed newarg, int id ) {
   return newarg;
} /* merge_effect() */
 
/** @ignore yes */
string query_classification() { return "npc.crowd.merge"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS + "crowd_merge_shadow"; }







