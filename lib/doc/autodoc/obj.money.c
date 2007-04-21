.DT
money.c
DW_wibble autodoc help
money.c

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/money.h, /include/move_failures.h, /include/obj_parser.h and /include/shops/bank.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^adjust_money%^RESET%^
.EI
.SI 5
varargs int adjust_money(mixed amount, string type)
.EI

.SI 3
* %^BOLD%^check_adjectives%^RESET%^
.EI
.SI 5
void check_adjectives()
.EI
.SP 7 5

This method goes through the coins and sets up all the adjectives
and plurals it needs to.

.EP

.SI 3
* %^BOLD%^find_best_fit%^RESET%^
.EI
.SI 5
int find_best_fit(mixed word)
.EI

.SI 3
* %^BOLD%^fixup_money_weight%^RESET%^
.EI
.SI 5
void fixup_money_weight()
.EI
.SP 7 5

This method fixes up the weight of the money object to be what it should
be.

.EP

.SI 3
* %^BOLD%^force_no_join%^RESET%^
.EI
.SI 5
void force_no_join()
.EI

.SI 3
* %^BOLD%^group_object%^RESET%^
.EI
.SI 5
int group_object()
.EI

.SI 3
* %^BOLD%^init_dynamic_arg%^RESET%^
.EI
.SI 5
void init_dynamic_arg(mapping map,  object)
.EI

.SI 3
* %^BOLD%^merge_coins%^RESET%^
.EI
.SI 5
object merge_coins()
.EI
.SP 7 5

This method merges two coin object together.  Or attempts to anyway.
This will occur whenever a coin object moves.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the merged coin object

.EP

.SI 3
* %^BOLD%^new_money_object%^RESET%^
.EI
.SI 5
object new_money_object(mixed number, string type)
.EI
.SP 7 5

This method creates a new money object from the current object.  It uses
the current objects values to make the money and removes the avlues from
the coins.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the number of coins to remove
.EP
.SP 9 5
type - the type of coins to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new money object

.EP

.SI 3
* %^BOLD%^query_already_joined%^RESET%^
.EI
.SI 5
int query_already_joined()
.EI

.SI 3
* %^BOLD%^query_collective%^RESET%^
.EI
.SI 5
int query_collective()
.EI

.SI 3
* %^BOLD%^query_dynamic_auto_load%^RESET%^
.EI
.SI 5
mapping query_dynamic_auto_load()
.EI

.SI 3
* %^BOLD%^query_merger%^RESET%^
.EI
.SI 5
int query_merger()
.EI

.SI 3
* %^BOLD%^query_money%^RESET%^
.EI
.SI 5
int query_money(string type)
.EI
.SP 7 5

This method returns the amount of type of money there is in the array.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of money to check
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

.SI 3
* %^BOLD%^query_no_join%^RESET%^
.EI
.SI 5
int query_no_join()
.EI

.SI 3
* %^BOLD%^query_number_coins%^RESET%^
.EI
.SI 5
int query_number_coins()
.EI
.SP 7 5

This method returns the number of coins in the object.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number of coins

.EP

.SI 3
* %^BOLD%^reset_already_joined%^RESET%^
.EI
.SI 5
void reset_already_joined()
.EI

.SI 3
* %^BOLD%^reset_no_join%^RESET%^
.EI
.SI 5
void reset_no_join()
.EI

.SI 3
* %^BOLD%^set_already_joined%^RESET%^
.EI
.SI 5
void set_already_joined()
.EI

.SI 3
* %^BOLD%^set_money_array%^RESET%^
.EI
.SI 5
void set_money_array(mixed * new_array)
.EI

.SI 3
* %^BOLD%^set_no_join%^RESET%^
.EI
.SI 5
void set_no_join()
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


