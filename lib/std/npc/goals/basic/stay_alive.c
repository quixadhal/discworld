/**
 * This is the very basic stay alive goal.  It will attempt to keep the npc
 * alive if they are being beaten up by someone.  All this goal will do is
 * cause the npc to run away, and attempt to stop the player from following
 * them.  This is exactly the same as the previous wimpy code.
 * @author Pinkfish
 * @started Sat Aug  1 18:15:30 PDT 1998
 */
#include <goals.h>

inherit BASE_GOAL;

void create() {
   ::create();
   // All keep alive goals should be at a high priority.
   set_default_priority(GOAL_HIGH_PRIORITY);
   add_default_plan(EVENT_HP_LOST,
                    GOAL_PLAN_DIR + "basic/run_away");
   // This method will only lose people attacking them.
   add_default_plan(EVENT_PLAN_FINISHED,
                    GOAL_PLAN_DIR + "basic/lose_attackers");
} /* create() */
