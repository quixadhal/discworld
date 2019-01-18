.DT
laundry_entry_inherit.c
Disk World autodoc help
laundry_entry_inherit.c

.SH Description
.SP 5 5

 Inherit  : CWC Laundry Entry Inherit 
 Author   : Siel
 Date     : January 2001


.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h, /include/vault.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_customer%^RESET%^
.EI
.SI 5
int check_customer(string verb, object thing, string special)
.EI

.SI 3
* %^BOLD%^set_laundry_exit%^RESET%^
.EI
.SI 5
void set_laundry_exit(string direc, mixed dest, string type)
.EI
.SP 7 5

Used to set the exit from the laundry entrance room into the laundry 
itself.
The parameters are just like those of add_exit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
direction - The direction of the exit.
.EP
.SP 9 5
dest - The destination object. Usually the filename of the destination.
.EP
.SP 9 5
type - An exit type as in add_exit.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
void
.EP

.SI 3
* %^BOLD%^set_laundryowner%^RESET%^
.EI
.SI 5
void set_laundryowner(object ob)
.EI
.SP 7 5

This is the object that is the owner of the laundry. This should be an
NPC.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
laundryowner - The owner NPC object.

.EP

.SI 3
* %^BOLD%^set_location%^RESET%^
.EI
.SI 5
void set_location(string str)
.EI
.SP 7 5

This function is used to set the location used for calculation of which
currency the laundry owner will accept.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
location - The name of the location.

.EP

.SI 3
* %^BOLD%^set_visit_cost%^RESET%^
.EI
.SI 5
void set_visit_cost(int i)
.EI
.SP 7 5

This is used to set the cost of a visit to the laundry (in monetary units)
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cost - The cost of a vist

.EP


