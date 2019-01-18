.DT
item_shop.c
Disk World autodoc help
item_shop.c

.SH Description
.SP 5 5

Item shop inherit. 
.EP
.SP 10 5


Written by Pinkfish
.EP



.SH See also
.SP 5 5
/std/shops/inherit/clone_on_demand

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/inherit/clone_on_demand, /std/shops/inherit/shop_event and /std/shops/inherit/open_shop.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h, /include/shops/item_shop.h, /include/shops/bank.h, /include/armoury.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_banking%^RESET%^
.EI
.SI 5
void do_banking()
.EI

.SI 3
* %^BOLD%^query_browse_function%^RESET%^
.EI
.SI 5
function query_browse_function()
.EI
.SP 7 5

This method returns the functionto be used when browseing something.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the function to be used when browseing something
.EP

.SI 3
* %^BOLD%^query_buy_function%^RESET%^
.EI
.SI 5
function query_buy_function()
.EI
.SP 7 5

This method returns the functionto be used when buying something.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the function to be used when buying something
.EP

.SI 3
* %^BOLD%^query_cost%^RESET%^
.EI
.SI 5
int query_cost(object thing)
.EI
.SP 7 5

This method returns the cost of the item shop object being talked about.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing to get the cost of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the cost of the thing

.EP

.SI 3
* %^BOLD%^query_list_function%^RESET%^
.EI
.SI 5
function query_list_function()
.EI
.SP 7 5

This method returns the functionto be used when listing something.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the function to be used when listing something
.EP

.SI 3
* %^BOLD%^query_max_float%^RESET%^
.EI
.SI 5
int query_max_float()
.EI

.SI 3
* %^BOLD%^query_no_steal%^RESET%^
.EI
.SI 5
int query_no_steal()
.EI
.SP 7 5

Queries to see if the room is unstealable.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for making the room unstealable
.EP

.SI 3
* %^BOLD%^query_safe%^RESET%^
.EI
.SI 5
object query_safe()
.EI
.SP 7 5

query_safe - This method returns the object currently being used
as a safe in the shop. 

.EP

.SI 3
* %^BOLD%^query_shop%^RESET%^
.EI
.SI 5
int query_shop()
.EI
.SP 7 5

This method tells us if the shop is really a shop or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 always

.EP

.SI 3
* %^BOLD%^query_shop_lift_items%^RESET%^
.EI
.SI 5
object * query_shop_lift_items(string str, object player)
.EI
.SP 7 5

This method returns the items which can potentially be shop lifted
with the passed in string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the name for the object to attempt to shop lift
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of matching objects
.EP

.SI 3
* %^BOLD%^query_shoplift_response_handler%^RESET%^
.EI
.SI 5
string query_shoplift_response_handler()
.EI

.SI 3
* %^BOLD%^query_sort_function%^RESET%^
.EI
.SI 5
function query_sort_function()
.EI
.SP 7 5

This method returns the function used to sort the inventory of the 
shop when it is printed out.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the function to use to sort the inventory
.EP

.SI 3
* %^BOLD%^query_steal_difficulty%^RESET%^
.EI
.SI 5
int query_steal_difficulty(int i)
.EI
.SP 7 5

This method returns the steal difficulty of the room.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the steal difficulty of the room

.EP

.SI 3
* %^BOLD%^set_browse_function%^RESET%^
.EI
.SI 5
void set_browse_function(function func)
.EI
.SP 7 5

This method sets the function to use when browseing something from
the shop.  The function will be called with two arguements, the
first is the player, the second is the array of objects being
browsed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to set
.EP

.SI 3
* %^BOLD%^set_buy_function%^RESET%^
.EI
.SI 5
void set_buy_function(function func)
.EI
.SP 7 5

This method sets the function to use when buying something from
the shop.  The function will be called with two arguements, the
first is the player, the second is the array of objects being
bought.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to set
.EP

.SI 3
* %^BOLD%^set_list_function%^RESET%^
.EI
.SI 5
void set_list_function(function func)
.EI
.SP 7 5

This method sets the function to use when listing something from
the shop.  The function will be called with two arguements, the
first is the player, the second is the array of objects being
bought.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to set
.EP

.SI 3
* %^BOLD%^set_max_float%^RESET%^
.EI
.SI 5
void set_max_float(int f)
.EI

.SI 3
* %^BOLD%^set_no_steal%^RESET%^
.EI
.SI 5
void set_no_steal(int i)
.EI
.SP 7 5

Sets this room as being unstealable.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - 1 if the room is unable to be stolen from
.EP

.SI 3
* %^BOLD%^set_open_condition%^RESET%^
.EI
.SI 5
void set_open_condition(function func)
.EI
.SP 7 5

This method makes it compatible with the way that the open stuff works in
the normal shops.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to evaluate for openness
.EP

.SI 3
* %^BOLD%^set_open_func%^RESET%^
.EI
.SI 5
void set_open_func(function func)
.EI
.SP 7 5

This method sets the function to evaluate to detmine if the shop 
is open or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to evaluate for openness
.EP

.SI 3
* %^BOLD%^set_safe%^RESET%^
.EI
.SI 5
void set_safe(object safe)
.EI
.SP 7 5

This allows the creators to set their own safe. 

.EP

.SI 3
* %^BOLD%^set_shoplift_response_handler%^RESET%^
.EI
.SI 5
void set_shoplift_response_handler(string word)
.EI

.SI 3
* %^BOLD%^set_sort_function%^RESET%^
.EI
.SI 5
void set_sort_function(function func)
.EI
.SP 7 5

This method sets a function to be used to sort the inventory when it
is printed out.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to use to sort the inventory
.EP

.SI 3
* %^BOLD%^set_steal_difficulty%^RESET%^
.EI
.SI 5
void set_steal_difficulty(int i)
.EI
.SP 7 5

Sets the steal difficulty of the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the steal difficulty of the room

.EP

.SI 3
* %^BOLD%^setup_safe%^RESET%^
.EI
.SI 5
void setup_safe()
.EI
.SP 7 5

This method creates the safe in the room.  The safe is then used for
various thief related activities.

.EP

.SI 3
* %^BOLD%^shoplift_success%^RESET%^
.EI
.SI 5
object shoplift_success(object ob)
.EI
.SP 7 5

This method turns the objects into real objects (if that is
nessessary, it is not with a normal shop).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to turn into a normal object

.EP

.SI 3
* %^BOLD%^switch_extra_properties%^RESET%^
.EI
.SI 5
mixed * switch_extra_properties()
.EI


