/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: light_change.c,v 1.1 1998/01/06 04:39:52 ceres Exp $
 * $Log: light_change.c,v $
 * Revision 1.1  1998/01/06 04:39:52  ceres
 * Initial revision
 * 
*/
inherit "/std/effect_shadow";

void event_light_change(object ob, int delta) {
   call_out("delayed_event_light_change",0, ob, delta );
} /* event_light_change() */

void delayed_event_light_change(object ob, int delta) {
   mixed *bit;
   int ret;

   bit = arg();
   if (functionp(bit)) {
      ret = evaluate(bit, player, ob, delta);
   } else if (pointerp(bit)) {
      ret = call_other(bit[0], bit[1], player, ob, delta);
   }
   player->event_light_change(ob, delta);
}
