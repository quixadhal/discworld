.DT
misc.c
Disk World autodoc help
misc.c

.SH Description
.SP 5 5

This class contains information relating to the value and the weight
of the object.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/consecrate, /std/basic/light and /std/basic/move.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h, /include/shops/bank.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^adjust_money%^RESET%^
.EI
.SI 5
varargs int adjust_money(mixed amt, string coin)
.EI
.SP 7 5

This method changes the value of the object by a certain number of
coins.  The coins can be any type.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new value

.EP

.SI 3
* %^BOLD%^adjust_value%^RESET%^
.EI
.SI 5
int adjust_value(int i)
.EI
.SP 7 5

This method changes the current value of the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the amount to change the value by
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new value of the object

.EP

.SI 3
* %^BOLD%^adjust_weight%^RESET%^
.EI
.SI 5
void adjust_weight(int w)
.EI
.SP 7 5

This method adjusts the weight by the given amount.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
w - the amount to change the weight by
.EP

.SI 3
* %^BOLD%^query_base_value%^RESET%^
.EI
.SI 5
int query_base_value()
.EI
.SP 7 5

This method returns the vase value of the object.  This is before
any scaling from things like condition occurs.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the base value

.EP

.SI 3
* %^BOLD%^query_complete_weight%^RESET%^
.EI
.SI 5
int query_complete_weight()
.EI
.SP 7 5

This method returns the current weight of the object. It does basicly
the same thing as the previous call.  NB: this_object()->query_weight()
is pretty much the same as query_weight(), except it takes shadows into
account.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current weight of the object
.EP

.SI 3
* %^BOLD%^query_length%^RESET%^
.EI
.SI 5
int query_length()
.EI
.SP 7 5

This method queries the length of an object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
w - the amount to set the length to
.EP

.SI 3
* %^BOLD%^query_money%^RESET%^
.EI
.SI 5
int query_money(string type)
.EI
.SP 7 5

This method returns the number of coins of a certain
type that are in the value of the object.  This is not
a good estimate of value or anything, no idea why it is
here really.  I am sure I had a good reason
at the time :)
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of coin to look for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of coins of that type
.EP

.SI 3
* %^BOLD%^query_money_array%^RESET%^
.EI
.SI 5
mixed * query_money_array()
.EI
.SP 7 5

This method returns the value of the object as a money array.  The
money array is a list oif coinages followed by a number of coins.
Eg: ({ "copper", 10, "silver", 12 }).
.EP

.SI 3
* %^BOLD%^query_value%^RESET%^
.EI
.SI 5
int query_value()
.EI
.SP 7 5

This method returns the current value of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current value of the object

.EP

.SI 3
* %^BOLD%^query_value_at%^RESET%^
.EI
.SI 5
int query_value_at(object place)
.EI
.SP 7 5

This method figures out how much an object will cost in a certain
place.  For instance at a shop that does not handle artifcacts an
artifact will be bought and sold cheaply.  The current types
of "artifact", "enchantment" and "material" are recognised.
A property in the shop of the type "artifact valued" will
cause the values of that type to be taken into account.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
place - the object to find the value at
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value in the shop
.EP

.SI 3
* %^BOLD%^query_value_info%^RESET%^
.EI
.SI 5
mapping query_value_info()
.EI
.SP 7 5

This method returns the information associated for all the special
bits of the object.  The value infor maping ihas keys which are
the type of value info and the value is the value of it.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value info mapping
.EP

.SI 3
* %^BOLD%^query_value_real%^RESET%^
.EI
.SI 5
int query_value_real(string place)
.EI
.SP 7 5

This method always returns the most expensive value of an item.
This should be used for things like theif quotas and anything which
requires the real actual value of an item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
place - the place to find the values in

.EP

.SI 3
* %^BOLD%^query_weight%^RESET%^
.EI
.SI 5
int query_weight()
.EI
.SP 7 5

This method returns the current weight of the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current weight of the object
.EP

.SI 3
* %^BOLD%^query_width%^RESET%^
.EI
.SI 5
int query_width()
.EI
.SP 7 5

This method queries the width of an object. The width should be the shorter
side of an object and the length its longer one. Sadly someone chose inches
as the size units. :(

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
w - the amount to set the width to
.EP

.SI 3
* %^BOLD%^remove_value_info%^RESET%^
.EI
.SI 5
void remove_value_info(string word)
.EI
.SP 7 5

This method removes the value information for a type of
special object.  This is the extra value information associated
with the "artifact", "enchantment" or "material" of the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the type of value information to remove
.EP

.SI 3
* %^BOLD%^set_length%^RESET%^
.EI
.SI 5
void set_length(int l)
.EI
.SP 7 5

This method sets the length to the given amount. The length should be the
long side of an object, the width its shorter one. Sadly someone chose
inches as the size units. :(

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
w - the amount to set the length to.
.EP

.SI 3
* %^BOLD%^set_value%^RESET%^
.EI
.SI 5
void set_value(int number)
.EI
.SP 7 5

This method sets the value of the object.  The actual value in
coins and so forth is worked out by the money handler based on the
value.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the new value of the object
.EP

.SI 3
* %^BOLD%^set_value_info%^RESET%^
.EI
.SI 5
void set_value_info(string word, int number)
.EI
.SP 7 5

This method sets the value information for a type of
special object.  This is the extra value information associated
with the "artifact", "enchantment" or "material" of the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the type of value information
.EP
.SP 9 5
number - the amount to set it to
.EP

.SI 3
* %^BOLD%^set_weight%^RESET%^
.EI
.SI 5
void set_weight(int w)
.EI
.SP 7 5

This method sets the weight of the object.  One weight unit
is 50 grams.


When setting the weights of weapons use the following guide: 
.EP
.SP 7 5
o===================o=====================o================o
.EP
.SP 7 5
| Weapon            | Approx. Weight (kg) |  Weight Units  |
.EP
.SP 7 5
o===================o=====================o================o
.EP
.SP 7 5
| Dagger            |        0.5          |       10       |
.EP
.SP 7 5
| War Hammer        |        1.1          |       22       |
.EP
.SP 7 5
| Mace              |        1.3          |       26       |
.EP
.SP 7 5
| Flail             |        1.5          |       30       |
.EP
.SP 7 5
| Pole Axe          |        2.3          |       46       |
.EP
.SP 7 5
| Short Sword       |        0.8          |       16       |
.EP
.SP 7 5
| Broad Sword       |        1.1          |       22       |
.EP
.SP 7 5
| Long Sword        |        1.4          |       28       |
.EP
.SP 7 5
| Bastard Sword     |        1.9          |       38       |
.EP
.SP 7 5
| Two-handed Sword  |        2.7          |       42       |
.EP
.SP 7 5
o===================o=====================o================o
.EP
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
w - the amount to change the weight by
.EP

.SI 3
* %^BOLD%^set_width%^RESET%^
.EI
.SI 5
void set_width(int w)
.EI
.SP 7 5

This method sets the width to the given amount.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
w - the amount to set the width to
.EP


