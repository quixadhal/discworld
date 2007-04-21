/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: spell_effects.c,v 1.1 1998/01/06 04:39:52 ceres Exp $
 * $Log: spell_effects.c,v $
 * Revision 1.1  1998/01/06 04:39:52  ceres
 * Initial revision
 * 
*/
inherit "/std/effect_shadow";

void do_spell_effects(object ob) {
   mixed *bit;
   int ret;

   bit = arg();
   if (functionp(bit)) {
      ret = evaluate(bit, player, ob);
   } else if (pointerp(bit)) {
      ret = call_other(bit[0], bit[1], player, ob);
   }
   player->do_spell_effects(ob);
} /* do_spell_effects() */
