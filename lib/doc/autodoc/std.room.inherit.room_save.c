.DT
room_save.c
Disk World autodoc help
room_save.c

.SH Description
.SP 5 5

A saving room inheritable.  This handles rooms which save their inventory,
or more specifically part of their inventory.

Which objects should be saved or not saved can be controlled by overriding
test_save.

Containers in this room that need to cause it to save its inventory should
generate a save event.

.EP
.SP 10 5


Written by ceres

.EP



.SH See also
.SP 5 5
test_save and event_save

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /global/auto_load.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player.h, /include/move_failures.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^debug_log%^RESET%^
.EI
.SI 5
void debug_log(string fmt, mixed args ...)
.EI

.SI 3
* %^BOLD%^door_action%^RESET%^
.EI
.SI 5
void door_action()
.EI

.SI 3
* %^BOLD%^event_save%^RESET%^
.EI
.SI 5
void event_save(object thing)
.EI
.SP 7 5

Objects that need to make this room save such as containers in the room
should generate a save event to make the room save its inventory.
eg. event(environment(this_object()), "save");

.EP

.SI 3
* %^BOLD%^query_save_file%^RESET%^
.EI
.SI 5
string query_save_file()
.EI
.SP 7 5

This method returns the current save file for the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current save file

.EP

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
void set_save_file(string file)
.EI
.SP 7 5

Set the filename that this object should use to save its inventory to.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
file - The file.

.EP

.SI 3
* %^BOLD%^test_save%^RESET%^
.EI
.SI 5
int test_save(object ob)
.EI
.SP 7 5

This method determines if a given object should be saved or not.  When
inheriting this room you should define your own test_save function and use
it to decide which parts of the inventory are saved and which are not.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - The object to be tested.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for yes 0 for no.

.EP


