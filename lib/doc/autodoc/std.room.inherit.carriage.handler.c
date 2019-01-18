.DT
handler.c
Disk World autodoc help
handler.c

.SH Description
.SP 5 5

Pinkfish's rewrite of the carriages.  March 2002.
.EP
.SP 10 5


Written by Pinkfish

Started Mon Mar  4 23:34:31 PST 2002

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/room/carriage.h, /include/data.h and /include/room.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^load_data%^RESET%^
.EI
.SI 5
void load_data()
.EI

.SI 3
* %^BOLD%^query_carriage_on_route%^RESET%^
.EI
.SI 5
object query_carriage_on_route(string route)
.EI
.SP 7 5

This method returns the carriage object on the specified route.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
route - the route to look for the carriage on
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the carriage on the route

.EP

.SI 3
* %^BOLD%^query_route_actual%^RESET%^
.EI
.SI 5
mapping query_route_actual()
.EI
.SP 7 5

This method returns all the actual actual routes and where they are.
This should only be used for debugging purposes.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the actual route details

.EP

.SI 3
* %^BOLD%^query_route_details%^RESET%^
.EI
.SI 5
mapping query_route_details()
.EI
.SP 7 5

Query the details of the route.  This returns everything about the
details and should not be used.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the route details

.EP

.SI 3
* %^BOLD%^query_route_names%^RESET%^
.EI
.SI 5
string * query_route_names()
.EI
.SP 7 5

This method returns all the names of all the different routes in the
system.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the route names

.EP

.SI 3
* %^BOLD%^query_route_stops%^RESET%^
.EI
.SI 5
class stop_list_details * query_route_stops(string route)
.EI
.SP 7 5

This method returns the list of stops associated with the
route.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
route - the name of the route
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the stops on the route

.EP

.SI 3
* %^BOLD%^query_stop_delay%^RESET%^
.EI
.SI 5
int query_stop_delay(string name)
.EI
.SP 7 5

This method returns the delay of the stop, the actual room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the stop
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the delay of the stop

.EP

.SI 3
* %^BOLD%^query_stop_location%^RESET%^
.EI
.SI 5
string query_stop_location(string name)
.EI
.SP 7 5

This method returns the location of the stop, the actual room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the stop
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the location of the stop

.EP

.SI 3
* %^BOLD%^query_stop_name%^RESET%^
.EI
.SI 5
string query_stop_name(string name)
.EI
.SP 7 5

This method returns the name of the stop, the actual room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the stop
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the stop

.EP

.SI 3
* %^BOLD%^scan_for_new_data%^RESET%^
.EI
.SI 5
void scan_for_new_data()
.EI
.SP 7 5

This method scans through all the files and sees if anything has
changed.

.EP

.SI 3
* %^BOLD%^setup_carriage%^RESET%^
.EI
.SI 5
void setup_carriage(string dir, string base_dir, string carriage, string pole, string save_file)
.EI
.SP 7 5

This method sets up the carriage handler stuff.  It sets up the pole
and the carriage items.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dir - the directory of the data files
.EP
.SP 9 5
base_dir - the base directory to use on the front of paths
.EP
.SP 9 5
carriage - what to clone for the carriage
.EP
.SP 9 5
pole - what to clone for a pole
.EP
.SP 9 5
save_file - where to save the data

.EP


