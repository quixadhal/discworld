.DT
route_handler.c
Disk World autodoc help
route_handler.c

.SH Description
.SP 5 5

Figures out the route between two destinations for an npc to travel.
.EP
.SP 10 5


Written by Wodan

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^get_route%^RESET%^
.EI
.SI 5
varargs void get_route(string end, string start, function call_back, int count)
.EI
.SP 7 5

Try and figure out a route between two locations.  It figures out
the route bwteen the two locations and then calls the
call_back function with the found route, or not found route.
If the route is not found, an empty array will be returend.  The
function in the npc object move_me_to() handles the call
to this function transparently.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
end - the end location
.EP
.SP 9 5
start - where to start from
.EP
.SP 9 5
call_back - the function to call when finished
.EP


