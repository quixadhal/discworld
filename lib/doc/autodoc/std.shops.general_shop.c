.DT
general_shop.c
Disk World autodoc help
general_shop.c

.SH Description
.SP 5 5

This the base shop.  It buys and sells stuff form players.


A storeroom must be set for the shop! 

Original made who knows when.
.EP
.SP 10 5


Written by Pinkfish
.EP



.SH See also
.SP 5 5
set_storeroom()
.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room and /std/shops/inherit/open_shop.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/armoury.h, /include/tasks.h, /include/move_failures.h, /include/shops/bank.h, /include/shop.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_other_shop%^RESET%^
.EI
.SI 5
void add_other_shop(mixed shop)
.EI
.SP 7 5

This method adds a shop with shich we will exchange inventories
of certain types.  So the main shop can sell off its swords and
stuff to the sword shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
shop - the other shop

.EP

.SI 3
* %^BOLD%^create_mercenary%^RESET%^
.EI
.SI 5
object create_mercenary(object rep)
.EI
.SP 7 5

This method creates a mercenaries to wander along with the
sales rep to get the stuff to the destination.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
rep - the representative to protect
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new mercenary

.EP

.SI 3
* %^BOLD%^create_rep%^RESET%^
.EI
.SI 5
object create_rep()
.EI
.SP 7 5

This method creates the sales representative which is sent off
to exchange goods with other shops.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the sales representative
.EP

.SI 3
* %^BOLD%^creator_object%^RESET%^
.EI
.SI 5
int creator_object(object ob)
.EI
.SP 7 5

This method determines whether or not a given object is a creator object
and should be in this shop or not. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the object is not allowed, 0 if it is.

.EP

.SI 3
* %^BOLD%^do_browse%^RESET%^
.EI
.SI 5
int do_browse(object * obs)
.EI
.SP 7 5

This method is called when the player is browseing stuff.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects to browse
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_buy%^RESET%^
.EI
.SI 5
int do_buy(object * obs)
.EI

.SI 3
* %^BOLD%^do_list%^RESET%^
.EI
.SI 5
int do_list()
.EI
.SP 7 5

This method lists all the objects in stock.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on succes, 0 on failure
.EP

.SI 3
* %^BOLD%^do_list_these%^RESET%^
.EI
.SI 5
int do_list_these(object * obs)
.EI
.SP 7 5

This method lists only the specified objects
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_parse%^RESET%^
.EI
.SI 5
void do_parse(mixed arr, object * ob, object client, string money, string extra)
.EI
.SP 7 5

This method is the major message processing function for
the buye messages, sell messages etc.  It handles calling
the functions and setting the results back onto the
player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
arr - the value of the message
.EP
.SP 9 5
ob - the objects to process
.EP
.SP 9 5
money - the money string
.EP
.SP 9 5
extra - the extra string

.EP

.SI 3
* %^BOLD%^do_rep_buy%^RESET%^
.EI
.SI 5
void do_rep_buy(mixed * bing)
.EI
.SP 7 5

This method is called when the rep tries to buy stuff.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bing - the stuff to buy

.EP

.SI 3
* %^BOLD%^do_sell%^RESET%^
.EI
.SI 5
int do_sell(object * in_obs)
.EI
.SP 7 5

This method does the actual selling.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
in_obs - the objects to sell
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_value%^RESET%^
.EI
.SI 5
int do_value(object * obs)
.EI
.SP 7 5

This method is called when the player is valueing stuff.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects to value
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^event_shoplift%^RESET%^
.EI
.SI 5
void event_shoplift(object command_ob, object thief, object victim)
.EI
.SP 7 5

This method is called when a shop lift is done on the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
command_ob - the command object
.EP
.SP 9 5
thief - the theif doing the shop lifting
.EP
.SP 9 5
victim - the victim of the shoplifiting, us I guess :)
.EP

