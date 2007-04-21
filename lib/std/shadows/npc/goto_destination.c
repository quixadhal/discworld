/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: goto_destination.c,v 1.2 1999/07/28 17:59:43 wodan Exp $
 * 
*/
/*
 * The goto destination shadow.
 */
inherit "/std/effect_shadow";

object womble;
int done, moved;

/* When it finishes.  We want to know. */
void stopped_route() {
   /* Is this a find player effect? */
   if (!done) {
      mixed *rabbit;

      rabbit = arg();
      if (objectp(rabbit[0])) {
         womble = rabbit[0];
      }
      done = 1;
   }

   if (womble && environment(womble) != environment(player)) {
      if (!moved) {
         player->move(environment(womble),
                      (string)player->query_mmsgin(),
                      (string)player->query_mmsgout() );
         call_out("remove_this_effect", 0);
      } else {
         player->move_me_to(file_name(environment(womble)));
      }
   } else {
      call_out("remove_this_effect", 0);
   }
} /* stopped_route() */

int move(object ob, string in_thing, string out_thing) {
   int ret;

   if (!done) {
      mixed *rabbit;

      rabbit = arg();
      done = 1;
      if (arrayp(rabbit) && objectp(rabbit[0])) {
         womble = rabbit[0];
         done = 2;
      }
   }

   ret = player->move(ob, in_thing, out_thing);

   if (!ret && womble && environment(womble) == environment(player)) {
      /* We have found our player... */
      player->stop_following_route();
      remove_this_effect();
   }

   if (done == 2 && !womble) {
      call_out("remove_this_effect", 4);
   }

   if (!ret)
      moved = 1;

   return ret;
} /* move() */
