/**
 * The base inherit for all plans.
 * @author Pinkfish
 * @started Wed Aug  5 15:39:52 PDT 1998
 */
#include <npc/plan.h>

private nosave int _sutability;
private nosave mapping _hooked_events;

/** @ignore yes */
void setup() {
} /* setup() */

void create() {
   _sutability = PLAN_INVALID_SUTABILITY;
   _hooked_events = ([ ]);
   setup();
} /* create() */

/**
 * The precondition to the plan starting.
 * The default is to never let it start and always let them be finished.
 * @return always returns 0
 */
int pre_condition(object npc,
		  string goal,
                  mixed goal_info,
		  string event,
		  mixed *data) {
   return 0;
} /* pre_condition() */

/**
 * This method determines if the plan has been finished.
 * @return always returns 1
 */
int finished(object npc,
             mixed *data) {
   return 1;
} /* finished() */

/**
 * This method should return a number saying how sutiable this plan is
 * for this operation.,
 * @return the sutability level, always invalid
 * @see set_sutability()
 */
int query_sutability(object npc,
		     string goal,
                     mixed goal_info,
		     string event,
		     mixed *data) {
   return _sutability;
} /* query_sutability() */

/**
 * This method sets the sutability of the plan.  This is the default
 * sutability level, if you wish to make the sutability more complicate than
 * a simple number then the query_sutability method should be overridden.
 * @param sutability the new sutability
 * @see query_sutability()
 */
void set_sutability(int sutability) {
   _sutability = sutability;
} /* set_sutability() */

/**
 * This method should return any extra data needed to be passed onto the
 * plan.
 * @return the needed data, always 0
 */
int query_data(object npc,
	       string goal,
               mixed goal_info,
	       string event,
	       mixed *data) {
   return 0;
} /* query_data() */

/**
 * This method creates the plan control information, if any is needed.
 * If this is not setup with the plan then the default values will be
 * used.
 * @return the plan data
 */
mixed create_plan_data() {
   return 0;
} /* create_plan_data() */

/**
 * This method sets up a hooked event, the function will be called if the
 * event is called.  Thus things like completion events and followon stuff
 * can be done.  The function will be called with 4 arguments, the 
 * arguments are:<br>
 * func(npc, plan_data, event, data)
 * @param event the event to call
 * @param func the function to call with the event
 */
void add_hooked_event(string event, function func) {
   _hooked_events[event] = func;
} /* add_hooked_event() */

/**
 * This method removes a hooked event.
 * @param event the event to remove the data for
 */
void remove_hooked_event(string event) {
   map_delete(_hooked_events, event);
} /* remove_hooked_event() */

/**
 * This method will be called when an event is setup.
 * @param npc the npc that called the event
 * @param plan_data the data associated with the plan
 * @param event the event which is called
 * @param data the data associated with the event
 */
int notify_event(object npc,
                 mixed plan_data,
                 string event,
                 mixed *data) {
   if (_hooked_events[event]) {
      return evaluate(_hooked_events[event], npc, plan_data, event, data);
   }
   return 0;
} /* notify_event() */
