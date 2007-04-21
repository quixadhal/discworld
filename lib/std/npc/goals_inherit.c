/**
 * This is the goal handler for the npcs.  It will handle all the goals
 * and ordering them, figuring out which goal to run.
 * @author Pinkfish
 * @started Thu Jul 30 17:06:19 PDT 1998
 */
#include <npc/goals.h>
// Local inherit stuff.
#include "goal_inherit.h"

class goal_information {
   mixed data;
   int priority;
   string *events;
}

class plan_info {
   int priority;
   mixed data;
}

// The index of the mapping is the goal.
private nosave mapping _goals;
private nosave mapping _events;
private nosave mapping _emotions;
// The format of this array is:
//  ({ priority, plan, priority, plan, ... })
private nosave mixed* _plans;
private nosave string  _current_plan;

private class goal_information query_goal_information(string goal);
void remove_goal_event(string goal, string event);
void add_goal_event(string goal, string event);
int query_goal_priority(string goal);
int check_current_plan_finished();

void create() {
   _goals = ([ ]);
   _events = ([ ]);
   _plans = ({ });
} /* create() */

/**
 * This method is *only* to be used for debugging.
 */
mapping query_goals() {
   return _goals;
} /* query_goals() */

/**
 * This method is *only* to be used for debugging.
 */
mapping query_events() {
   return _events;
} /* query_events() */

/**
 * This method will add a goal to the NPC.
 * @param goal the goal to add
 * @param data the data associated with the goal
 */
void add_goal(string goal, mixed data) {
   class goal_information frog;

   if (!goal->invarient(this_object())) {
      // Cannot add the goal.
      return ;
   }

   if (!_goals[goal]) {
      frog = new(class goal_information);
      frog->data = data;
      frog->priority = goal->query_priority(this_object(), data);
      frog->events = ({ });
      _goals[goal] = frog;
      goal->initialise(this_object(), goal, data);
   } else {
      goal->add_again(this_object(), query_goal_information(goal)->data, data);
   }
} /* add_goal() */

/**
 * This method removes the goal from the current goal list.
 * @param goal the goal to remove
 */
void remove_goal(string goal) {
   class goal_information frog;
   string event;

   frog = query_goal_information(goal);
   if (frog) {
      goal->finalise(this_object(), frog->data);
      foreach (event in frog->events) {
         remove_goal_event(goal, event);
      }
      map_delete(_goals, goal);
   }
} /* remove_goal() */

/**
 * This method gets the information associated with the goal.
 * @param goal the goal to get the information for
 * @return the goal information
 */
private class goal_information query_goal_information(string goal) {
   class goal_information frog;

   frog = _goals[goal];
   return frog;
} /* query_goal_information() */

/**
 * This method returns the data associated with this goal.
 * @param goal the goal to get the data for
 * @return the data associated with the goal
 * @see query_goal_priority()
 * @see set_goal_data()
 */
mixed query_goal_data(string goal) {
   class goal_information frog;

   frog = query_goal_information(goal);
   if (frog) {
      return frog->data;
   }
   return 0;
} /* query_goal_data() */

/**
 * This method returns the data associated with this goal.
 * @param goal the goal to get the data for
 * @param data the new data for the goal
 * @return 1 on success, 0 on failure
 * @see query_goal_priority()
 * @see query_goal_data()
 */
int set_goal_data(string goal, mixed data) {
   class goal_information frog;

   frog = query_goal_information(goal);
   if (frog) {
      frog->data = data;
      return 1;
   }
   return 0;
} /* set_goal_data() */

/**
 * This method returns the priotity of the goal.
 * @param goal the goal to query the priority of
 * @return the priority of the goal
 * @see change_goal_priority()
 */
int query_goal_priority(string goal) {
   class goal_information frog;

   frog = query_goal_information(goal);
   if (frog) {
      return frog->priority;
   }
   return GOAL_INVALID_PRIORITY;
} /* query_priority() */

/**
 * This method changes the priority of the specified goal.  It will recall
 * the query_priority() method on the goal to figure out the new priority.
 * @param goal the goal whose priority to change
 * @see add_goal_event()
 * @see remove_goal_event()
 * @see query_goal_priority()
 */
void change_goal_priority(string goal) {
   class goal_information frog;
   int new_priority;
   string evt;

   frog = query_goal_information(goal);
   if (frog) {
      new_priority = goal->query_priority(this_object(), frog->data);
      if (new_priority != frog->priority) {
         frog->priority = new_priority;
      }
      foreach (evt in frog->events) {
         /* Put itself back into the event list. */
         remove_goal_event(goal, evt);
         add_goal_event(goal, evt);
      }
   }
} /* change_goal_priority() */

