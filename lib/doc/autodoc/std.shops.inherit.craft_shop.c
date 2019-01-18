.DT
craft_shop.c
Disk World autodoc help
craft_shop.c

.SH Description
.SP 5 5

This room will handle craft shop like things, where you sell stuff to
the store and it will sell it onto other players after taking
a margin.  This can be used for potions, pottery, swords, anything!
This has a few small hooks in here to handle categories in the
sell process.  Most of the category handling code is dealt with in
the category based craft shop.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Feb  5 15:39:57 CST 1998

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/inherit/open_shop.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h, /include/shops/craft_shop.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_open%^RESET%^
.EI
.SI 5
int check_open(object player, string type)
.EI
.SP 7 5

This method checks to see if the shop is open or not.  It will setup the
default fail message if this is what is needed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
command - the command to check for
.EP

.SI 3
* %^BOLD%^do_browse%^RESET%^
.EI
.SI 5
int do_browse(object * obs)
.EI
.SP 7 5

This function browses through the current selected list of items.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the items to browse through

.EP

.SI 3
* %^BOLD%^do_buy%^RESET%^
.EI
.SI 5
int do_buy(object * obs, int force)
.EI
.SP 7 5

This method will attempt to buy an object from the shops inventory.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the string to match an object with
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_change%^RESET%^
.EI
.SI 5
int do_change(object * obs, string change)
.EI
.SP 7 5

This function changes the list price of an object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the object to change the list price of
.EP
.SP 9 5
change - the value to change the list price to

.EP

.SI 3
* %^BOLD%^do_change_name%^RESET%^
.EI
.SI 5
int do_change_name(object * obs, string new_name)
.EI
.SP 7 5

This function changes the list name of an object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the object to change the list name of
.EP
.SP 9 5
new_name - the value to change the list name to

.EP

.SI 3
* %^BOLD%^do_collect%^RESET%^
.EI
.SI 5
int do_collect()
.EI
.SP 7 5

This method collects any royalties the current player is owed.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on succes, 0 on failure

.EP

.SI 3
* %^BOLD%^do_delete%^RESET%^
.EI
.SI 5
int do_delete(string id)
.EI
.SP 7 5

This method will delete a list entry without the owner getting paid.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the string to match an object with
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_list%^RESET%^
.EI
.SI 5
int do_list()
.EI
.SP 7 5

This method will list the current set of objects which are
available to buy.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_royalties%^RESET%^
.EI
.SI 5
int do_royalties()
.EI
.SP 7 5

This method shows any royalties that you might be able to collect.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on succes, 0 on failure

.EP

.SI 3
* %^BOLD%^do_sell%^RESET%^
.EI
.SI 5
int do_sell(object * obs)
.EI
.SP 7 5

This method sells an object into the craft shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects to sell
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^do_sell_id%^RESET%^
.EI
.SI 5
int do_sell_id(object * obs, string id, int force)
.EI
.SP 7 5

This method tries to sell an object as a name that already exists in
the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects to sell
.EP
.SP 9 5
id - the id to sell them as
.EP

.SI 3
* %^BOLD%^inform_of_buy%^RESET%^
.EI
.SI 5
void inform_of_buy(int value, object * obs, object player, string * sellers, string * names, string * cats, int * values)
.EI
.SP 7 5

This method is called when something is bought and tells us how much
has been spent.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
value - of the the items sold
.EP
.SP 9 5
obs - the objects bought
.EP
.SP 9 5
player - who bought them
.EP
.SP 9 5
sellers - the people whose items were bought
.EP
.SP 9 5
names - the names of the items bought
.EP
.SP 9 5
cats - the categories of the items bought
.EP
.SP 9 5
values - the values of each item

.EP

.SI 3
* %^BOLD%^is_able_to_change%^RESET%^
.EI
.SI 5
int is_able_to_change(object ob)
.EI
.SP 7 5

This allows the system to override this for player run shops so the
owner can discount peoples items.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to test
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they can modify the item, 0 if not

.EP

.SI 3
* %^BOLD%^is_allowed_to_sell%^RESET%^
.EI
.SI 5
int is_allowed_to_sell(object * obs, string name, string * sellable_names)
.EI
.SP 7 5

This method checks to see if this item is allowed to be sold in this shop.
It assumes it is called from a add_command() method and sets up fail
messages accordingly.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects to check
.EP
.SP 9 5
name - the name of the person doing the selling
.EP
.SP 9 5
sellable_names - the names of the items to sell

.EP

.SI 3
* %^BOLD%^query_allowed_to_sell%^RESET%^
.EI
.SI 5
function query_allowed_to_sell()
.EI
.SP 7 5

This method returns the function used to check to see if an object
is able to be sold here or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
func the function to use for checking
.EP

.SI 3
* %^BOLD%^query_always_ask_price%^RESET%^
.EI
.SI 5
int query_always_ask_price()
.EI
.SP 7 5

This method returns the flag on this object that tells us if the shop
will always ask for a price
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the flag

.EP

.SI 3
* %^BOLD%^query_controller%^RESET%^
.EI
.SI 5
object query_controller()
.EI
.SP 7 5

