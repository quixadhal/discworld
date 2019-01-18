.DT
parcel.c
Disk World autodoc help
parcel.c

.SH Description
.SP 5 5


Post Office Delivery Handler
.EP
.SP 10 5


Written by Terano

Started 26 December, 1998


.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /global/auto_load.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^parcel%^RESET%^
class parcel {
string from;
string * autoloadinfo;
int exp_time;
}

.EI
.SP 7 5

Parcels is a mapping with player names as keys, and arrays of class parcel
as values

.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^collect%^RESET%^
.EI
.SI 5
int collect(string name, string location, object target, int copy)
.EI
.SP 7 5

This function collects any items available
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - Name of the player collecting
.EP
.SP 9 5
location - Location collecting from
.EP
.SP 9 5
target - An object to move the items into, a room or container
.EP
.SP 9 5
copy - An int, if its 1 it doesnt delete the objects from the parcels
       array
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if no items or failure, 1 if successful

.EP

.SI 3
* %^BOLD%^deposit%^RESET%^
.EI
.SI 5
int deposit(string to, string from, string location, object * obs)
.EI
.SP 7 5

This function deposits some items for a player
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
to - The name of the player items are going to
.EP
.SP 9 5
from - The name of the player sending the items
.EP
.SP 9 5
location - The place the items are being sent from
.EP
.SP 9 5
obs - An array of the objects being sent
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it worked, or 0 if it screwed up :)

.EP

.SI 3
* %^BOLD%^do_migrate%^RESET%^
.EI
.SI 5
void do_migrate(int letter, string location)
.EI
.SP 7 5

Migrate packages
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
letter - to expire
.EP
.SP 9 5
location - to expire

.EP

.SI 3
* %^BOLD%^player_refreshed%^RESET%^
.EI
.SI 5
void player_refreshed(mixed name, int total)
.EI
.SP 7 5

Called when a player deletes
Deletes all their parcels
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
string - name
.EP
.SP 9 5
int - Total Flag

.EP

.SI 3
* %^BOLD%^queryparcels_for%^RESET%^
.EI
.SI 5
class parcel * queryparcels_for(string name, string location)
.EI
.SP 7 5

Queries if any parcels are at a location for a person
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - of the person
.EP
.SP 9 5
location - of the post office

.EP


