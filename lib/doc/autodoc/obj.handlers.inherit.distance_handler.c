.DT
distance_handler.c
Disk World autodoc help
distance_handler.c

.SH Description
.SP 5 5

This file handles the distance checking stuff for figuring out how
far apart two rooms or co-ordinates are.
.EP
.SP 10 5


Written by Rodion

Started Wed Jul 25 15:22:24 PDT 2001

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_distance_from_coords%^RESET%^
.EI
.SI 5
int query_distance_from_coords(int * co_ord_one, int * co_ord_two)
.EI
.SP 7 5

This method figures out the distance between two co-ordinates.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
co_ord_one - the first co-ordinate
.EP
.SP 9 5
co_ord_two - the second co-ordinate
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the distance

.EP

.SI 3
* %^BOLD%^query_distance_from_rooms%^RESET%^
.EI
.SI 5
int query_distance_from_rooms(mixed one, mixed two)
.EI
.SP 7 5

This method figures out the distance between two rooms and returns the
result.  The result will be in co-ordinate units.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
one - the first room
.EP
.SP 9 5
two - the second room
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the distance between them

.EP


