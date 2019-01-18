.DT
room_rental_office.c
Disk World autodoc help
room_rental_office.c

.SH Description
.SP 5 5

A Room Rental Office inheritable

by goldenthread started 24 Sep y2k1


.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h, /include/room/room_rental.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_room%^RESET%^
.EI
.SI 5
int add_room(string room, int cost)
.EI
.SP 7 5

This method adds a room into the offices list of rooms to rent.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to add
.EP
.SP 9 5
cost - how much the room costs to rent
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^clear_rooms%^RESET%^
.EI
.SI 5
int clear_rooms()
.EI
.SP 7 5

This method removes all the rooms from the offices list of rooms to rent.
Use this function with caution.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^delete_room%^RESET%^
.EI
.SI 5
int delete_room(string room)
.EI
.SP 7 5

This method deletes a room from the offices list of rooms to rent.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to delete
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_list%^RESET%^
.EI
.SI 5
int do_list()
.EI
.SP 7 5

The command to print the list of rooms available to rent.

.EP

.SI 3
* %^BOLD%^do_rental%^RESET%^
.EI
.SI 5
int do_rental(string room)
.EI
.SP 7 5

This method attempts to rent a room.  It checks the return from the
handler and prints the appropriate message.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room to try and rent
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^query_domain%^RESET%^
.EI
.SI 5
string query_domain()
.EI
.SP 7 5

Queries the domain of the current office.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the domain of the office.

.EP

.SI 3
* %^BOLD%^query_list%^RESET%^
.EI
.SI 5
string query_list()
.EI
.SP 7 5

This method returns the list of all the rooms available from this
office.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string list of rooms

.EP

.SI 3
* %^BOLD%^query_location%^RESET%^
.EI
.SI 5
string query_location()
.EI
.SP 7 5

Queries the location of the current office.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the location of the office.

.EP

.SI 3
* %^BOLD%^set_domain%^RESET%^
.EI
.SI 5
void set_domain(string dom)
.EI
.SP 7 5

Sets the domain of the current office.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
dom - the domain

.EP

.SI 3
* %^BOLD%^set_location%^RESET%^
.EI
.SI 5
void set_location(string loc)
.EI
.SP 7 5

Sets the location of the current office.
This is a name to identify the "hotel" or
local group of rooms.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
loc - the location

.EP


