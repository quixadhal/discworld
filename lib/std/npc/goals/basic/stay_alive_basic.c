/**
 * This is the very basic stay alive goal.  It will attempt to keep the npc
 * alive if they are being beaten up by someone.  All this goal will do is
 * cause the npc to run away, and attempt to stop the player from following
 * them.  This is exactly the same as the previous wimpy code.
 * @author Pinkfish
 * @started Sat Aug  1 18:15:30 PDT 1998
 */
#include <npc/goals.h>
#include <npc/events.h>
#include <npc/plan.h>

inherit GOAL_BASE_INHERIT;

/** @ignore yes */
void setup() {
   // All keep alive goals should be at a high priority.
   set_default_priority(GOAL_HIGH_PRIORITY);
   add_default_plan(EVENT_LOSE_HPS,
                    PLAN_DIR + "basic/run_away");

   // This method will only lose people attacking them.
   /*
   add_default_plan(EVENT_PLAN_FINISHED,
                    PLAN_DIR + "basic/lose_attackers");
    */
} /* setup() */

/**
 * This goal is never completed...
 * @ignore yes
 */
int invarient(object npc) {
   return 1;
} /* invarient() */

/**
 * This setups all the goal specific stuff for this goal.  In the case of
 * this goal, the data is the wimpy value.
 * @param npc the npc the goal is being added to
 * @param goal_name the name of the goal
 * @param stuff the parameter passed in by the creator
 * @return any specific information to store in the goal data
 * @ignore yes
 */
mixed initialise_plans(object npc, 
                       string goal_name,
                       int wimpy) {
tell_creator("pinkfish", "Initialise_plans(%O, %O, %O)\n", npc, goal_name, wimpy);
   /* Setup the wimpy values for the run away plan. */
   npc->set_goal_plan_data(goal_name, 
                           PLAN_DIR + "basic/run_away",
                           wimpy);
   return wimpy;
} /* create_goal_data() */
