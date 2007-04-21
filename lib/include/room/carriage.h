/**
 * This is the include file for the carriages.
 * @author Pinkfish
 * @started Thu Aug 15 17:29:16 PDT 2002
 */
#ifndef __CARRIAGE_H__
/** @ignore yes */
#define __CARRIAGE_H__

/**
 * The class for the stop details.
 * @member name the name of the stop
 * @member path the path to the stop
 * @member delay how long to tarry at the stop
 */
class stop_details {
   string name;
   string path;
   int delay;
}

/**
 * This is the details of each stop.
 * @member id the id of the stop
 * @member travel_time how long it takes to get here from the last stop
 */
class stop_list_details {
   string id;
   int travel_time;
}

/**
 * The specific details of the route.
 * @member colour the colour of the carriage
 * @member return_along the carriage returns along the same route
 * @member stops the details of exactly where to stop and in what order
 */
class route_details {
   string colour;
   int return_along;
   class stop_list_details* stops;
}

/** The path of the handler inherit */
#define CARRIAGE_HANDLER_INHERIT "/std/room/inherit/carriage/handler"
/** The path of the carriage inherit */
#define CARRIAGE_CARRIAGE_INHERIT "/std/room/inherit/carriage/carriage"
/** The path of the default carriage pole */
#define CARRIAGE_CARRIAGE_POLE "/std/room/inherit/carriage/carriage_pole"

#endif