/**
 * This method adds an event for a goal to react to.  This should only be
 * done from inside the goal initialisation code, and other parts of the
 * goal.  We will only add things for goals which we know about.
 * @param goal the goal the event is for
 * @param event the event name to react to
 * @see change_priority()
 * @see remove_goal_event()
 */
void add_goal_event(string goal, string event) {
   class goal_information frog;
   string test_goal;
   int i;
   int priority;
   int found;

   frog = query_goal_information(goal);
   if (frog) {
      if (!_events[event]) {
         _events[event] = ({ goal });
      } else if (member_array(goal, _events[event]) == -1) {
         // Place the goal into the list in priority order.
         priority = query_goal_priority(goal);
         while (i < sizeof(_events[event])) {
            test_goal = _events[event][i];
            if (query_goal_priority(_events[event][i]) < priority) {
               _events[event] = _events[event][0..i-1] + ({ goal }) +
                                _events[event][i..];
               found = 1;
               break;
            }
            i++;
         }
         if (!found) {
            _events[event] += ({ goal });
         }
      }
      frog->events += ({ event });
   }
} /* add_goal_event() */

/**
 * This method removes the goal event for the goal.
 * @param goal the goal to remove the event for
 * @param event the event name to not react to
 * @see add_goal_event()
 * @see change_priority()
 */
void remove_goal_event(string goal, string event) {
   class goal_information frog;

   frog = query_goal_information(goal);
   if (frog && _events[event]) {
      _events[event] -= ({ goal });
      if (!sizeof(_events[event])) {
         map_delete(_events, event);
      }
      frog->events -= ({ event });
   }
} /* remove_goal_event() */

/**
 * This method is called when an event needs to be notified about.
 * The event handling routines should try to be short.  They should
 * just modify the priority of something, or add a new goal to the
 * current list.
 * @param event the event to tell us about
 * @param information the information associated with the event
 */
varargs void notify_npc_event(string event, mixed *information ...) {
   string goal;

tell_creator("pinkfish", "%s %O\n", event, _events[event]);
   if (_events[event]) {
      foreach (goal in _events[event]) {
  	 // If the event is handled then we break out.
         if (goal->notify_of_event(this_object(),
                                   query_goal_information(goal)->data,
                                   event,
                                   information)) {
            return ;
         }
      }
   }

   if (sizeof(_plans)) {
      if (_plans[PLAN_PLAN]->notify_of_event(this_object(),
                                      ((class plan_info)_plans[PLAN_INFO])->data,
                                      event,
				      information)) {
         check_current_plan_finished();
      }
   }
} /* notify_npc_event() */

/**
 * This activates the plan on the npc.
 * @param plan the plan to activate
 * @param goal the goal which is activating the plan
 */
void activate_plan(string plan,
                   string goal,
                   mixed data) {
   int priority;
   int pos;
   int i;
   int old;
   mixed *tmp;
   class plan_info info;

   priority = query_goal_priority(goal);
   /* First see if the plan is already activated. */
   pos = member_array(plan, _plans);
   if (pos != -1) {
      /* Update the data value */
      info = _plans[pos + PLAN_INFO];
      info->data = plan->combine_plans(info->data, data);
      if (info->priority != priority) {
         /* Priority has changed... Tricky... */
         /* First see if we should move up the list. */
         old = -1;
         for (i = pos - PLAN_ARRAY_SIZE; i >= 0; i -= PLAN_ARRAY_SIZE) {
            if (priority > ((class plan_info)_plans[i + PLAN_INFO])->priority) {
               old = i;
            } else {
               break;
            }
         }
         if (old != -1) {
            /* We move upwards! */
            tmp = _plans[pos..pos + PLAN_ARRAY_SIZE];
            _plans = _plans[0..pos - 1] + _plans[pos + PLAN_ARRAY_SIZE..];
            _plans = _plans[0..old - 1] + tmp + _plans[old..];
         } else {
            /* Check to see if we need to move down. */
            for (i = pos + PLAN_ARRAY_SIZE; i < sizeof(_plans);
                 i += PLAN_ARRAY_SIZE) {
               if (priority < ((class plan_info)_plans[i + PLAN_INFO])->priority) {
                  old = i;
               } else {
                  break;
               }
            }
            if (old != -1) {
               /* We move down... :( */
               tmp = _plans[old..old + PLAN_ARRAY_SIZE];
               _plans = _plans[0..old - 1] + _plans[old + PLAN_ARRAY_SIZE..];
               _plans = _plans[0..pos - 1] + tmp + _plans[pos..];
            }
         }
      }
   } else {
      old = -1;

      info = new(class plan_info);
      info->data = data;
      info->priority = priority;
      /* Try to find the right spot to put it into the array. */
      for (i = 0; i < sizeof(_plans); i += PLAN_ARRAY_SIZE) {
         if (priority > ((class plan_info)_plans[i + PLAN_INFO])->priority) {
            /* We go here... */
            _plans = _plans[0..i - 1] + ({ plan, info }) +
                     _plans[i..];
            old = i + PLAN_ARRAY_SIZE;
            break;
         }
      }
      if (i >= sizeof(_plans)) {
         _plans += ({ plan, info });
      }
   }

tell_creator("pinkfish", "Activating %O, %O, %O\n", _current_plan, _plans[PLAN_PLAN], _plans[PLAN_INFO]);
   /* Check to see if any plans should be suspended or activated. */
   if (_plans[PLAN_PLAN] != _current_plan) {
      if (_current_plan) {
         pos = member_array(_current_plan, _plans);
         if (pos != -1) {
            _plans[pos + PLAN_PLAN]->suspend_plan(this_object(),
                                                  ((class plan_info)_plans[pos + PLAN_INFO])->data);
         }
      }
      _plans[PLAN_PLAN]->activate_plan(this_object(),
                                       ((class plan_info)_plans[PLAN_INFO])->data);
      // Check to see if any of the plans are completed now...
      while (check_current_plan_finished());
   }
} /* activate_plan() */