This method returns the current value of the controller.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current controller
.EP

.SI 3
* %^BOLD%^query_craft_shop_save_dir%^RESET%^
.EI
.SI 5
string query_craft_shop_save_dir()
.EI
.SP 7 5

This method returns the file name of the save file.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the file name of the save file
.EP

.SI 3
* %^BOLD%^query_cut%^RESET%^
.EI
.SI 5
int query_cut()
.EI
.SP 7 5

This method queries the current cut defined for this shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current cut for the shop
.EP

.SI 3
* %^BOLD%^query_extra_price_information%^RESET%^
.EI
.SI 5
string query_extra_price_information(string person, object * obs)
.EI
.SP 7 5

This method returns any extra information about prices that the
shop wants to display before entering a cost.  This can be
overridden in higher inherits to control this more effectively.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an extra string to print

.EP

.SI 3
* %^BOLD%^query_extra_sell_check%^RESET%^
.EI
.SI 5
function query_extra_sell_check()
.EI
.SP 7 5

This method returns the extra sell check function.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the extra sell check function
.EP

.SI 3
* %^BOLD%^query_max_sell_per_person%^RESET%^
.EI
.SI 5
int query_max_sell_per_person()
.EI
.SP 7 5

This methods returns the maximum number of allowed objects to be
sold by each person into the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maximum number to be sold
.EP

.SI 3
* %^BOLD%^query_maximum_sale_value%^RESET%^
.EI
.SI 5
int query_maximum_sale_value(string person, object * obs)
.EI
.SP 7 5

This method will return the maximum price this item is allowed to
be sold for.  This will return 0 if there is no maximum price
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if there is no maximum price, or maximum price

.EP

.SI 3
* %^BOLD%^query_minimum_cost%^RESET%^
.EI
.SI 5
int query_minimum_cost()
.EI
.SP 7 5

This method returns the minimum allowed cost for an item in the
shop inventory.
.EP

.SI 3
* %^BOLD%^query_no_royalty_commands%^RESET%^
.EI
.SI 5
int query_no_royalty_commands()
.EI
.SP 7 5

This method returns the flagt the removes the royalty commands from the
shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the royalty command flag

.EP

.SI 3
* %^BOLD%^query_use_name%^RESET%^
.EI
.SI 5
int query_use_name()
.EI
.SP 7 5

This method returns the flag for the use name of this shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the name is used, 0 if not

.EP

.SI 3
* %^BOLD%^query_when_sold_function%^RESET%^
.EI
.SI 5
function query_when_sold_function()
.EI
.SP 7 5

This method returns the when sold function.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the when sold function
.EP

.SI 3
* %^BOLD%^reload_shop%^RESET%^
.EI
.SI 5
void reload_shop()
.EI
.SP 7 5

This method allows you to cause the shop to load itself up.

.EP

.SI 3
* %^BOLD%^set_allowed_to_sell%^RESET%^
.EI
.SI 5
void set_allowed_to_sell(function func)
.EI
.SP 7 5

This method sets the function to use to check to see if the object
is able to be sold here or not.  The function will be called with
one parameter, that is the object being check for buyability.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to check objects with
.EP

.SI 3
* %^BOLD%^set_always_ask_price%^RESET%^
.EI
.SI 5
void set_always_ask_price(int ask_price)
.EI
.SP 7 5

This method sets the flag which makes the shop always ask for a price
when it attempts to save something.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ask_price - the flag

.EP

.SI 3
* %^BOLD%^set_controller%^RESET%^
.EI
.SI 5
void set_controller(string name)
.EI
.SP 7 5

This method sets the controller object for this shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
controller - the controller name as a string
.EP

.SI 3
* %^BOLD%^set_cut%^RESET%^
.EI
.SI 5
void set_cut(int new_cut)
.EI
.SP 7 5

This method sets the cut for the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_cut - the new cut of the object
.EP

.SI 3
* %^BOLD%^set_extra_sell_check%^RESET%^
.EI
.SI 5
void set_extra_sell_check(function func)
.EI
.SP 7 5

This method sets the extra sell function for the shop.  The extra sell
function is called after the sell process is almost finished.  It can
check for extra things needed in the sell process, like which page of
the book to browse.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the extra sell function
.EP

.SI 3
* %^BOLD%^set_max_sell_per_person%^RESET%^
.EI
.SI 5
void set_max_sell_per_person(int num)
.EI
.SP 7 5

This method sets the maxium allowed number of objects to be sold
by each person into the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the maximum number to be sold
.EP

.SI 3
* %^BOLD%^set_minimum_cost%^RESET%^
.EI
.SI 5
void set_minimum_cost(int cost)
.EI
.SP 7 5

This method sets the minimum allowed cost for the item in the
shop inventory.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cost - the minimum cost
.EP

.SI 3
* %^BOLD%^set_no_royalty_commands%^RESET%^
.EI
.SI 5
void set_no_royalty_commands(int flag)
.EI
.SP 7 5

This method sets the flag that removes the royalty commands from the
shop, if the royalty stuff is be handled somewhere else.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
flag - the flag to set

.EP