.SI 3
* %^BOLD%^query_browse_mess%^RESET%^
.EI
.SI 5
mixed query_browse_mess()
.EI
.SP 7 5

This method returns the current browse message of the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current browse message of the shop

.EP

.SI 3
* %^BOLD%^query_buy_mess%^RESET%^
.EI
.SI 5
mixed query_buy_mess()
.EI
.SP 7 5

This method returns the current buy message of the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current buy message of the shop

.EP

.SI 3
* %^BOLD%^query_list_mess%^RESET%^
.EI
.SI 5
mixed query_list_mess()
.EI
.SP 7 5

This method returns the current list message of the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current list message of the shop

.EP

.SI 3
* %^BOLD%^query_max_amount%^RESET%^
.EI
.SI 5
int query_max_amount()
.EI
.SP 7 5

This method returns the maximum value oif an object that can
be sold here.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maximum amount
.EP

.SI 3
* %^BOLD%^query_min_amount%^RESET%^
.EI
.SI 5
int query_min_amount()
.EI
.SP 7 5

This method returns the minimum value oif an object that can
be sold here.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the minimum amount
.EP

.SI 3
* %^BOLD%^query_no_steal%^RESET%^
.EI
.SI 5
int query_no_steal()
.EI
.SP 7 5

This method returns the current no steal property.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current no steal properyt of the shop

.EP

.SI 3
* %^BOLD%^query_not_worthy_mess%^RESET%^
.EI
.SI 5
mixed query_not_worthy_mess()
.EI
.SP 7 5

This method returns the current not worthy message of the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current not worthy message of the shop

.EP

.SI 3
* %^BOLD%^query_sell_mess%^RESET%^
.EI
.SI 5
mixed query_sell_mess()
.EI
.SP 7 5

This method returns the current sell message of the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current sell message of the shop

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
* %^BOLD%^query_shop_type%^RESET%^
.EI
.SI 5
string query_shop_type()
.EI
.SP 7 5

This method returns the type of the shop.  This conttrols what sort
of merchandise the shop will buy and sell.


The types of allowed shops are: 
.EP
.SO 8 2 -12

	*	jewelery 
	*	armoury 
	*	clothes 
	*	magic 
	*	none set (ie: 0, general type) 
.EO
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the shop type
.EP

.SI 3
* %^BOLD%^query_shoplift_response_handler%^RESET%^
.EI
.SI 5
string query_shoplift_response_handler()
.EI
.SP 7 5

This method returns the shop lift response handler for the shop.
This allows the shop to respond in someway to someone shop
lifting stuff, like the heavys in Ankh-Morpork.
.EP

.SI 3
* %^BOLD%^query_steal_difficulty%^RESET%^
.EI
.SI 5
int query_steal_difficulty(int i)
.EI
.SP 7 5

This method returns the current steal difficulty of the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current steal difficulty message of the shop

.EP

.SI 3
* %^BOLD%^query_stock%^RESET%^
.EI
.SI 5
object * query_stock(string type)
.EI
.SP 7 5

This method returns alkl the stock in the shop of various
types.  The types are:

.EP
.SO 8 2 -12

	*	jewelery 
	*	armoury 
	*	clothes 
	*	magic 
	*	none set (ie: 0, general type) 
.EO
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of stock to return
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of objects of the type

.EP

.SI 3
* %^BOLD%^query_store_room%^RESET%^
.EI
.SI 5
object query_store_room()
.EI
.SP 7 5

This method returns the storeroom associated with the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current store room

.EP

.SI 3
* %^BOLD%^query_strict_currency%^RESET%^
.EI
.SI 5
int query_strict_currency()
.EI
.SP 7 5

This returns the value of the strict currency variable.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if default currency is accepted, 1 if it is not.

.EP

.SI 3
* %^BOLD%^query_strict_shop%^RESET%^
.EI
.SI 5
int query_strict_shop()
.EI
.SP 7 5

This method returns the structness of the shop, if the shop is strict
it will not deal in items of other types at all.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current strict value
.EP

