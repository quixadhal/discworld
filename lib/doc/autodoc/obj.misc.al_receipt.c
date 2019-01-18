.DT
al_receipt.c
Disk World autodoc help
al_receipt.c

.SH Description
.SP 5 5

Sets the name of the object.  This will be used so the item recipt
can be identified.
.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/virtual.h, /include/move_failures.h, /include/player.h, /include/armoury.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^being_sold%^RESET%^
.EI
.SI 5
void being_sold()
.EI
.SP 7 5

When the item is sold, kill ourselves.

.EP

.SI 3
* %^BOLD%^move%^RESET%^
.EI
.SI 5
varargs int move(mixed dest, string messin, string messout)
.EI

.SI 3
* %^BOLD%^query_missing_item_receipt%^RESET%^
.EI
.SI 5
int query_missing_item_receipt()
.EI
.SP 7 5

This will return 1 if the item is a missing item receipt.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the item is a missing item receipt

.EP

.SI 3
* %^BOLD%^query_obname%^RESET%^
.EI
.SI 5
string query_obname()
.EI
.SP 7 5

This method returns the name of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the object

.EP

.SI 3
* %^BOLD%^query_obpath%^RESET%^
.EI
.SI 5
string query_obpath()
.EI
.SP 7 5

This method returns the path of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the path of the object

.EP

.SI 3
* %^BOLD%^query_static_save%^RESET%^
.EI
.SI 5
mixed query_static_save()
.EI
.SP 7 5

This method returns the save information for the object.  THis is not
just the static save information, it is an array of two elements.  The
first is the static save information and the second is the dynamic
save information.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the save information

.EP

.SI 3
* %^BOLD%^query_timestamp%^RESET%^
.EI
.SI 5
int query_timestamp()
.EI
.SP 7 5

This method returns the timestamp of the reciept.  This is the time the
receipt was created.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the timestamp

.EP

.SI 3
* %^BOLD%^query_virt_obname%^RESET%^
.EI
.SI 5
string query_virt_obname()
.EI
.SP 7 5

This method returns the virtual object path name of the item.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the virtual object name

.EP

.SI 3
* %^BOLD%^set_object%^RESET%^
.EI
.SI 5
void set_object(string s)
.EI
.SP 7 5

Sets the path to the object.  The path of the object for the recipt to
work with.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the object path

.EP

.SI 3
* %^BOLD%^set_obname%^RESET%^
.EI
.SI 5
void set_obname(string s)
.EI

.SI 3
* %^BOLD%^set_obpath%^RESET%^
.EI
.SI 5
void set_obpath(string s)
.EI
.SP 7 5

This method is identical to set_object()
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the object path

.EP

.SI 3
* %^BOLD%^set_static_save%^RESET%^
.EI
.SI 5
void set_static_save(mixed m)
.EI
.SP 7 5

This method sets the save information for the file.  It actually sets
both the static and dynamic information.  This assumes it is passed
an array of two elements, the first is the static save information and
the second is the dynamic save information.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
m - the data as defined above

.EP

.SI 3
* %^BOLD%^set_virtname%^RESET%^
.EI
.SI 5
void set_virtname(string s)
.EI
.SP 7 5

This method is identical to set_virtobname()
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the virtual object name

.EP

.SI 3
* %^BOLD%^set_virtobname%^RESET%^
.EI
.SI 5
void set_virtobname(string s)
.EI
.SP 7 5

This method sets the virtual object name of the item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the virtual object name

.EP

.SI 3
* %^BOLD%^setup_receipt%^RESET%^
.EI
.SI 5
void setup_receipt(object ob)
.EI
.SP 7 5

This method sets up the receipt for the specified object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to setup the reciept for

.EP


