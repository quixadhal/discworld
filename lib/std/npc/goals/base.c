/**
 * The standard base for all the npc goals.  This should be inherited into
 * every goal.  The methods can be overridden is specific behaviour of some
 * sort is required.
 * @author Pinkfish
 * @started Thu Jul 30 18:17:35 PDT 1998
 */

#include <npc/goals.h>

private mapping _default_plans;
private int _default_priority;

#define GOAL_PLAN_ARRAY_SIZE 1
#define GOAL_PLAN_PLAN 0

/** @ignore yes */
void setup() {
} /* setup() */

void create() {
   _default_plans = ([ ]);
   _default_priority = GOAL_INVALID_PRIORITY;
   setup();
} /* create() */

/**
 * This method initialises all the plans associated with this goal.  It
 * should be overridden by the higher level objects to allow them to
 * initialise the plans and stuff.
 * @param npc the npc the goal is on
 * @param goal_name the name of the goal
 * @param control the control data
 */
void initialise_plans(object npc,
                      string goal_name,
                      mixed data) {
} /* initialise_plans() */

/**
 * This method initialises the goal.  It sets up all the call back
 * events and anything that needs to be done at startup.  This method will
 * also call the initialise_plan() method to setup all the default
 * plan information for this goal.
 * @param npc the npc to initialise
 * @param goal_name the name of the goal
 * @param control the control data
 */
void initialise(object npc, 
                string goal_name,
                class goal_control control) {
   string name;

   foreach (name in keys(_default_plans)) {
tell_creator("pinkfish", "Adding event hook for %O\n", name);
      npc->add_goal_event(base_name(this_object()), name);
   }

   foreach (name in keys(control->extra_plans)) {
      npc->add_goal_event(base_name(this_object()), name);
   }

   initialise_plans(npc, goal_name, control->data);
} /* initialise() */

/**
 * This method returns the current plan associated with the goal control
 * for this goal.
 * @param npc the npc the goal is operating on
 * @param control the goal control class
 */
string query_current_plan(object npc, 
                          class goal_control control) {
   return control->current_plan;
} /* query_current_plan() */

/**
 * This method is used to create the plan specific data.
 * @param npc the npc that the goal is being setup on
 * @param goal_name the noralised name of the goal
 * @param stuff the information passed in when the goal was created
 * @return the data used by the goal
 */
mixed create_goal_data(object npc,
                       string goal_name,
                       mixed stuff) {
   return stuff;
} /* create_goal_data() */

/**
 * This method creats a goal control class with any of the extra bits that are
 * required.  The extra plans are a mapping which is indexed on the event
 * name and contains values which are the plans to use for that goal.
 * @param npc the npc creating the goal
 * @param extra_plans the extra plans needed for this npc
 * @param stuff the startup goal data
 * @return the class to use as an initial argument to the goal
 */
void add_goal_to_npc(object npc,
                     mapping extra_plans,
		     mixed stuff) {
   class goal_control frog;
   mixed value;
   string goal_name;

tell_creator("pinkfish", "Adding goal %O to npc(%O).\n", base_name(this_object()), stuff);
   if (!mapp(extra_plans)) {
      extra_plans = ([ ]);
   }
   goal_name = base_name(this_object());
   frog = new(class goal_control);
   frog->extra_plans = extra_plans;
   frog->current_plan = 0;
   frog->priority = GOAL_DEFAULT_PRIORITY;
   frog->data = create_goal_data(npc, goal_name, stuff);
   frog->plan_info = ([ ]);
   npc->add_goal(goal_name, frog);
} /* add_goal_to_npc() */

/**
 * This method returns the current priority for the goal.
 * @param npc the npc the goal is linked to
 * @param control the control information for the goal
 * @return the priority of the goal
 */
int query_priority(object npc,
                   class goal_control control) {
   if (control->priority != GOAL_DEFAULT_PRIORITY) {
      return control->priority;
   }
   return _default_priority;
} /* query_priority() */

/**
 * This method returns the default priority for the goal.
 * @return the default priority
 */
int query_default_priority() {
   return _default_priority;
} /* query_default_priority() */

