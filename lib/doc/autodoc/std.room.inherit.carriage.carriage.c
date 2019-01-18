.DT
carriage.c
Disk World autodoc help
carriage.c

.SH Description
.SP 5 5

This method should be overriden by higher level carriages to setup
the driver correctly.
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/room/carriage.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_can_leave%^RESET%^
.EI
.SI 5
int check_can_leave(string exit, object person)
.EI

.SI 3
* %^BOLD%^co%^RESET%^
.EI
.SI 5
mixed * co()
.EI

.SI 3
* %^BOLD%^create_carriage_driver%^RESET%^
.EI
.SI 5
object create_carriage_driver()
.EI

.SI 3
* %^BOLD%^do_ring%^RESET%^
.EI
.SI 5
int do_ring()
.EI

.SI 3
* %^BOLD%^give_warning%^RESET%^
.EI
.SI 5
void give_warning(string this_stop_name, string next_stop_name)
.EI

.SI 3
* %^BOLD%^make_carriage%^RESET%^
.EI
.SI 5
int make_carriage(string name, class route_details new_route_info, object handler)
.EI
.SP 7 5

This is called by the handler to originaly setup the current route
information.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the rotue
.EP
.SP 9 5
new_route_info - the new route to use
.EP
.SP 9 5
handler - the handler for the carriage

.EP

.SI 3
* %^BOLD%^make_stop%^RESET%^
.EI
.SI 5
void make_stop()
.EI
.SP 7 5

Stop at the next location.

.EP

.SI 3
* %^BOLD%^move_on%^RESET%^
.EI
.SI 5
void move_on()
.EI

.SI 3
* %^BOLD%^query_carriage_name%^RESET%^
.EI
.SI 5
string query_carriage_name()
.EI
.SP 7 5

This method returns the name of the route the carriage is on.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the carriage route.

.EP

.SI 3
* %^BOLD%^query_current_stop%^RESET%^
.EI
.SI 5
class stop_list_details query_current_stop()
.EI
.SP 7 5

This method returns the details of the current stop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the details of the current stop

.EP

.SI 3
* %^BOLD%^query_environ_long%^RESET%^
.EI
.SI 5
string query_environ_long()
.EI

.SI 3
* %^BOLD%^query_next_stop%^RESET%^
.EI
.SI 5
class stop_list_details query_next_stop()
.EI
.SP 7 5

This method returns the details of the next stop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the details of the next stop

.EP

.SI 3
* %^BOLD%^query_outside%^RESET%^
.EI
.SI 5
object query_outside()
.EI
.SP 7 5

This method returns the object that is the outside of us.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the environment of the carriage

.EP

.SI 3
* %^BOLD%^query_route_map%^RESET%^
.EI
.SI 5
class route_details query_route_map()
.EI
.SP 7 5

This method returns the entire details of the route used by this
carriage.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the entire route details

.EP

.SI 3
* %^BOLD%^query_stop_info%^RESET%^
.EI
.SI 5
int * query_stop_info()
.EI
.SP 7 5

This method returns the indexes of the current stop and the
next stop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
({ current stop, next stop })

.EP

.SI 3
* %^BOLD%^update_route%^RESET%^
.EI
.SI 5
void update_route(string name, class route_details new_route_info, object handler)
.EI
.SP 7 5

This is called by the handler to update the current route information
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the rotue
.EP
.SP 9 5
new_route_info - the new route to use
.EP
.SP 9 5
handler - the handler for the carriage

.EP