.SI 3
* %^BOLD%^set_save_dir%^RESET%^
.EI
.SI 5
void set_save_dir(string fname)
.EI
.SP 7 5

This method sets the save file associated with the controller.  The save
file should actually be a directory, the name of the directory plus a
.o will be used for the save files.  The directory must exist or weird
errors will occur.
This should always be set in the setup function of the room.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the save file
.EP

.SI 3
* %^BOLD%^set_use_name%^RESET%^
.EI
.SI 5
void set_use_name(int flag)
.EI
.SP 7 5

This method sets the flag which makes the shop use the name of the
item as the sell type always.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
flag - 1 to make it force the use name ability

.EP

.SI 3
* %^BOLD%^set_when_sold_function%^RESET%^
.EI
.SI 5
void set_when_sold_function(function func)
.EI
.SP 7 5

This method sets the function to be called on all the objects when
they are sold.  For instance this can be used to close all books
etc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to use
.EP

.SI 3
* %^BOLD%^value_from_string%^RESET%^
.EI
.SI 5
int value_from_string(string str, string place)
.EI
.SP 7 5

This attempts to figure out how much the person is telling us the
item costs.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the string to get the value of

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^check_sell_name%^RESET%^
.EI
.SI 5
int check_sell_name(string name)
.EI
.SP 7 5

This method makes sure a name is not too long and contains
no colour codes.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name to list the object as
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if name is unacceptable, 1 if it's OK

.EP

.SI 3
* %^BOLD%^complete_sale%^RESET%^
.EI
.SI 5
void complete_sale(int value, string name, object * sellable, string category)
.EI
.SP 7 5

This method is called to complete the sale completely.  It is split
up into a second function to allow the extra sell stuff
to work neatly.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
value - the value of the objects to sell
.EP
.SP 9 5
name - the list name
.EP
.SP 9 5
sellable - the list of objects to sell
.EP
.SP 9 5
category - the category of the object, 0 if none

.EP

.SI 3
* %^BOLD%^confirm_sale%^RESET%^
.EI
.SI 5
void confirm_sale(string answer, int value, string name, object * sellable, string category)
.EI
.SP 7 5

This method confirms the sale after the cost and name have been
determined.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
answer - the answer to confirm with
.EP
.SP 9 5
value - the value of the object
.EP
.SP 9 5
name - the name of the object
.EP
.SP 9 5
sellable - the list of sellable objects
.EP
.SP 9 5
category - the category of the object, 0 if none

.EP

.SI 3
* %^BOLD%^confirm_sell_name_choice%^RESET%^
.EI
.SI 5
void confirm_sell_name_choice(string answer, string name, object * sellable)
.EI
.SP 7 5

This method is used to confirm the choice of name to use when
selling the object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
answer - the confirmation choice
.EP
.SP 9 5
name - the name to sell it as
.EP
.SP 9 5
sellable - the list of objects to sell

.EP

.SI 3
* %^BOLD%^determine_cost%^RESET%^
.EI
.SI 5
void determine_cost(string cost, string name, object * sellable)
.EI
.SP 7 5

This method figured out the cost of the item based on the value they
type in.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cost - the cost of the item
.EP
.SP 9 5
name - the name of the item
.EP
.SP 9 5
sellable - the array of objects to sell

.EP

.SI 3
* %^BOLD%^do_confirm_sale%^RESET%^
.EI
.SI 5
void do_confirm_sale(int value, string name, object * sellables)
.EI
.SP 7 5

This method will ask the person to confirm the sale.  It will check
the extra_sell_check function and then call the extra sale thing.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
value - the value of the thing being sold
.EP
.SP 9 5
name - the name the thing is sold for
.EP
.SP 9 5
sellables - the things to sell

.EP

.SI 3
* %^BOLD%^do_read_file%^RESET%^
.EI
.SI 5
string do_read_file(int type, string fname)
.EI
.SP 7 5

This method handles the reading of the file to the disk.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of file to read
.EP
.SP 9 5
fname - the file name is optional and only used for data files
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the value read

.EP

.SI 3
* %^BOLD%^do_save_file%^RESET%^
.EI
.SI 5
void do_save_file(int type, mixed value, string fname)
.EI
.SP 7 5

This method handles the saving of the file to the disk.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of file to save
.EP
.SP 9 5
value - the value to save to it
.EP
.SP 9 5
fname - the file name is optional and only used for data files

.EP

.SI 3
* %^BOLD%^sell_choice%^RESET%^
.EI
.SI 5
void sell_choice(string answer, object * sellables, string * names)
.EI
.SP 7 5

This method is used when there is a list of object already listed.  It
will check to see if the choice entered is valid and then use that
name when selling the object if it is.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
choice - the selected choice
.EP
.SP 9 5
sellable - the array of sellable objects
.EP
.SP 9 5
names - the names of choices for name

.EP

.SI 3
* %^BOLD%^sell_name%^RESET%^
.EI
.SI 5
void sell_name(string name, object * sellable)
.EI
.SP 7 5

This method determines the name to list the object under.  This is
called when a new name is used.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name to list the object as
.EP
.SP 9 5
sellable - the list of sellable objects

.EP


