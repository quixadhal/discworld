.DT
auto_load.c
Disk World autodoc help
auto_load.c

.SH Description
.SP 5 5

Handles the automatic loading and saving of objects into the
players inventories on log in/out.
.EP
.SP 10 5


Written by Pinkfish
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/virtual.h, /include/player.h, /include/move_failures.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^create_auto_load%^RESET%^
.EI
.SI 5
string * create_auto_load(object * obs, int into_array)
.EI
.SP 7 5

Creates the complete auto load array from the array of objects
passed into this object.  It returns an array of elements 
as specified in auto_str_ob.  The arrays from auto_str_ob() are
added together, so every 3 elemnts in the array is a new
autoload object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects to create an autoload string for
.EP
.SP 9 5
into_array - place the results directly iunto the auto_load array
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the auto load array
.EP

.SI 3
* %^BOLD%^load_auto_load_alt%^RESET%^
.EI
.SI 5
void load_auto_load_alt(mixed * auto_string, object dest, object tell_pl, function finished)
.EI
.SP 7 5

The new method of handling auto loading of objects.  This determines
if the object failed to load and gives the player a recipt if it does
not.


The finished variable should %^BOLD%^only%^RESET%^be set in outer level calls to this. If you set it inside containers when they call this it will cause errors. This will only be called from within the player object itself.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
auto_string - the values specifing the objects to auto load
.EP
.SP 9 5
dest - the destination to put the objects in
.EP
.SP 9 5
tell - who to tell about errors
.EP
.SP 9 5
finished - the function to call when the auto loading has completed
.EP

.SI 3
* %^BOLD%^load_auto_load_to_array%^RESET%^
.EI
.SI 5
object * load_auto_load_to_array(mixed * auto_string, object tell_pl)
.EI
.SP 7 5

This method creates all the objects and sets them up now.  This will
not move any of the objects anywhere...  Relying on the calling
code to handle this.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
auto_string - the values specifing the objects to auto load
.EP
.SP 9 5
dest - the destination to put the objects in
.EP
.SP 9 5
tell - who to tell about errors
.EP
.SP 9 5
finished - the function to call when the auto loading has completed
.EP

.SI 3
* %^BOLD%^load_auto_load_to_inventory%^RESET%^
.EI
.SI 5
void load_auto_load_to_inventory(mixed * auto_string, object dest, object tell_pl, function move_f)
.EI
.SP 7 5

This method loads all the objects from the array and places them
into the inventory of the specified object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
auto_string - the values specifing the objects to auto load
.EP
.SP 9 5
dest - the destination to put the objects in
.EP
.SP 9 5
tell - who to tell about errors
.EP
.SP 9 5
move_f - the function to call to move the object to it's destination
.EP

.SI 3
* %^BOLD%^query_auto_loading%^RESET%^
.EI
.SI 5
int query_auto_loading()
.EI
.SP 7 5

This method tells us if the player is in the inventory regeneration
phase still.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the inventory is being regenerated, 0 otherwise.

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^query_auto_load_string%^RESET%^
.EI
.SI 5
mixed query_auto_load_string()
.EI
.SP 7 5

This method retrusn the auto load string which is used to
regenerate the players inventory.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of the auto load string
.EP

.SI 3
* %^BOLD%^set_auto_load_string%^RESET%^
.EI
.SI 5
void set_auto_load_string(mixed str)
.EI
.SP 7 5

This method sets the current auto load string to the specified value.
.EP


