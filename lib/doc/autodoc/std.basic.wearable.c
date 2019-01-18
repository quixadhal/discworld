.DT
wearable.c
Disk World autodoc help
wearable.c

.SH Description
.SP 5 5

This file contains all the methods needed to make an object wearable
by a player or an npc.
.EP
.SP 10 5


Written by Pinkfish
.EP



.SH See also
.SP 5 5
/obj/armour.c and /obj/clothing.c

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/condition.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h and /include/clothing.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_immune_to%^RESET%^
.EI
.SI 5
void add_immune_to(mixed args)
.EI
.SP 7 5

This adds a new type of damage that the object is immune to.
The parameter can either be a string or an array of strings
being the types of damage to be immune to.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
args - the type of damage to be immune to
.EP

.SI 3
* %^BOLD%^add_wear_effect%^RESET%^
.EI
.SI 5
void add_wear_effect(string effect)
.EI
.SP 7 5

This method adds a new wear effect to the current wear effect array.  These effects will automaticly be added when worn and
removed when taken off.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
effect - the effect to add
.EP

.SI 3
* %^BOLD%^query_immune_to%^RESET%^
.EI
.SI 5
string * query_immune_to()
.EI
.SP 7 5

This returns the list of types of damage that the object is immune to.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of damage we are immune to
.EP

.SI 3
* %^BOLD%^query_type%^RESET%^
.EI
.SI 5
string query_type()
.EI
.SP 7 5

This method returns the current type(s) associated with the object.
If this method returns a string then there is only one type for this
object.  If it returns a string then there is more than one
type associated with an object.  An example of something with more
than one type is a skirt, which is a dress and a shirt at the
same time.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current type of the item
.EP

.SI 3
* %^BOLD%^query_wear_effects%^RESET%^
.EI
.SI 5
string * query_wear_effects()
.EI
.SP 7 5

This method returns the list of effects to be added to the wearer when
it is worn.  These effects will automaticly be added when worn and
removed when taken off.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of effects to be added to the wearer when worn
.EP

.SI 3
* %^BOLD%^query_wear_remove_func%^RESET%^
.EI
.SI 5
mixed * query_wear_remove_func()
.EI
.SP 7 5

This method returns the current function associated with
wearing and removing the item.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current function for wearing and removing the clothing
.EP

.SI 3
* %^BOLD%^query_wearable%^RESET%^
.EI
.SI 5
int query_wearable()
.EI
.SP 7 5

This method tells us if the object is wearable.
In the case of a wearable object it will always return 1.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always returns 1

.EP

.SI 3
* %^BOLD%^query_worn_by%^RESET%^
.EI
.SI 5
object query_worn_by()
.EI
.SP 7 5

This method returns the person who is currently wearing the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current wearer of the object
.EP

.SI 3
* %^BOLD%^remove_immune_to%^RESET%^
.EI
.SI 5
void remove_immune_to(mixed args)
.EI
.SP 7 5

This method removes a type of damage that the weapon is immune
to.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
args - the type(s) of damage to remove immunity too
.EP

.SI 3
* %^BOLD%^remove_wear_remove_func%^RESET%^
.EI
.SI 5
} void remove_wear_remove_func()
.EI
.SP 7 5

Unset the wear_remove func.

.EP

.SI 3
* %^BOLD%^set_type%^RESET%^
.EI
.SI 5
void set_type(mixed word)
.EI
.SP 7 5

This method sets the type(s) which are associated with the
item.  If the parameter is a string then a single type is associated
with the item, if the parameter is an array then a list of types
is associated with the object.  If any of these types are not
legal and error message will be produced.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the new type(s) to set for the object
.EP

.SI 3
* %^BOLD%^set_wear_effects%^RESET%^
.EI
.SI 5
void set_wear_effects(string * effects)
.EI
.SP 7 5

This method sets the list of effects to be added to the wearer when it
is worn.  These effects will automaticly be added when worn and
removed when taken off.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
effects - the array of effects to be added to the wearer when worn
.EP

.SI 3
* %^BOLD%^set_wear_remove_func%^RESET%^
.EI
.SI 5
void set_wear_remove_func(mixed file, string func)
.EI
.SP 7 5

This method sets the current function associated with wearing and
removing the item.  The value of this function should be choosen
carefully, using an object reference for the name will work but
it will not then be able to restored from the save file.  The same
goes for using function pointers.  It is better to use a real
file name and a string function name.


The function will be called with two arguments, the first argument will be 0 if the object is being removed or the object which it is being worn by if it is non-zero. The second argument will always be the person who was wearing the item, or is about to wear the item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
file - the file to call the function on
.EP
.SP 9 5
func - the function to call
.EP

.SI 3
* %^BOLD%^set_worn_by%^RESET%^
.EI
.SI 5
int set_worn_by(object thing)
.EI
.SP 7 5

This method sets the object as being worn by the passed in object.
It calls all the various worn functions and sets up or removes
all the effects associated with the object.


If the object is alreadying being worn the wear_remove_function will be called with an argument of 0. The method taken_off will be called on the object wearing the object for all the effects associated with this object. 

If the object is being set to be worn by someone the the wear_remove function will be called with an argument being the person who is to wear the object. All of the effects associated with the object will be added to the wearer. 

This calls the method 'person_removing_item' on the effect when some one removes the item. This can be used to make sure the effects are taken off when the item is removed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the new person to wear the object (0 for worn by no one)
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 on failure
.EP