.SI 3
* %^BOLD%^query_too_costly_mess%^RESET%^
.EI
.SI 5
mixed query_too_costly_mess()
.EI
.SP 7 5

This method returns the current too costly message of the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current too costly message of the shop

.EP

.SI 3
* %^BOLD%^query_value%^RESET%^
.EI
.SI 5
varargs int query_value(object thing, int sell)
.EI
.SP 7 5

This method returns the value of the object in this shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing to value
.EP
.SP 9 5
sell - are we selling it?
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value of the object here
.EP

.SI 3
* %^BOLD%^query_value_mess%^RESET%^
.EI
.SI 5
mixed query_value_mess()
.EI
.SP 7 5

This method returns the current value message of the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current value message of the shop

.EP

.SI 3
* %^BOLD%^rep_came_back%^RESET%^
.EI
.SI 5
void rep_came_back()
.EI
.SP 7 5

This method is called when the rep gets back home.

.EP

.SI 3
* %^BOLD%^rep_made_it%^RESET%^
.EI
.SI 5
void rep_made_it(int bing)
.EI
.SP 7 5

This method is called onces the representative
reaches its destination.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bing - we mkde it ok

.EP

.SI 3
* %^BOLD%^scaled_value%^RESET%^
.EI
.SI 5
int scaled_value(int n)
.EI
.SP 7 5

This returns the amount of money you can sell and object for
which is less than the amount it will be sold for).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
n - the value to scale
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the sell value

.EP

.SI 3
* %^BOLD%^send_out_reps%^RESET%^
.EI
.SI 5
void send_out_reps()
.EI
.SP 7 5

This method checks to see if there are any other shops
associated with ours and sends out representatives to them
to exchange goods.
.EP

.SI 3
* %^BOLD%^set_browse_function%^RESET%^
.EI
.SI 5
void set_browse_function(mixed func)
.EI
.SP 7 5

This method sets the function to call when browsing something.  The
function will be called with two elements, the first being the
player doing the browsing and the second being the array
of objects being browsed.


If the function is a string then the function of that name will be called on the shop, if it is a function pointer then the function pointer will be evaluated.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to call
.EP

.SI 3
* %^BOLD%^set_browse_message%^RESET%^
.EI
.SI 5
void set_browse_message(mixed str)
.EI
.SP 7 5

This sets the message which will be told to the players when
they browse something at the shop.  If the parameter is a string
then that message is sent to the player and nothing is sent to
the other people in the room.  If the message is a two element
array, the first element is sent to the player and the second
element is sent to the others in the room.  In both these
cases the pattersn $ob$ will be replaces with the objects
being sold, $client$ will be replaced with the client, $money$
will be replaced with the money information, $extra$ will be replaces
with any extra information.  In the case oif a browse the
extra information is the long description of the object.


If the parameter is a function pointer, then this is called with the parameter func(obs, client, money, extra);
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the message to print
.EP

.SI 3
* %^BOLD%^set_buy_function%^RESET%^
.EI
.SI 5
void set_buy_function(mixed func)
.EI
.SP 7 5

This method sets the function to call when buying something.  The
function will be called with two elements, the first being the
player doing the buying and the second being the array
of objects being bought.


If the function is a string then the function of that name will be called on the shop, if it is a function pointer then the function pointer will be evaluated.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to call
.EP

.SI 3
* %^BOLD%^set_buy_message%^RESET%^
.EI
.SI 5
void set_buy_message(mixed str)
.EI
.SP 7 5

This sets the message which will be told to the players when
they buy something at the shop.  If the parameter is a string
then that message is sent to the player and nothing is sent to
the other people in the room.  If the message is a two element
array, the first element is sent to the player and the second
element is sent to the others in the room.  In both these
cases the pattersn $ob$ will be replaces with the objects
being sold, $client$ will be replaced with the client, $money$
will be replaced with the money information, $extra$ will be replaces
with any extra information.