/**
 * This method checks to see if the top plan is finished or not.
 * @param plan the plan to finish
 * @return 1 if the plan has been finished, 0 if not
 */
int check_current_plan_finished() {
   if (sizeof(_plans)) {
      if (_plans[PLAN_PLAN]->finished(this_object(),
                                      ((class plan_info)_plans[PLAN_INFO])->data)) {
         _plans = _plans[PLAN_ARRAY_SIZE..];
         if (sizeof(_plans)) {
            _plans[PLAN_PLAN]->activate_plan(this_object(),
                                             ((class plan_info)_plans[PLAN_INFO])->data);
         }
         return 1;
      }
   }
   return 0;
} /* finish_plan() */

/**
 * This method changes the data associated with the specified plan.  This
 * should only be called from the plan itself, as the plan will not be
 * informed of this change.
 * @param data the new data for the plan
 */
void set_plan_data(string plan, mixed data) {
   int pos;

   pos = member_array(plan, _plans);
   if (pos != -1) {
      ((class plan_info)_plans[pos + PLAN_INFO])->data = data;
   }
} /* set_plan_data() */

/** @ignore yes */
mixed *stats() {
   mixed *ret;
   string goal;
   class goal_information frog;
   int goal_num;
   int i;

   goal_num = 0;
   ret = ({ });
   foreach (goal in keys(_goals)) {
      frog = query_goal_information(goal);
      ret += ({ ({ "Goal " + goal_num + ") name", goal }) });
      ret += ({ ({ "Goal " + goal_num + ") events",
                   query_multiple_short(frog->events) }) });
      ret += ({ ({ "Goal " + goal_num + ") priority",  frog->priority }) });
      goal_num++;
   }

   for (i = 0; i < sizeof(_plans); i += PLAN_ARRAY_SIZE) {
      ret += ({ ({ "Plan " + goal_num + ") name", _plans[i + PLAN_PLAN] }) });
      ret += ({ ({ "Plan " + goal_num + ") priority",
                   ((class plan_info)_plans[i + PLAN_INFO])->priority }) });
   }

   return ret;
} /* stats() */

/**
 * This method sets the default data associated with the specified plan.
 * This should be things like global plan things, the level at which the
 * npc will run away.  Etc.
 * @param goal the goal to check the data for
 * @param plan the name of the plan
 * @param data the data associated with the plan
 * @see query_default_plan_data()
 */
void set_goal_plan_data(string goal, string plan, mixed data) {
   mixed goal_data;

   goal_data = query_goal_data(goal);
   goal->set_plan_data(this_object(), goal_data, plan, data);
} /* set_default_plan_data() */

/**
 * This method returns the data associated with the specified plan.
 * @param goal the foal to query the information for
 * @param plan the name of the plan to get the data for
 * @return the plans data
 * @see set_default_plan_data()
 */
mixed query_goal_plan_data(string goal, string plan) {
   mixed goal_data;

   goal_data = query_goal_data(goal);
   return goal->query_plan_data(this_object(), goal_data, plan);
} /* query_default_plan_data() */
