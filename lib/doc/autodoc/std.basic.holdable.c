.DT
holdable.c
Disk World autodoc help
holdable.c

.SH Description
.SP 5 5

A nice file to allow you to setup an object as being holdable.  To deal
with this inheritable you only need to define the method held_this_item
if you wish to do anything when the item is held.
.EP
.SP 10 5


Written by Pinkfish

Started Wed Feb  2 15:38:10 PST 2000

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^held_this_item%^RESET%^
.EI
.SI 5
int held_this_item(int held, object holder, mixed arg)
.EI
.SP 7 5

This method is called by the inheritable when the object is change from
being held to unheld, or vica versa.  This should be used to turn on 
and off light sources and so on.  The method will also be called with 
a 2 if the object was held and we are doing an initialisation sequence.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
held - 1 if the item is held, 0 if not, 2 if held on init
.EP
.SP 9 5
holder - this is person already holding it when removed, new holder when held
.EP
.SP 9 5
arg - an argument use in the autoloading, passed into init_dynamic_arg
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the hold/remove was successful

.EP

.SI 3
* %^BOLD%^hold_item%^RESET%^
.EI
.SI 5
varargs int * hold_item(object who, int pos)
.EI
.SP 7 5


Attempts to free up enough limbs for this_object to be held.  If
items are dropped to make this possible, who is notified.  If 
enough limbs are freed, attempts to hold this_object.  If the position
is undefined (ie: not specified) then it will try and choose a limb
by itself.


The return numbers are the limb numbers that were used by holding the item. 

hold_item: As designed by Tannah!
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
who - The living object trying to hold this one.
.EP
.SP 9 5
pos - the position to hold the item in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the an (int *) if successful or ({ }) if not enough limbs are freed

.EP

.SI 3
* %^BOLD%^move%^RESET%^
.EI
.SI 5
int move(mixed ob)
.EI
.SP 7 5

This method should be called by the upper level moves.  The result
needs to be check, if it is not MOVE_OK then the move should be
stopped.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to move

.EP

.SI 3
* %^BOLD%^query_holder%^RESET%^
.EI
.SI 5
object query_holder()
.EI
.SP 7 5

This method returns the person who is holding the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the person holding the object

.EP

.SI 3
* %^BOLD%^query_my_limb%^RESET%^
.EI
.SI 5
int query_my_limb()
.EI
.SP 7 5

This method returns the current limb the object is being
held in.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number limb the object is held in

.EP

.SI 3
* %^BOLD%^query_no_limbs%^RESET%^
.EI
.SI 5
int query_no_limbs()
.EI
.SP 7 5

This method returns the number of limbs that the object uses.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of limbs the object uses when held

.EP

.SI 3
* %^BOLD%^set_holder%^RESET%^
.EI
.SI 5
int set_holder(object ob, int limb)
.EI
.SP 7 5

This method is called from the living object when we are held or
unheld.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the person holding us
.EP
.SP 9 5
limb - the limb we are being held in

.EP

.SI 3
* %^BOLD%^set_no_limbs%^RESET%^
.EI
.SI 5
void set_no_limbs(int num)
.EI
.SP 7 5

This method sets the number of limbs the object uses when held.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the number of limbs

.EP