If the parameter is a function pointer, then this is called with the parameter func(obs, client, money, extra);
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the message to print
.EP

.SI 3
* %^BOLD%^set_cannot_sell_function%^RESET%^
.EI
.SI 5
void set_cannot_sell_function(mixed func)
.EI
.SP 7 5

This method sets the function to call when selling something
which fails for some reason.  The
function will be called with two elements, the first being the
player doing the selling and the second being the array
of objects being sold.


If the function is a string then the function of that name will be called on the shop, if it is a function pointer then the function pointer will be evaluated.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to call
.EP

.SI 3
* %^BOLD%^set_list_function%^RESET%^
.EI
.SI 5
void set_list_function(mixed func)
.EI
.SP 7 5

This method sets the function to call when listing something.  The
function will be called with two elements, the first being the
player doing the listing and the second being the array
of objects being listed.


If the function is a string then the function of that name will be called on the shop, if it is a function pointer then the function pointer will be evaluated.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to call
.EP

.SI 3
* %^BOLD%^set_list_message%^RESET%^
.EI
.SI 5
void set_list_message(mixed str)
.EI
.SP 7 5

This sets the message which will be told to the players when
they list something at the shop.  If the parameter is a string
then that message is sent to the player and nothing is sent to
the other people in the room.  If the message is a two element
array, the first element is sent to the player and the second
element is sent to the others in the room.  In both these
cases the pattersn $ob$ will be replaces with the objects
being sold, $client$ will be replaced with the client, $money$
will be replaced with the money information, $extra$ will be replaces
with any extra information.  In the case of a list,
the extra information *is* the list.


If the parameter is a function pointer, then this is called with the parameter func(obs, client, money, extra);
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the message to print
.EP

.SI 3
* %^BOLD%^set_max_inventory%^RESET%^
.EI
.SI 5
void set_max_inventory(int i)
.EI
.SP 7 5

This method sets the maximum number of inventory items this shop will
take before it starts to refuse to purchase items from players and
deleting items from its inventory.
The default for this is defined as MAX_INVENTORY in shop.h

.EP

.SI 3
* %^BOLD%^set_min_amount%^RESET%^
.EI
.SI 5
void set_min_amount(int i)
.EI
.SP 7 5

This method sets the minimum value of items that can be sold here.
.EP

.SI 3
* %^BOLD%^set_no_steal%^RESET%^
.EI
.SI 5
void set_no_steal(int i)
.EI
.SP 7 5

This method sets the no steal property.  If a shop is set
as no steal, then it cannot be shoplifted.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the new value of the no_steal property
.EP

.SI 3
* %^BOLD%^set_not_worthy_message%^RESET%^
.EI
.SI 5
void set_not_worthy_message(mixed str)
.EI
.SP 7 5

This sets the message which will be told to the players when
they sell soemthign that is not worth enough at the shop. 
If the parameter is a string
then that message is sent to the player and nothing is sent to
the other people in the room.  If the message is a two element
array, the first element is sent to the player and the second
element is sent to the others in the room.  In both these
cases the pattersn $ob$ will be replaces with the objects
being sold, $client$ will be replaced with the client, $money$
will be replaced with the money information, $extra$ will be replaces
with any extra information.


If the parameter is a function pointer, then this is called with the parameter func(obs, client, money, extra);
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the message to print
.EP

.SI 3
* %^BOLD%^set_open_condition%^RESET%^
.EI
.SI 5
void set_open_condition(mixed str)
.EI
.SP 7 5

This sets the conditons which the shop will be open during.  If this
is set to an integer the shop will always be in that state, so
if you set the open condition to 0, it would always be closed.
If it is set to a string, then that function will be called on
this object to test to see if it is open.  If it iset to 
function pointer, the function pointer will be evaluated.  If it
is set to an array, the first element specifies the object and the
second specifies the function to call.
.EP

