.DT
armour.c
Disk World autodoc help
armour.c

.SH Description
.SP 5 5

This file contains all the armour related code for the living
objects.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/armour_logic.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/race.h, /include/armour.h, /include/function.h and /include/clothing.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^clear_armours%^RESET%^
.EI
.SI 5
void clear_armours()
.EI
.SP 7 5

This method will make sure all the armours are unworn by the living
object.
.EP

.SI 3
* %^BOLD%^query_ac%^RESET%^
.EI
.SI 5
varargs int query_ac(string type, int amount, string zone)
.EI
.SP 7 5

This function returns the amount of damage that can be blocked
by the armour on a specified zone.  It automatically
damages the armour, and sets the stopped object to be the
the object (piece of armour) that stopped the blow.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - The type of damage, eg: "sharp", "blunt", "pierce".
.EP
.SP 9 5
amount - The amount of damage that is being done.
.EP
.SP 9 5
zone - The zone which the damage is being done through, eg: "head".
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The amount of damage that will be blocked.
.EP

.SI 3
* %^BOLD%^query_armour_callbacks%^RESET%^
.EI
.SI 5
mixed * query_armour_callbacks()
.EI
.SP 7 5

Query all the callback data. This should typically only be used for
debugging purposes.

.EP

.SI 3
* %^BOLD%^query_armours%^RESET%^
.EI
.SI 5
object * query_armours()
.EI
.SP 7 5

This method returns all the armours that the is currently being
worn.  This will always return the same value as query_wearing()
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of armours

.EP

.SI 3
* %^BOLD%^query_skin%^RESET%^
.EI
.SI 5
string query_skin()
.EI
.SP 7 5

This method returns the current skin of the living object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the skin of the object
.EP

.SI 3
* %^BOLD%^query_stopped%^RESET%^
.EI
.SI 5
mixed query_stopped()
.EI
.SP 7 5

This returns the object which stopped the blow.  This is only valid
inside and after a query_ac() call.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the object which stopped the call

.EP

.SI 3
* %^BOLD%^query_wearing%^RESET%^
.EI
.SI 5
object * query_wearing()
.EI
.SP 7 5

This method returns all the objects you are currently wearing.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current array of worn objects the array of worn stuff
.EP

.SI 3
* %^BOLD%^register_armour_callback%^RESET%^
.EI
.SI 5
int register_armour_callback(int level, mixed callback, mixed data)
.EI
.SP 7 5

Register a callback for magical protection.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
level - How close the protection is to the skin.
.EP
.SP 9 5
callback - The object & function to be called.
.EP
.SP 9 5
data - Any extra data to be passed.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an ID to identify this callback.

.EP

.SI 3
* %^BOLD%^remove_armour%^RESET%^
.EI
.SI 5
int remove_armour(object thing)
.EI
.SP 7 5

This method will remove the armour from the living object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the armour to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if was unsuccessful and 0 if it was successful
.EP

.SI 3
* %^BOLD%^remove_armour_callback%^RESET%^
.EI
.SI 5
int remove_armour_callback(int level, int id)
.EI
.SP 7 5

Remove a registered callback, used when the protection ends.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
level - The level of the callback
.EP
.SP 9 5
id - The callbacks ID.

.EP

.SI 3
* %^BOLD%^set_skin%^RESET%^
.EI
.SI 5
void set_skin(string word)
.EI
.SP 7 5

This method sets the current skin of the living object.  This can
be done by the race object and by specific spells or effects.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the new skin type
.EP

.SI 3
* %^BOLD%^set_stopped%^RESET%^
.EI
.SI 5
void set_stopped(mixed arg)
.EI
.SP 7 5

This method sets the object which stops the call.  This should be
used to do weird stuff.  I have no idea what it should be used for
at all and I am just rambling.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
arg - the new value of the stopped object

.EP

.SI 3
* %^BOLD%^update_armour_callback%^RESET%^
.EI
.SI 5
int update_armour_callback(int level, int id, mixed data)
.EI
.SP 7 5

Update the data for an armour callback.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
level - The level of the callback
.EP
.SP 9 5
id - The callbacks ID.
.EP
.SP 9 5
data - The new data.

.EP

.SI 3
* %^BOLD%^wear_armour%^RESET%^
.EI
.SI 5
string wear_armour(object armour, int doing_hold)
.EI
.SP 7 5

This is the method used to get the living object to wear a piece of
armour or clothing.  This is called from inside the armour or clothing
code itself.  It will call the function set_worn_by() on the
armour or clothing and if this returns a non-zero result then it
will add it into the current list of worn types.  Assuming the
type of the armour fits into the allowed list.  It will also
call the functon hold_item on the object if it is required to be
held as well (ie: shield).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
armour - the armour to wear
.EP
.SP 9 5
doing_hold - if this is called by the hold command
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the failure message, or 0 if ok
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^equivalent_armour_types%^RESET%^
.EI
.SI 5
int equivalent_armour_types(mixed comp, string type)
.EI
.SP 7 5

This method checks to see if the two armour types are
equivilant or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
comp - the first type
.EP
.SP 9 5
type - the second type

.EP


