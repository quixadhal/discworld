/**
 * This is a test npc ai object.
 * @author Pinkfish
 * @started Tue Aug  4 19:13:19 PDT 1998
 */
#include <npc/events.h>

inherit "/obj/monster";
inherit "/std/npc/goals_inherit";

int _last_hp;

protected int check_heart_beat_results();

void create() {
   goals_inherit::create();
   monster::create();
} /* create() */

/** @ignore yes */
void heart_beat() {
   int hp;

   ::heart_beat();
   call_out((: check_heart_beat_results :), 0);
} /* heart_beat() */

/**
 * This is just to make sure that the heart beat is not broken by
 * our processing errors.
 * @ignore yes
 */
protected int check_heart_beat_results() {
   hp = query_hp();
   if (hp != _last_hp) {
      if (hp > _last_hp) {
         notify_npc_event(EVENT_GAIN_HPS, hp - _last_hp, hp);
      } else {
         notify_npc_event(EVENT_LOSE_HPS, _last_hp - hp, hp);
      }
      _last_hp = hp;
   }
} /* heart_beat() */

/** @ignore yes */
mixed *stats() {
   return monster::stats() + goals_inherit::stats();
} /* stats() */

/** @ignore yes */
int move(mixed dest, string messin, string messout) {
   int ret;
   object from;

   from = environment();
   ret = ::move(dest, messin, messout);
   if (from) {
      call_out((: notify_npc_event :), 0, EVENT_MOVED, from);
   }
   return ret;
} /* move() */

/** @ignore yes */
void event_enter(object moving, string mess, object from) {
   if (living(from)) {
      call_out((: notify_npc_event :), 0, EVENT_GIVEN, moving);
   }
} /* event_enter() */