.SI 3
* %^BOLD%^set_sell_function%^RESET%^
.EI
.SI 5
void set_sell_function(mixed func)
.EI
.SP 7 5

This method sets the function to call when selling something.  The
function will be called with two elements, the first being the
player doing the selling and the second being the array
of objects being sold.


If the function is a string then the function of that name will be called on the shop, if it is a function pointer then the function pointer will be evaluated.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to call
.EP

.SI 3
* %^BOLD%^set_sell_large%^RESET%^
.EI
.SI 5
void set_sell_large(int i)
.EI
.SP 7 5

This method sets the shop to sell very large objects. ie. If a player
cannot carry items they are placed in the room for the player rather
than not being sold to the player.

.EP

.SI 3
* %^BOLD%^set_sell_message%^RESET%^
.EI
.SI 5
void set_sell_message(mixed str)
.EI
.SP 7 5

This sets the message which will be told to the players when
they sell something at the shop.  If the parameter is a string
then that message is sent to the player and nothing is sent to
the other people in the room.  If the message is a two element
array, the first element is sent to the player and the second
element is sent to the others in the room.  In both these
cases the pattersn $ob$ will be replaces with the objects
being sold, $client$ will be replaced with the client, $money$
will be replaced with the money information, $extra$ will be replaces
with any extra information.


If the parameter is a function pointer, then this is called with the parameter func(obs, client, money, extra);
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the message to print
.EP

.SI 3
* %^BOLD%^set_sell_stolen%^RESET%^
.EI
.SI 5
void set_sell_stolen(int i)
.EI
.SP 7 5

This method sets the shop as a place which will receive and
seel stolen goods.  It automaticly sets the no_steal property
to 1.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the new value for the stolen property

.EP

.SI 3
* %^BOLD%^set_shop_type%^RESET%^
.EI
.SI 5
void set_shop_type(string ty)
.EI
.SP 7 5

This method sets the type of the shop.  This controls what sort of
mechandise the shop will buy and sell.


The types of allowed shops are: 
.EP
.SO 8 2 -12

	*	jewelery 
	*	armoury 
	*	clothes 
	*	magic 
	*	none set (ie: 0, general type) 
.EO
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ty - the type of the shop

.EP

.SI 3
* %^BOLD%^set_shoplift_response_handler%^RESET%^
.EI
.SI 5
void set_shoplift_response_handler(string word)
.EI
.SP 7 5

This method sets the shop lift response handler for the shop.
This allows the shop to respond in someway to shop lifting
stuff, like the heavies in Ankh-Morpork.  The function
'handle_shoplift' will be called on the handler when the
shop lift is attempted.  It will be passed two arguments
the first is the thief, the second is the room being
shoplifted.  This can be set to  afucntion pointer
which will be evaluated and passed in two arguemtns
when  a shoplift occurs.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the new shop lift response handler

.EP

.SI 3
* %^BOLD%^set_steal_difficulty%^RESET%^
.EI
.SI 5
void set_steal_difficulty(int i)
.EI
.SP 7 5

This method sets the difficulty at which to steal stuff from
this shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the new value for the steal difficulty

.EP

.SI 3
* %^BOLD%^set_store_room%^RESET%^
.EI
.SI 5
void set_store_room(mixed ob)
.EI
.SP 7 5

This method sets the current store room associated with
the shop.  This is important!  A shop needs a storeroom.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the storeroom to set
.EP

.SI 3
* %^BOLD%^set_strict_currency%^RESET%^
.EI
.SI 5
void set_strict_currency(int new_strict)
.EI
.SP 7 5

This method controls whether or not a shop will accept currency
from the "default" area.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_strict - set this to 1 if the shop should only accept currency
from the current area it is in (determined by the "place" property).

.EP

.SI 3
* %^BOLD%^set_strict_shop%^RESET%^
.EI
.SI 5
void set_strict_shop(int i)
.EI
.SP 7 5

This method sets the strictness of the shop, if the shop is strict
it will not deal in items of other types at all.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the new strict value
.EP

