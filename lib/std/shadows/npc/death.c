/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: death.c,v 1.1 1998/01/06 04:39:52 ceres Exp $
 * $Log: death.c,v $
 * Revision 1.1  1998/01/06 04:39:52  ceres
 * Initial revision
 * 
*/
inherit "/std/effect_shadow";

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

   remove_this_effect();
   return ret;
} /* second_life() */
