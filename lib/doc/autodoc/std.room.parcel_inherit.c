.DT
parcel_inherit.c
Disk World autodoc help
parcel_inherit.c

.SH Description
.SP 5 5


Post Office Delivery Inheritable
.EP
.SP 10 5


Written by Terano

Started 28 December, 1998

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/player.h, /include/shops/bank.h, /include/mail.h, /include/playtesters.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^deposit_parcel%^RESET%^
.EI
.SI 5
int deposit_parcel(object * obs, string name, int show_sender)
.EI
.SP 7 5

This method is used by outside objects to send parcels (if they should
need to do so).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - Objects to deposit (required)
.EP
.SP 9 5
name - Name to go to (required)
.EP
.SP 9 5
show_sender - non-zero to show name of sender in automail (optional)

.EP

.SI 3
* %^BOLD%^do_collect%^RESET%^
.EI
.SI 5
int do_collect()
.EI
.SP 7 5

Collects a parcel if any are here
Mask to do interesting things
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if there are parcels, and 0 if there arent

.EP

.SI 3
* %^BOLD%^do_deposit%^RESET%^
.EI
.SI 5
varargs int do_deposit(object * obs, string name, int show_sender)
.EI
.SP 7 5

Do deposit function, mask to do interesting things
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
*obs - Objects to deposit (required)
.EP
.SP 9 5
name - Name to go to (required)
.EP
.SP 9 5
show_sender - non-zero to show name of sender in automail (optional)
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
-5 if user is not allowed to recieve parcels, -4 if sender is not
allowed to send parcels, -3 if sender and user are the same person,
-2 if no such user, -1 if not enough money, 0 if missing arguments or
failed for unknown reason and 1 if successful

.EP

.SI 3
* %^BOLD%^make_shelf%^RESET%^
.EI
.SI 5
void make_shelf(string short, string long)
.EI
.SP 7 5

Makes a shelf and moves it to the room, if there is a shelf, items
collected will be moved to it, otherwise they just get moved to the floor
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
short - The short desc of the shelf
.EP
.SP 9 5
long - The long desc of the shelf

.EP

.SI 3
* %^BOLD%^query_shelf%^RESET%^
.EI
.SI 5
object query_shelf()
.EI

.SI 3
* %^BOLD%^set_cost%^RESET%^
.EI
.SI 5
void set_cost(int _cost)
.EI
.SP 7 5

Sets the cost to send a parcel, in default money units
400 = $1AM
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
_cost - The cost

.EP

.SI 3
* %^BOLD%^set_currency%^RESET%^
.EI
.SI 5
void set_currency(string temp)
.EI
.SP 7 5

What currency to use
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
temp - The type of currency (ie "Ankh-Morpork")

.EP

.SI 3
* %^BOLD%^set_location%^RESET%^
.EI
.SI 5
void set_location(string _loc)
.EI
.SP 7 5

Sets the location of the parcel office
This is a directory in /save/parcels
ie /save/parcels/teranotest/
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
_loc - location of the post office

.EP

.SI 3
* %^BOLD%^set_shelf%^RESET%^
.EI
.SI 5
void set_shelf(object ob)
.EI


