.DT
goals.h
Disk World autodoc help
goals.h

.SH Description
.SP 5 5

The helper include file for the goals system.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Jul 30 18:08:47 PDT 1998
.EP



.SH See also
.SP 5 5
/std/living/goals.c and /std/npc/goals/base.c

.EP
.SH Classes

.SI 3
* %^BOLD%^goal_control%^RESET%^
class goal_control {
mapping extra_plans;
string current_plan;
int priority;
mixed data;
mapping plan_info;
}

.EI
.SP 7 5

The class used to keep track of the data passed into a goal.

.EP


.SH Defines
.SI 3
* %^BOLD%^GOAL_BASE_INHERIT%^RESET%^
.EI
.SP 7 5

The basic inherit for all npc goals.

.EP

.SI 3
* %^BOLD%^GOAL_CONTINUE_PROCESSING%^RESET%^
.EI
.SP 7 5

This is the return value for the goal which tells it to skip
this goal and continue onto the next.

.EP

.SI 3
* %^BOLD%^GOAL_DEFAULT_PRIORITY%^RESET%^
.EI
.SP 7 5

The default priority for a goal that does not have a priority set.

.EP

.SI 3
* %^BOLD%^GOAL_EVENT_HANDLED%^RESET%^
.EI
.SP 7 5

This is the return value from the event handling system if the event
was handled.

.EP

.SI 3
* %^BOLD%^GOAL_EVENT_NOT_HANDLED%^RESET%^
.EI
.SP 7 5

This is the return value from the event handling routine if the event
was not handled.

.EP

.SI 3
* %^BOLD%^GOAL_HIGH_PRIORITY%^RESET%^
.EI
.SP 7 5

A high priority goal.

.EP

.SI 3
* %^BOLD%^GOAL_INVALID_PRIORITY%^RESET%^
.EI
.SP 7 5

The invalid priority for a goal.

.EP

.SI 3
* %^BOLD%^GOAL_LOW_PRIORITY%^RESET%^
.EI
.SP 7 5

A low priority goal.

.EP

.SI 3
* %^BOLD%^GOAL_LOWEST_PRIORITY%^RESET%^
.EI
.SP 7 5

The lowest priority goal.

.EP

.SI 3
* %^BOLD%^GOAL_MEDIUM_PRIORITY%^RESET%^
.EI
.SP 7 5

A medium priority goal.

.EP

.SI 3
* %^BOLD%^GOAL_VERY_HIGH_PRIORITY%^RESET%^
.EI
.SP 7 5

A very high priority goal.

.EP

.SI 3
* %^BOLD%^GOALS_DIR%^RESET%^
.EI
.SP 7 5

The directory all the standard goals will go in.

.EP

