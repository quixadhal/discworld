.DT
room_rental.c
Disk World autodoc help
room_rental.c

.SH Description
.SP 5 5

A Room Rental Handler


This keeps track of who rented what rooms where and for how much. 

After including <room/room_rental.h> there are two options available for room inherits. ROOM_RENTAL_INHERIT_BASIC_ROOM includes the basic room inherit while ROOM_RENTAL_INHERIT_ROOM is for use in areas that have their own base room inherits. 

The most important function is the one that checks that someone is allowed to enter. 
.EP
.SP 5 5
int check_entry( object player, object room ) 
.EP
.SP 5 5
it returns: RENTAL_SUCCESS if the player rents the room successfully RENTAL_NO_SUCH_ROOM if the room isn't on the books RENTAL_ALREADY_RENTED if the room is rented to someone else RENTAL_AVAILABLE if the player doesn't rent the room but it is available 
.EP
.SP 10 5


Written by goldenthread

Started 3 Sep y2k1


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
int add_room(string room, int cost, object office)
.EI
.SP 7 5

Adds a room to the handler with a rental cost.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the path of the room to add to the handler
.EP
.SP 9 5
cost - the rental cost per day of this room
.EP
.SP 9 5
office - the current office

.EP

.SI 3
* %^BOLD%^check_tenant%^RESET%^
.EI
.SI 5
int check_tenant(object player_ob, object room_ob, object office)
.EI
.SP 7 5

Checks if a particular player is renting a particular room.
 returns:
RENTAL_SUCCESS if the player rents the room successfully
RENTAL_NO_SUCH_ROOM if the room isn't on the books
RENTAL_ALREADY_RENTED if the room is rented to someone else
RENTAL_AVAILABLE if the player doesn't rent the room but it is available

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player_ob - the player object who you think might be renting the room
.EP
.SP 9 5
room_ob -   the room object that you think the player might be renting

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
value dependent on success of check.  See below for details

.EP

.SI 3
* %^BOLD%^clear_rooms%^RESET%^
.EI
.SI 5
int clear_rooms(object office)
.EI
.SP 7 5

Clears the list of rooms available for rent.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
office - the current office

WARNING: This clears all rooms bookable from this office.

.EP

.SI 3
* %^BOLD%^delete_room%^RESET%^
.EI
.SI 5
int delete_room(string room, object office)
.EI
.SP 7 5

Deletes a room from the handler

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the path of the room that will be deleted from the handler
.EP
.SP 9 5
office - the current office

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on success, 1 on failure

.EP

.SI 3
* %^BOLD%^do_rental%^RESET%^
.EI
.SI 5
int do_rental(object player_ob, string room_short, object office)
.EI
.SP 7 5

Handles the actual renting of the room.  If the prospective
tenant is already renting the room, it extends the rental.
 returns:
RENTAL_SUCCESSFUL if the rental is successful
RENTAL_NO_SUCH_ROOM if the room isn't on the books
RENTAL_ALREADY_RENTED if someone else is renting the room already
RENTAL_NO_MONEY if the player hasn't enough money

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player_ob - the player who is renting the room
.EP
.SP 9 5
room_short - the short of the room that is to be rented
.EP
.SP 9 5
office - the current office

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
value dependent on success of rental. See below for details

.EP

.SI 3
* %^BOLD%^evict_tenant%^RESET%^
.EI
.SI 5
void evict_tenant(object room_ob)
.EI
.SP 7 5

Evicts the tenant ( and guests ).

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room_ob - the room object

.EP

.SI 3
* %^BOLD%^query_rental_value%^RESET%^
.EI
.SI 5
int query_rental_value(string room)
.EI
.SP 7 5

Returns the amount needed to pay for a rental

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the room being queried
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
rental amount

.EP

.SI 3
* %^BOLD%^query_rooms_to_rent%^RESET%^
.EI
.SI 5
string * query_rooms_to_rent(object office)
.EI
.SP 7 5

Returns a list of rooms available in a particular location.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
office - the office

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string ready for presentation to a player containing a
list of all rooms available in that office.

.EP

.SI 3
* %^BOLD%^query_who_rents%^RESET%^
.EI
.SI 5
string query_who_rents(string room, object office)
.EI
.SP 7 5

Returns the name of whoever is renting the room specified.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room - the short of the room you want to check
.EP
.SP 9 5
office - the current office

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the short of the player who's renting the room

.EP

.SI 3
* %^BOLD%^remove_rental%^RESET%^
.EI
.SI 5
int remove_rental(object room_ob)
.EI
.SP 7 5

Removes the rented status from a room.  Should be called when the
time limit of the rental is over.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
room_ob - the room object that is currently rented

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on success, 1 on failure

.EP


