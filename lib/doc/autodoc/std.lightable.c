.DT
lightable.c
Disk World autodoc help
lightable.c

.SH Description
.SP 5 5
 This is the standard lightable.  It can be used for candles,
lanterns, lamps etc.  It adds the commands "light" and
"dowse"/"extinguish" to the player, but no refuelling
command.  Holding stuff is handled by /obj/weapon and the
ho_ld command.
.EP
.SP 10 5


Written by Gruper

Started 11th of May, 1998

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/weapon.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/fuel_handler.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^current_fuel_message%^RESET%^
.EI
.SI 5
string current_fuel_message()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
String describing how much fuel is left.

.EP

.SI 3
* %^BOLD%^query_brightness%^RESET%^
.EI
.SI 5
int query_brightness()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
How brightly the object shines when lit

.EP

.SI 3
* %^BOLD%^query_empty_mess%^RESET%^
.EI
.SI 5
string query_empty_mess()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
String empty_mess

.EP

.SI 3
* %^BOLD%^query_fuel%^RESET%^
.EI
.SI 5
int query_fuel()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The current amount of fuel.

.EP

.SI 3
* %^BOLD%^query_fuel_messages%^RESET%^
.EI
.SI 5
mixed query_fuel_messages()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a mapping containing all the different fuel_messages

.EP

.SI 3
* %^BOLD%^query_hold_required%^RESET%^
.EI
.SI 5
int query_hold_required()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
number of hands required to hold object when lit

.EP

.SI 3
* %^BOLD%^query_lit%^RESET%^
.EI
.SI 5
int query_lit()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if lit, 0 if unlit

.EP

.SI 3
* %^BOLD%^query_max_fuel%^RESET%^
.EI
.SI 5
int query_max_fuel()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The max amount of fuel the object can contain.

.EP

.SI 3
* %^BOLD%^set_brightness%^RESET%^
.EI
.SI 5
void set_brightness(int i)
.EI
.SP 7 5

The brightness is the number used in set_light(),
ie how brightly the lightable shines when lit.

.EP

.SI 3
* %^BOLD%^set_empty_mess%^RESET%^
.EI
.SI 5
void set_empty_mess(string msg)
.EI
.SP 7 5

The empty message is a string used to describe the lightable
when it is out of fuel.  It should be on the form
"is burnt to a stub." to fit both the_short() +" "+ msg
and "It "+ msg.

.EP

.SI 3
* %^BOLD%^set_fuel%^RESET%^
.EI
.SI 5
void set_fuel(int i)
.EI
.SP 7 5

Sets the current amount of fuel.  One fuel unit equals
one second of burning time.  If fuel > max_fuel,
fuel = max_fuel, so it is important to set max_fuel
before fuel.

.EP

.SI 3
* %^BOLD%^set_fuel_messages%^RESET%^
.EI
.SI 5
void set_fuel_messages(mixed msgs)
.EI
.SP 7 5

This function is used to set the different messages shown
depending on how much fuel is left in the lightable.
The fuel messages should be on a form suitable to be
appended to the_short() +" is lit/not lit.  "
The argument msgs can either be an array of strings or an
array of string, int pairs.  In the first case, the fuel
messages will be evenly spaced.  In the second case, the
int is a percentage (fuel_left*100/max_fuel) below which
the string will be used.  If no message for 100 is given,
it will default to the last string element in the array.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
msgs - A mixed array of either strings or string, int pairs
.EP

.SI 3
* %^BOLD%^set_hold_required%^RESET%^
.EI
.SI 5
void set_hold_required(int hands)
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
hands - The number of hands required to hold the object when lit
Any non-zero value will cause the ho_ld command to be executed,
so use set_no_limbs( 2 ) as usual for two-handed weapons.
I guess what I'm really trying to say is that a 0 will not require
the object to be held and any other value will.

In winter darkness


Gruper lights a cheerful flame


It smells like honey



.EP

.SI 3
* %^BOLD%^set_lit%^RESET%^
.EI
.SI 5
int set_lit(int i)
.EI
.SP 7 5

This function is used to light or dowse the lightable.
Two properties are checked: unextinguishable means that
the lightable cannot be extinguished and unlightable that
it cannot be lit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - 1 for lit and 0 for unlit
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The current state, 1 for lit, 0 for unlit

.EP

.SI 3
* %^BOLD%^set_max_fuel%^RESET%^
.EI
.SI 5
void set_max_fuel(int i)
.EI
.SP 7 5

Sets the maximum amount of fuel.
One fuel unit equals one second of burning time.

.EP