.SI 3
* %^BOLD%^set_too_costly_function%^RESET%^
.EI
.SI 5
void set_too_costly_function(mixed func)
.EI
.SP 7 5

This method sets the function to call when buying something and it
costs too much.  The
function will be called with two elements, the first being the
player doing the buying and the second being the array
of objects which cost too much.


If the function is a string then the function of that name will be called on the shop, if it is a function pointer then the function pointer will be evaluated.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to call
.EP

.SI 3
* %^BOLD%^set_too_costly_message%^RESET%^
.EI
.SI 5
void set_too_costly_message(mixed str)
.EI
.SP 7 5

This sets the message which will be told to the players when
they buy something at the shop and it costs too much.
If the parameter is a string
then that message is sent to the player and nothing is sent to
the other people in the room.  If the message is a two element
array, the first element is sent to the player and the second
element is sent to the others in the room.  In both these
cases the pattersn $ob$ will be replaces with the objects
being sold, $client$ will be replaced with the client, $money$
will be replaced with the money information, $extra$ will be replaces
with any extra information.


If the parameter is a function pointer, then this is called with the parameter func(obs, client, money, extra);
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the message to print
.EP

.SI 3
* %^BOLD%^set_up_return%^RESET%^
.EI
.SI 5
void set_up_return(object rep)
.EI
.SP 7 5

This method sets up the rep to return home.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
rep - the rep to return home

.EP

.SI 3
* %^BOLD%^set_value_function%^RESET%^
.EI
.SI 5
void set_value_function(mixed func)
.EI
.SP 7 5

This method sets the function to call when an item(*s) are being
valued.  The
function will be called with three elements, the first being the
player doing the valueing and the second being the array
of objects being valued and the third being the string
value of the objects.


If the function is a string then the function of that name will be called on the shop, if it is a function pointer then the function pointer will be evaluated.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to call
.EP

.SI 3
* %^BOLD%^set_value_message%^RESET%^
.EI
.SI 5
void set_value_message(mixed str)
.EI
.SP 7 5

This sets the message which will be told to the players when
they value something at the shop.  If the parameter is a string
then that message is sent to the player and nothing is sent to
the other people in the room.  If the message is a two element
array, the first element is sent to the player and the second
element is sent to the others in the room.  In both these
cases the pattersn $ob$ will be replaces with the objects
being sold, $client$ will be replaced with the client, $money$
will be replaced with the money information, $extra$ will be replaces
with any extra information.


If the parameter is a function pointer, then this is called with the parameter func(obs, client, money, extra);
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the message to print
.EP

.SI 3
* %^BOLD%^shop_list%^RESET%^
.EI
.SI 5
string shop_list(mixed arr, int detail)
.EI
.SP 7 5

This method creates the list  for the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
arr - the array of objecsts to list
.EP
.SP 9 5
detail - display them in detail?
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string list

.EP

.SI 3
* %^BOLD%^shop_parse%^RESET%^
.EI
.SI 5
string shop_parse(string str, mixed ob, object client, string money, string extra, string which)
.EI
.SP 7 5

This is a minature version of do_parse(), which can be used when you
set the money array to a function pointer in order to make message
processing go smoother. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the value of the message
.EP
.SP 9 5
ob - the objects to process
.EP
.SP 9 5
money - the money string
.EP
.SP 9 5
extra - the extra string
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a parsed string

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
* %^BOLD%^summon_guards%^RESET%^
.EI
.SI 5
void summon_guards(object tp)
.EI
.SP 7 5

This method is yused to handle guards to protect the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
tp - the object which is shop lifting
.EP

.SI 3
* %^BOLD%^tidy_inventory%^RESET%^
.EI
.SI 5
void tidy_inventory()
.EI
.SP 7 5

This method is used by the shop to tidyup its inventory.  It does this by
desting objects at random until it has reduced the inventory to 3/4 of its
maximum.

.EP


