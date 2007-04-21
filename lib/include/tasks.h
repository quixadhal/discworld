#ifndef __TASKS_H
#define __TASKS_H

#define TASKER "/obj/handlers/taskmaster"

// Perform_task result codes
#define BARF      -1
#define FAIL      0
#define SUCCEED   1
#define AWARD     2

// compare_skills result codes
#define OFFAWARD  -2
#define OFFWIN    -1
#define DRAW      0
#define DEFWIN    1
#define DEFAWARD  2

// Perform_task result codes with better names.
#define TASKER_BARF      -1
#define TASKER_FAIL      0
#define TASKER_SUCCEED   1
#define TASKER_AWARD     2

// compare_skills result codes with better names.
#define TASKER_OFFAWARD  -2
#define TASKER_OFFWIN    -1
#define TASKER_DRAW      0
#define TASKER_DEFWIN    1
#define TASKER_DEFAWARD  2

// Degree-of-success values
#define TASKER_MARGINAL     1
#define TASKER_NORMAL       2
#define TASKER_EXCEPTIONAL  3
#define TASKER_CRITICAL     4

// Upper bounds of the d-o-s ranges.
// These are only really used internally...
#define TASKER_MARGINAL_UPPER        10
#define TASKER_NORMAL_UPPER          80
#define TASKER_EXCEPTIONAL_UPPER     95

// How likely you are to get a critical result if you're within
// the critical range.  Also only really used internally.
#define TASKER_CRITICAL_PERCENTAGE   5

// these are used in tweaking the TM rate for different levels of skill
#define BASE 100.0
#define DECAY 300.0
#define MODIFIER 10
#define E_MODIFIER 5

/*
 * these are temporary defines till the new TM stuff is working.
 */
#define COVERT_TM 100, ({0, 5})
#define CONTINUOUS_COVERT_TM 100, ({0, 3})
#define PERCEPTION_TM 50

#define FAITH_TM 75
#define FAITH_TM_HALF 30

#define MAGIC_TM 75
#define MAGIC_TM_HALF 30

#define OTHER_TM 100

// New definitions for different task types
#define TM_FIXED 1      // A fixed skill test
#define TM_FREE 2       // A free skill test in a room or somesuch
#define TM_CONTINUOUS 3 // continuous task
#define TM_COMMAND 4    // a command
#define TM_RITUAL 5     // a ritual
#define TM_SPELL 6      // a spell
#define TM_NONE 7       // just a test, no chance of an increase

/**
 * This is the class you get when using the degree of success code for the
 * taskmaster.
 * @member result This is the same as the return value would normally be
 *   when using the binary version. (SUCCEED/FAIL/AWARD/etc.)
 * @member degree This is the degree of success, based on how well
 *   you succeeded or how badly you failed.
 * @member raw This is included to enable you to tweak damage/duration/
 *   strength/etc transparently and smoothly.  Never use this to determine
 *   degree of success; use the 'degree' member for that.
 */
class tasker_result {
   int result;
   int degree;
   int raw;
} /* class tasker_result */

#endif   // __TASKS_H
