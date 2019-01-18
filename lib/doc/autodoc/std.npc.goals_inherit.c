.DT
goals_inherit.c
Disk World autodoc help
goals_inherit.c

.SH Description
.SP 5 5

This is the goal handler for the npcs.  It will handle all the goals
and ordering them, figuring out which goal to run.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Jul 30 17:06:19 PDT 1998

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/npc/goals.h and /std/npc/goal_inherit.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^goal_information%^RESET%^
class goal_information {
mixed data;
int priority;
string * events;
}

.EI

.SI 3
* %^BOLD%^plan_info%^RESET%^
class plan_info {
int priority;
mixed data;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^activate_plan%^RESET%^
.EI
.SI 5
void activate_plan(string plan, string goal, mixed data)
.EI
.SP 7 5

This activates the plan on the npc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
plan - the plan to activate
.EP
.SP 9 5
goal - the goal which is activating the plan

.EP

.SI 3
* %^BOLD%^add_goal%^RESET%^
.EI
.SI 5
void add_goal(string goal, mixed data)
.EI
.SP 7 5

This method will add a goal to the NPC.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
goal - the goal to add
.EP
.SP 9 5
data - the data associated with the goal

.EP

.SI 3
* %^BOLD%^add_goal_event%^RESET%^
.EI
.SI 5
void add_goal_event(string goal, string event)
.EI
.SP 7 5

This method adds an event for a goal to react to.  This should only be
done from inside the goal initialisation code, and other parts of the
goal.  We will only add things for goals which we know about.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
goal - the goal the event is for
.EP
.SP 9 5
event - the event name to react to
.EP

.SI 3
* %^BOLD%^change_goal_priority%^RESET%^
.EI
.SI 5
void change_goal_priority(string goal)
.EI
.SP 7 5

This method changes the priority of the specified goal.  It will recall
the query_priority() method on the goal to figure out the new priority.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
goal - the goal whose priority to change
.EP

.SI 3
* %^BOLD%^check_current_plan_finished%^RESET%^
.EI
.SI 5
int check_current_plan_finished()
.EI
.SP 7 5

This method checks to see if the top plan is finished or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
plan - the plan to finish
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the plan has been finished, 0 if not

.EP

.SI 3
* %^BOLD%^notify_npc_event%^RESET%^
.EI
.SI 5
varargs void notify_npc_event(string event, mixed * information ...)
.EI
.SP 7 5

This method is called when an event needs to be notified about.
The event handling routines should try to be short.  They should
just modify the priority of something, or add a new goal to the
current list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
event - the event to tell us about
.EP
.SP 9 5
information - the information associated with the event

.EP

.SI 3
* %^BOLD%^query_events%^RESET%^
.EI
.SI 5
mapping query_events()
.EI
.SP 7 5

This method is *only* to be used for debugging.

.EP

.SI 3
* %^BOLD%^query_goal_data%^RESET%^
.EI
.SI 5
mixed query_goal_data(string goal)
.EI
.SP 7 5

This method returns the data associated with this goal.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
goal - the goal to get the data for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the data associated with the goal
.EP

.SI 3
* %^BOLD%^query_goal_plan_data%^RESET%^
.EI
.SI 5
mixed query_goal_plan_data(string goal, string plan)
.EI
.SP 7 5

This method returns the data associated with the specified plan.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
goal - the foal to query the information for
.EP
.SP 9 5
plan - the name of the plan to get the data for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the plans data
.EP

.SI 3
* %^BOLD%^query_goal_priority%^RESET%^
.EI
.SI 5
int query_goal_priority(string goal)
.EI
.SP 7 5

This method returns the priotity of the goal.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
goal - the goal to query the priority of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the priority of the goal
.EP

.SI 3
* %^BOLD%^query_goals%^RESET%^
.EI
.SI 5
mapping query_goals()
.EI
.SP 7 5

This method is *only* to be used for debugging.

.EP

.SI 3
* %^BOLD%^remove_goal%^RESET%^
.EI
.SI 5
void remove_goal(string goal)
.EI
.SP 7 5

This method removes the goal from the current goal list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
goal - the goal to remove

.EP

.SI 3
* %^BOLD%^remove_goal_event%^RESET%^
.EI
.SI 5
void remove_goal_event(string goal, string event)
.EI
.SP 7 5

This method removes the goal event for the goal.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
goal - the goal to remove the event for
.EP
.SP 9 5
event - the event name to not react to
.EP

.SI 3
* %^BOLD%^set_goal_data%^RESET%^
.EI
.SI 5
int set_goal_data(string goal, mixed data)
.EI
.SP 7 5

This method returns the data associated with this goal.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
goal - the goal to get the data for
.EP
.SP 9 5
data - the new data for the goal
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^set_goal_plan_data%^RESET%^
.EI
.SI 5
void set_goal_plan_data(string goal, string plan, mixed data)
.EI
.SP 7 5

This method sets the default data associated with the specified plan.
This should be things like global plan things, the level at which the
npc will run away.  Etc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
goal - the goal to check the data for
.EP
.SP 9 5
plan - the name of the plan
.EP
.SP 9 5
data - the data associated with the plan
.EP

.SI 3
* %^BOLD%^set_plan_data%^RESET%^
.EI
.SI 5
void set_plan_data(string plan, mixed data)
.EI
.SP 7 5

This method changes the data associated with the specified plan.  This
should only be called from the plan itself, as the plan will not be
informed of this change.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
data - the new data for the plan

.EP


