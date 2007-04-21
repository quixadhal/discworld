/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: pub_brawl.c,v 1.1 1998/01/06 04:39:52 ceres Exp $
 * $Log: pub_brawl.c,v $
 * Revision 1.1  1998/01/06 04:39:52  ceres
 * Initial revision
 * 
*/
inherit "/std/effect_shadow";

void event_pub_brawl(object ob, string mess) {
   mixed *bit;
   int ret;

   bit = arg();
   if (functionp(bit)) {
      ret = evaluate(bit, player, ob, mess);
   } else if (pointerp(bit)) {
      ret = call_other(bit[0], bit[1], player, ob, mess);
   }
   player->event_pub_brawl(ob, mess);
} /* event_pub_brawl() */
