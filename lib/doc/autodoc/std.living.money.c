.DT
money.c
Disk World autodoc help
money.c

.SH Description
.SP 5 5

This file has all the functions to deal with living objects and
money.
.EP
.SP 10 5


Written by Pinkfish
.EP



.SH See also
.SP 5 5
/obj/money.c and /obj/handlers/money_handler.c

.EP
.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^adjust_money%^RESET%^
.EI
.SI 5
varargs void adjust_money(mixed amount, string type)
.EI
.SP 7 5

This method adjusts the amount of money the player has.
It redirects the call off the the money object and calls the
adjust_money() function on it.  The amount can be a money
array, in which case it will adjust the size by the members
of the money array.


If the money object doesn't exist yet, this function might create a new money object. The exception is that if the amount argument is 0, nothing happens.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
amount - the amount to adjust the money by
.EP
.SP 9 5
type - the type of coin to adjust

.EP

.SI 3
* %^BOLD%^pay_money%^RESET%^
.EI
.SI 5
varargs void pay_money(mixed m_array, string where)
.EI
.SP 7 5

This method makes the player make a payment.  This is used for
shops and things.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
m_array - the money to pay
.EP
.SP 9 5
where - the money area to make the payment in
.EP

.SI 3
* %^BOLD%^query_money%^RESET%^
.EI
.SI 5
int query_money(string type)
.EI
.SP 7 5

This method returns all of the money of a certain type on the
money object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of money to return 
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of those money items
.EP

.SI 3
* %^BOLD%^query_money_array%^RESET%^
.EI
.SI 5
mixed * query_money_array()
.EI
.SP 7 5

This method returns the money array associated with the money on
the player object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the money array on the player object
.EP

.SI 3
* %^BOLD%^query_money_object%^RESET%^
.EI
.SI 5
object query_money_object(int force)
.EI
.SP 7 5

This method returns the money object for the player.  If the force
flag is set then the object will be created if it does not
exist.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
force - make the money object exist

.EP

.SI 3
* %^BOLD%^query_value%^RESET%^
.EI
.SI 5
int query_value()
.EI
.SP 7 5

This method returns the value of the money as an integer.  This is
an absolute value of their worth.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an integer value of their money
.EP

.SI 3
* %^BOLD%^query_value_in%^RESET%^
.EI
.SI 5
int query_value_in(string where)
.EI
.SP 7 5

This method returns the value of the money as in integer in the specified
place.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
where - the place in which to determine the money from
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the integer value of money
.EP

.SI 3
* %^BOLD%^set_money_array%^RESET%^
.EI
.SI 5
void set_money_array(mixed * new_array)
.EI
.SP 7 5

This method sets the money array associated with the player.   THis
will overwrite any current money on the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_array - the new money array for the player
.EP


