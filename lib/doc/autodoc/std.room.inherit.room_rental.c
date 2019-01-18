.DT
room_rental.c
Disk World autodoc help
room_rental.c

.SH Description
.SP 5 5

A Room Rental room inheritable

.EP
.SP 10 5


Written by goldenthread

Started 1 Oct y2k1


.EP

.SH Includes
.SP 5 5
This class includes the following files /include/room/room_rental.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^evict_tenant%^RESET%^
.EI
.SI 5
void evict_tenant(object * tenants)
.EI
.SP 7 5

This sends a message to the tenant and moves them from the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
tenant - the person being evicted

.EP

.SI 3
* %^BOLD%^query_costume_mess%^RESET%^
.EI
.SI 5
string query_costume_mess(object costume)
.EI
.SP 7 5

Returns the message given to the player when a costume item is
confiscated
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
costume - the costume object
.EP


