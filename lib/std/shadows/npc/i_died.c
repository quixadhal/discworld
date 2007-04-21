/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: i_died.c,v 1.2 2001/02/21 06:37:31 shrike Exp $
 * $Log: i_died.c,v $
 * Revision 1.2  2001/02/21 06:37:31  shrike
 * changed from remove_this_effect() to submit_ee2() in an attempt to fix undead NPC porbelms.
 *
 * Revision 1.1  1998/01/06 04:39:52  ceres
 * Initial revision
 * 
*/
#include <effect.h>

inherit "/std/effect_shadow";

#define CLASS "npc.death.me"

int second_life() {
   mixed *bit;
   int ret;

   bit = arg();
   if (functionp(bit)) {
      ret = evaluate(bit, player);
   } else if (pointerp(bit)) {
      ret = call_other(bit[0], bit[1], player);
   }

   if (!pointerp(bit) || sizeof(bit) < 3 || !bit[2]) {
      ret = player->second_life();
   }

   player->submit_ee2( player->effects_matching(CLASS)[0], 0, 0, EE_REMOVE );
   return ret;
} /* second_life() */