/**
 * This method sets the default priority for the goal.
 * @param priority the new default priority
 */
protected void set_default_priority(int priority) {
   _default_priority = priority;
} /* set_default_priority() */

/**
 * This method handles an incoming event and will see which of the
 * possible plans might need to be setup to run.
 * @param event_name the name of the event
 * @return notification of the event handling
 */
int notify_of_event(object npc,
                    class goal_control control,
                    string event,
                    mixed *data) {
   int value;
   string plan;
   int max_value;
   string max_plan;
   mixed max_data;
   int i;
   mixed info;

   /* Check the extra known plans for sutability. */
   if (control->extra_plans[event]) {
      for (i = 0; i < sizeof(control->extra_plans[event]);
           i += GOAL_PLAN_ARRAY_SIZE) {
         plan = control->extra_plans[event][i + GOAL_PLAN_PLAN];
         info = control->plan_info[plan];
         if (plan->pre_condition(npc,
                                 base_name(this_object()),
                                 info,
                                 event,
                                 data)) {
            value = plan->query_sutability(npc,
                                           base_name(this_object()),
                                           info,
                                           event,
                                           data);
            if (value > max_value) {
               max_value = value;
               max_plan = plan;
               max_data = plan->query_data(npc,
                                           base_name(this_object()),
                                           info,
                                           event,
                                           data);
            }
         }
      }
   }

   /* Check the default plans for sutability. */
   if (_default_plans[event]) {
      for (i = 0; 
           i < sizeof(_default_plans[event]);
           i += GOAL_PLAN_ARRAY_SIZE) {
         plan = _default_plans[event][i + GOAL_PLAN_PLAN];
         info = control->plan_info[plan];
         if (plan->pre_condition(npc,
                                 base_name(this_object()),
                                 info,
                                 event,
                                 data)) {
            value = plan->query_sutability(npc,
                                           base_name(this_object()),
                                           info,
                                           event,
                                           data);
            if (value > max_value) {
               max_value = value;
               max_plan = plan;
               max_data = plan->query_data(npc,
                                           base_name(this_object()),
                                           info,
                                           event,
                                           data);
            }
         }
      }
   }

   /* If we found a plan to activate, activate it. */
   if (max_plan) {
      // Allow plans to modify their priority up and down a bit.
      return npc->activate_plan(max_plan,
                                base_name(this_object()),
                                query_priority(npc, control) + 
                                   max_plan->query_priority_offset(),
                                max_data);
   }
   return GOAL_EVENT_NOT_HANDLED;
} /* notify_of_event() */

/**
 * This method returns the known set of default plans for the goal.
 * @return the default known plans
 */
mapping query_default_plans() {
   return _default_plans;
} /* query_default_plans() */

/**
 * This method adds a default plan to the currently known list of plans.
 * @param event_name the name of the event to trigger the plan on
 * @param plan the name of the plan to add
 * @param activate the function pointer to create the data for the plan
 */
protected void add_default_plan(string event_name,
                                string plan) {
   if (!_default_plans[event_name]) {
      _default_plans[event_name] = ({ plan });
   } else if (member_array(plan, _default_plans[event_name]) == -1) {
      _default_plans[event_name] += ({ plan });
   }
} /* add_default_plan() */

/**
 * This method changes the data associated with the plan for the particular
 * npc and goal.
 * @param npc the npc that the goal is on
 * @param control the data associated with the goal
 * @param plan the name of the plan to setup
 * @param plan_data the data to set for the plan
 */
void set_plan_data(object npc,
                   class goal_control control,
                   string plan,
                   mixed plan_data) {
   control->plan_info[plan] = plan_data;
} /* set_plan_data() */

/**
 * This method returns the data associated withthe plan for the particular
 * npc and goal.
 * @param npc the npc the goal is on
 * @param control the control data for the goal
 * @param plan the name of the plan to setup
 * @param plan_data the data to set for the plan
 */
mixed query_plan_data(object npc,
                      class goal_control control,
                      string plan) {
   return control->plan_info[plan];
} /* query_plan_data() */
