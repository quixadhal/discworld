/**
 * The helper include file for the goals system.
 * @author Pinkfish
 * @started Thu Jul 30 18:08:47 PDT 1998
 * @see /std/living/goals.c
 * @see /std/npc/goals/base.c
 */
#ifndef __GOALS_H__
/** @ignore yes */
#define __GOALS_H__

/**
 * The lowest priority goal.
 */
#define GOAL_LOWEST_PRIORITY 0
/**
 * A low priority goal.
 */
#define GOAL_LOW_PRIORITY 1000
/**
 * A medium priority goal.
 */
#define GOAL_MEDIUM_PRIORITY 2000
/**
 * A high priority goal.
 */
#define GOAL_HIGH_PRIORITY 3000
/**
 * A very high priority goal.
 */
#define GOAL_VERY_HIGH_PRIORITY 4000

/**
 * The invalid priority for a goal.
 */
#define GOAL_INVALID_PRIORITY -32768
/**
 * The default priority for a goal that does not have a priority set.
 */
#define GOAL_DEFAULT_PRIORITY GOAL_LOWEST_PRIORITY

/**
 * This is the return value for the goal which tells it to skip
 * this goal and continue onto the next.
 */
#define GOAL_CONTINUE_PROCESSING 1

/**
 * The directory all the standard goals will go in.
 */
#define GOALS_DIR "/std/npc/goals/"

/**
 * The basic inherit for all npc goals.
 */
#define GOAL_BASE_INHERIT (GOALS_DIR "base")

/**
 * The class used to keep track of the data passed into a goal.
 */
class goal_control {
   /** The extra plans associated with the goal. */
   mapping extra_plans;
   /** The current plan of the goal. */
   string current_plan;
   /** The override priority of the goal. */
   int priority;
   /** The data is controled by the goal itself. */
   mixed data;
   /** The plan data for each plan. */
   mapping plan_info;
}

/**
 * This is the return value from the event handling routine if the event
 * was not handled.
 */
#define GOAL_EVENT_NOT_HANDLED 0
/**
 * This is the return value from the event handling system if the event
 * was handled.
 */
#define GOAL_EVENT_HANDLED 1

#endif
