.DT
carriage.h
Disk World autodoc help
carriage.h

.SH Description
.SP 5 5

This is the include file for the carriages.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Aug 15 17:29:16 PDT 2002

.EP

.SH Classes

.SI 3
* %^BOLD%^route_details%^RESET%^
class route_details {
string colour;
int return_along;
class stop_list_details * stops;
}

.EI
.SP 7 5

The specific details of the route.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
colour - the colour of the carriage
.EP
.SP 9 5
return_along - the carriage returns along the same route
.EP
.SP 9 5
stops - the details of exactly where to stop and in what order

.EP

.SI 3
* %^BOLD%^stop_details%^RESET%^
class stop_details {
string name;
string path;
int delay;
}

.EI
.SP 7 5

The class for the stop details.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
name - the name of the stop
.EP
.SP 9 5
path - the path to the stop
.EP
.SP 9 5
delay - how long to tarry at the stop

.EP

.SI 3
* %^BOLD%^stop_list_details%^RESET%^
class stop_list_details {
string id;
int travel_time;
}

.EI
.SP 7 5

This is the details of each stop.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
id - the id of the stop
.EP
.SP 9 5
travel_time - how long it takes to get here from the last stop

.EP


.SH Defines
.SI 3
* %^BOLD%^CARRIAGE_CARRIAGE_INHERIT%^RESET%^
.EI
.SP 7 5
The path of the carriage inherit 
.EP

.SI 3
* %^BOLD%^CARRIAGE_CARRIAGE_POLE%^RESET%^
.EI
.SP 7 5
The path of the default carriage pole 
.EP

.SI 3
* %^BOLD%^CARRIAGE_HANDLER_INHERIT%^RESET%^
.EI
.SP 7 5
The path of the handler inherit 
.EP

