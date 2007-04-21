/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: savage_corpse.c,v 1.2 1999/12/31 19:23:03 ceres Exp $
 * $Log: savage_corpse.c,v $
 * Revision 1.2  1999/12/31 19:23:03  ceres
 * Updated with Oaf's changes
 *
 * Revision 1.1  1998/01/06 04:39:52  ceres
 * Initial revision
 * 
*/
/*
 * Rip apart a corpse now and then to remove bits off it (then eat them).
 * This should be used in conjunction with the eat_edible effect to
 * actually eat the stuff ripped off.
 */
inherit "/std/effect_shadow";

mixed move(mixed dest, string messin, string messout) {
   int ret;

   ret = player->move(dest, messin, messout);
   if (ret == 0) {
      call_out("do_savage_corpse", 0);
   }
   return ret;
} /* move() */

protected void do_savage_corpse() {
   int i;
   object *obs;

   obs = all_inventory(environment(player));
   for (i = 0; i < sizeof(obs); i++) {
      if (obs[i]->query_corpse() &&
          sizeof(obs[i]->query_edible_bits_left()) > 0) {
         remove_call_out("real_savage");
         call_out("real_savage", 1 + random(120), obs[i]);
         break;
      }
   }
} /* do_save_corpse() */

protected void real_savage(object ob) {
   string *bits;
   object bit;
   int i;

   if (!ob) {
      call_out("do_savage_corpse", 1 + random(2*60));
      return ;
   }
   bits = ob->query_edible_bits_left();
   if (sizeof(bits) > 0 && environment(ob) == environment(player)) {
      i = random(sizeof(bits));
      bit = ob->make_bit(bits[i]);
      tell_room(environment(player), capitalize(player->the_short()) +
                " rips apart " + ob->the_short() +
                " pulling off " + bit->a_short() + ".\n");
      bit->move(environment(player));
   }

   if (sizeof(bits) > 1) {
      /* Every 2 minutes or so */
      remove_call_out("real_savage");
      call_out("real_savage", 1 + random(2*60), ob);
   } else {
      call_out("do_savage_corpse", 1 + random(2*60));
   }
} /* real_savage() */

void event_enter(object ob, string mess, object from) {
   player->event_enter(ob, mess, from);
   if (ob->query_corpse()) {
      remove_call_out("real_savage");
      call_out("real_savage", 10, ob);
   }
} /* event_enter() */
