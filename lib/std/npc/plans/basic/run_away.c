/**
 * This is the run away plan.  It will cause the npc to dash off somewhere
 * in terrible excitement.
 * @author Pinkfish
 * @started Wed Sep 23 00:15:02 EDT 1998
 */

#include <npc/plan.h>
#include <npc/events.h>

inherit PLAN_BASE_INHERIT;

int moved_event(object npc,
                mixed plan_data,
                string event,
                mixed *data);

/** @ignore yes */
void setup() {
   set_sutability(PLAN_MEDIUM_SUTABILITY);
   add_hooked_event(EVENT_MOVED, (: moved_event :));
} /* setup() */

/** @ignore yes */
protected int being_attacked(object npc) {
   object *attackers;

   attackers = filter(npc->query_attacker_list(), (: environment($1) ==
                                                 environment($2) :), npc);
   return sizeof(attackers);
} /* being_attacked() */

/** @ignore yes */
void activate_plan(object npc,
                   mixed data) {
   /* This should do the run away thing. */
   npc->run_away();
   //   npc->remove_plan(base_name(this_object()));
} /* activate_plan() */

/** @ignore yes */
int pre_condition(object npc,
                  string goal,
                  mixed plan_info,
                  string event,
                  mixed *data) {
   int wimpy_level;
   int percent;

tell_creator("pinkfish", "Precondition!\n");
   // The wimpy level should be determined by the plan.
   if (!plan_info) {
      wimpy_level = 50;
   } else {
      wimpy_level = plan_info;
   }

tell_creator("pinkfish", sprintf("%O: %O, %O\n", event, goal, plan_info));
   percent = (npc->query_hp() * 100) / npc->query_max_hp();
   if (percent < wimpy_level) {
      // Being attacked or we lost hps.  Out of here.
      if (being_attacked(npc) ||
          event == EVENT_LOSE_HPS) {
         return 1; 
      }
   }
   return 0;
} /* pre_condition() */

/** @ignore yes */
int finished(object npc) {
   /* We are finished if there are no attackers in our environment. */
   return !being_attacked(npc);
} /* finished() */

/** @ignore yes */
int moved_event(object npc,
                mixed plan_data,
                string event,
                mixed *data) {
   // We moved!  Maybe now we are finished?
   if (finished(npc)) {
      return 1;
   } else {
      // Need to run away again dammit!
      npc->run_away();
   }
   return 0;
} /* notify_event() */
