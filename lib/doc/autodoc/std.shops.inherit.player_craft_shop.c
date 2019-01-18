.DT
player_craft_shop.c
Disk World autodoc help
player_craft_shop.c

.SH Description
.SP 5 5

This is the file containing extra controls for the player run
craft shop.  We will assume that all player run craft shops will be
rooms and not npcs.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Apr 21 13:37:44 PDT 2000

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/inherit/craft_shop_category and /std/basic/expressions.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/nroff.h, /include/mail.h, /include/expressions.h, /include/shops/craft_shop.h, /include/player_handler.h, /include/move_failures.h, /include/player.h, /include/am_time.h, /include/shops/bank.h, /include/clubs.h, /include/money.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^approval%^RESET%^
class approval {
mixed hairy;
class expression_type * expressions;
int low_cost;
int high_cost;
mapping items;
int num_allowed;
int high_cost_deny;
}

.EI

.SI 3
* %^BOLD%^approval_item%^RESET%^
class approval_item {
mixed value;
string list_name;
int markup;
int final_cost;
}

.EI

.SI 3
* %^BOLD%^approval_obs%^RESET%^
class approval_obs {
string seller;
string category;
int value;
string name;
int * saved;
int enter_time;
int * checkout;
}

.EI

.SI 3
* %^BOLD%^expression_type%^RESET%^
class expression_type {
int type;
class parse_node * condition;
class parse_node * value;
string list_name;
}

.EI

.SI 3
* %^BOLD%^seller_information%^RESET%^
class seller_information {
int max_sellable;
int value_limit;
int deny_value_limit;
}

.EI

.SI 3
* %^BOLD%^shop_stats%^RESET%^
class shop_stats {
int num_sold;
int value_sold;
}

.EI

.SI 3
* %^BOLD%^shop_transaction%^RESET%^
class shop_transaction {
int time;
string person;
string objects;
string name_cat;
int type;
int amount;
mixed extra;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_to_approval_list%^RESET%^
.EI
.SI 5
object * add_to_approval_list(string seller, object * sellables, int value, string name, string category, int approved)
.EI
.SP 7 5

This method adds an item to the list of items that need to be approved.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
seller - name of the player
.EP
.SP 9 5
sellables - the list of items to sell
.EP
.SP 9 5
value - the cost of the item
.EP
.SP 9 5
name - the name of the items
.EP
.SP 9 5
category - the category of the item
.EP
.SP 9 5
approved - a flag to say if this was approved or not
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
obs the objects we bought

.EP

.SI 3
* %^BOLD%^add_transaction%^RESET%^
.EI
.SI 5
void add_transaction(string person, object * objects, int type, int amount, string name_cat, mixed extra)
.EI
.SP 7 5

This method adds in transaction into the transaction list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person doing the thing
.EP
.SP 9 5
objects - the string name of the objects
.EP
.SP 9 5
type - the type of the transaction
.EP
.SP 9 5
amount - the cost of the transaction
.EP
.SP 9 5
name_cat - the name/category of the item
.EP
.SP 9 5
extra - anything extra about the transaction

.EP

.SI 3
* %^BOLD%^adjust_owners_money%^RESET%^
.EI
.SI 5
void adjust_owners_money(int amt)
.EI
.SP 7 5

This method changes the current level of the owners money.

.EP

.SI 3
* %^BOLD%^adjust_royalty%^RESET%^
.EI
.SI 5
void adjust_royalty(string name, int amount)
.EI
.SP 7 5

This method adjusts the royalty.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to adjust
.EP
.SP 9 5
amt - the amoutn to adjust by

.EP

.SI 3
* %^BOLD%^create_checkout_object%^RESET%^
.EI
.SI 5
object create_checkout_object(class approval_obs approve, int id, object player)
.EI
.SP 7 5

This method turns all the specified id into an item to be checked.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the object to create
.EP
.SP 9 5
player - the player to use for autoloading

.EP

.SI 3
* %^BOLD%^destroy_checkout_object%^RESET%^
.EI
.SI 5
int destroy_checkout_object(object ob)
.EI
.SP 7 5

This method updates the checked out item and changes to to be what
we now have...
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to destroy

.EP

.SI 3
* %^BOLD%^do_add_new_category%^RESET%^
.EI
.SI 5
int do_add_new_category(string category)
.EI
.SP 7 5

This method allows the player to add a new category to the list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
category - the new category name

.EP

.SI 3
* %^BOLD%^do_approve_auto_expression_add%^RESET%^
.EI
.SI 5
int do_approve_auto_expression_add(string expression, string value_str, int type, string list_name)
.EI
.SP 7 5

This method sets the automatic expressions to allow.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
expression - the expressions short to allow

.EP

.SI 3
* %^BOLD%^do_approve_auto_expression_remove%^RESET%^
.EI
.SI 5
int do_approve_auto_expression_remove(string idstr)
.EI
.SP 7 5

This method removes an allowed expression
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the expression id to remove

.EP

.SI 3
* %^BOLD%^do_approve_auto_high%^RESET%^
.EI
.SI 5
int do_approve_auto_high(string cost)
.EI
.SP 7 5

This method sets the automatic high cut off level.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cost - the cost to use a high cut off level

.EP

.SI 3
* %^BOLD%^do_approve_auto_high_deny%^RESET%^
.EI
.SI 5
int do_approve_auto_high_deny(string cost)
.EI
.SP 7 5

This method sets the automatic high cut off level for denying sales.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cost - the cost to use a high cut off level

.EP

.SI 3
* %^BOLD%^do_approve_auto_item_add%^RESET%^
.EI
.SI 5
int do_approve_auto_item_add(string item, string money, string list_name, int markup, string money_str)
.EI
.SP 7 5

This method sets the automatic items to allow.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
item - the items short to allow

.EP

.SI 3
* %^BOLD%^do_approve_auto_item_add_expr%^RESET%^
.EI
.SI 5
int do_approve_auto_item_add_expr(string item, string expr, string list_name, int markup, string money_str)
.EI
.SP 7 5

This method sets the automatic items to allow.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
item - the items short to allow

.EP

.SI 3
* %^BOLD%^do_approve_auto_item_add_object%^RESET%^
.EI
.SI 5
int do_approve_auto_item_add_object(object * obs, string money, string list_name, int markup, string money_str)
.EI
.SP 7 5

This method sets the automatic items to allow.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
item - the items short to allow

.EP

.SI 3
* %^BOLD%^do_approve_auto_item_add_object_expr%^RESET%^
.EI
.SI 5
int do_approve_auto_item_add_object_expr(object * obs, string expr, string list_name, int markup, string money_str)
.EI
.SP 7 5

This method sets the automatic items to allow.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
item - the items short to allow

.EP

.SI 3
* %^BOLD%^do_approve_auto_item_remove%^RESET%^
.EI
.SI 5
int do_approve_auto_item_remove(string item)
.EI
.SP 7 5

This method sets the automatic items to allow.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
item - the item to accept

.EP

.SI 3
* %^BOLD%^do_approve_auto_item_remove_object%^RESET%^
.EI
.SI 5
int do_approve_auto_item_remove_object(object * obs)
.EI
.SP 7 5

This method sets the automatic items to allow.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
item - the item to accept

.EP

.SI 3
* %^BOLD%^do_approve_auto_item_test%^RESET%^
.EI
.SI 5
int do_approve_auto_item_test(object * obs)
.EI
.SP 7 5

This method checks to see if the specified object can really be sold or
not automatically.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects to test

.EP

.SI 3
* %^BOLD%^do_approve_auto_low%^RESET%^
.EI
.SI 5
int do_approve_auto_low(string cost)
.EI
.SP 7 5

This method sets the automatic low acception level.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
cost - the cost to use a low level

.EP

.SI 3
* %^BOLD%^do_approve_black_list_add%^RESET%^
.EI
.SI 5
int do_approve_black_list_add(string name)
.EI
.SP 7 5

This method adds the person to the black list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to add to the black list

.EP

.SI 3
* %^BOLD%^do_approve_black_list_add_ignore%^RESET%^
.EI
.SI 5
int do_approve_black_list_add_ignore()
.EI
.SP 7 5

This method adds their ignore list to the black list

.EP

.SI 3
* %^BOLD%^do_approve_black_list_remove%^RESET%^
.EI
.SI 5
int do_approve_black_list_remove(string name)
.EI
.SP 7 5

This method removes the person from the black list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to remove from the black list

.EP

.SI 3
* %^BOLD%^do_approve_browse%^RESET%^
.EI
.SI 5
int do_approve_browse(string name)
.EI
.SP 7 5

This method browses the specified item.

.EP

.SI 3
* %^BOLD%^do_approve_buy_expression%^RESET%^
.EI
.SI 5
int do_approve_buy_expression(string str)
.EI
.SP 7 5

This method sets up the expression to use when calculating the
ability to buy from the shop.

.EP

.SI 3
* %^BOLD%^do_approve_category_change%^RESET%^
.EI
.SI 5
int do_approve_category_change(string name, string new_category)
.EI
.SP 7 5

This method changes the category of the item waiting to be approved.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the reference
.EP
.SP 9 5
new_category - the new category for the item

.EP

.SI 3
* %^BOLD%^do_approve_checkin%^RESET%^
.EI
.SI 5
int do_approve_checkin(object * obs)
.EI
.SP 7 5

This method checks in a checked out object.

.EP

.SI 3
* %^BOLD%^do_approve_checkout%^RESET%^
.EI
.SI 5
int do_approve_checkout(string name)
.EI
.SP 7 5

This method checks out the specified item.

.EP

.SI 3
* %^BOLD%^do_approve_item%^RESET%^
.EI
.SI 5
int do_approve_item(string name, string money_str)
.EI
.SP 7 5

This method approves the specified item.

.EP

.SI 3
* %^BOLD%^do_approve_limit%^RESET%^
.EI
.SI 5
int do_approve_limit(string limit)
.EI
.SP 7 5

This method sets the limit on number of items sellable by all the players.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
limit - the default limit

.EP

.SI 3
* %^BOLD%^do_approve_limit_person_items%^RESET%^
.EI
.SI 5
int do_approve_limit_person_items(string name, string value)
.EI
.SP 7 5

This method sets the limit per person to be a specified value.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to set a limit on
.EP
.SP 9 5
value - the limit to set

.EP

.SI 3
* %^BOLD%^do_approve_limit_person_remove%^RESET%^
.EI
.SI 5
int do_approve_limit_person_remove(string name)
.EI
.SP 7 5

This method removes any limits set on the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to remove the limit on

.EP

.SI 3
* %^BOLD%^do_approve_limit_person_value%^RESET%^
.EI
.SI 5
int do_approve_limit_person_value(string name, string money)
.EI
.SP 7 5

This method sets the limit at which a person can automatically sell
to the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to set a limit on
.EP
.SP 9 5
money - the limit to set it to

.EP

.SI 3
* %^BOLD%^do_approve_limit_person_value_deny%^RESET%^
.EI
.SI 5
int do_approve_limit_person_value_deny(string name, string money)
.EI
.SP 7 5

This method sets the limit at which a person can automatically sell
to the shop (deny limit).
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to set a limit on
.EP
.SP 9 5
money - the limit to set it to

.EP

.SI 3
* %^BOLD%^do_approve_name_change%^RESET%^
.EI
.SI 5
int do_approve_name_change(string name, string new_name)
.EI
.SP 7 5

This method changes the name of the item to be approved.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the reference
.EP
.SP 9 5
new_name - the new name for the item

.EP

.SI 3
* %^BOLD%^do_approve_pay_deficit%^RESET%^
.EI
.SI 5
int do_approve_pay_deficit()
.EI
.SP 7 5

This method pays off the cash deficit (if one exists).

.EP

.SI 3
* %^BOLD%^do_approve_percentage%^RESET%^
.EI
.SI 5
int do_approve_percentage(mixed num)
.EI
.SP 7 5

This method sets the percentage to add to the sell price.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the number to set the percentage to

.EP

.SI 3
* %^BOLD%^do_approve_reject%^RESET%^
.EI
.SI 5
int do_approve_reject(string name, string mess, int reject_pos)
.EI
.SP 7 5

This method rejects the specified item.

.EP

.SI 3
* %^BOLD%^do_approve_status%^RESET%^
.EI
.SI 5
int do_approve_status(int status)
.EI
.SP 7 5

This method shows the current status of the shop.

.EP

.SI 3
* %^BOLD%^do_approve_status_functions%^RESET%^
.EI
.SI 5
int do_approve_status_functions(int hints)
.EI
.SP 7 5

This method shows the status of all the user defined functions.
These can be used in other exciting functions.

.EP

.SI 3
* %^BOLD%^do_approve_status_internal_functions%^RESET%^
.EI
.SI 5
int do_approve_status_internal_functions()
.EI
.SP 7 5

This method shows the status of all the user defined functions.
These can be used in other exciting functions.

.EP

.SI 3
* %^BOLD%^do_approve_use_expression%^RESET%^
.EI
.SI 5
int do_approve_use_expression(string str)
.EI
.SP 7 5

This method sets up the expression to use when calculating the
ability to use the shop.

.EP

.SI 3
* %^BOLD%^do_check_sell%^RESET%^
.EI
.SI 5
int do_check_sell(object * obs)
.EI
.SP 7 5

This method runs through the objects and checks to see what sort of levels
they are allowed to sell them for to this shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_collect_partial_royalties%^RESET%^
.EI
.SI 5
int do_collect_partial_royalties(string amount)
.EI
.SP 7 5

This method collects royalities up to a certain amount.  This is
only usable by the owner of the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
amount - the amount to collect
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_function_help%^RESET%^
.EI
.SI 5
int do_function_help(string name)
.EI
.SP 7 5

Get help on a specific function.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the function to get help on

.EP

.SI 3
* %^BOLD%^do_function_help_list%^RESET%^
.EI
.SI 5
int do_function_help_list()
.EI
.SP 7 5

Get help on a specific function.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the function to get help on

.EP

.SI 3
* %^BOLD%^do_list_approval%^RESET%^
.EI
.SI 5
int do_list_approval(int approved)
.EI
.SP 7 5

List the items for approval.

.EP

.SI 3
* %^BOLD%^do_list_categories%^RESET%^
.EI
.SI 5
int do_list_categories()
.EI
.SP 7 5

This method lists the current categories and shows us which one is
currently the default.

.EP

.SI 3
* %^BOLD%^do_name_shop%^RESET%^
.EI
.SI 5
int do_name_shop(string name)
.EI
.SP 7 5

This method gives the shop a name.

.EP

.SI 3
* %^BOLD%^do_remove_category%^RESET%^
.EI
.SI 5
int do_remove_category(string category)
.EI
.SP 7 5

This method allows the player to remove a category from the list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
category - the category to remove

.EP

.SI 3
* %^BOLD%^do_remove_sign%^RESET%^
.EI
.SI 5
int do_remove_sign()
.EI
.SP 7 5

This method removes the sign from the shop.

.EP

.SI 3
* %^BOLD%^do_rename_category%^RESET%^
.EI
.SI 5
int do_rename_category(string category, string new_category)
.EI
.SP 7 5

This method allows the player to remove a category from the list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
category - the category to rename
.EP
.SP 9 5
new_category - the new name of a category

.EP

.SI 3
* %^BOLD%^do_return%^RESET%^
.EI
.SI 5
int do_return(string name)
.EI
.SP 7 5

This method returns the item to the person that started to sell it.
This only works before the item has been approved.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the item to return

.EP

.SI 3
* %^BOLD%^do_set_default_category%^RESET%^
.EI
.SI 5
int do_set_default_category(string default_cat)
.EI
.SP 7 5

This method sets the default category for the place.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
default_cat - the default category

.EP

.SI 3
* %^BOLD%^do_set_sign%^RESET%^
.EI
.SI 5
int do_set_sign(object * obs)
.EI
.SP 7 5

This method sets the sign you wish to use for your shop.
The sign will be displayed outside the shop for passer bys.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the sign

.EP

.SI 3
* %^BOLD%^do_stats_items%^RESET%^
.EI
.SI 5
int do_stats_items(int type)
.EI
.SP 7 5

This method shows the stats on all the sold items, the number and value
of each one sold.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
type - the type of sorting to do

.EP

.SI 3
* %^BOLD%^do_stats_money%^RESET%^
.EI
.SI 5
int do_stats_money()
.EI
.SP 7 5

This method shows stats about money and stuff in the shop.

.EP

.SI 3
* %^BOLD%^do_stats_transactions%^RESET%^
.EI
.SI 5
int do_stats_transactions()
.EI
.SP 7 5

This method shows a list of all the transactions that have occured in the
shop.

.EP

.SI 3
* %^BOLD%^do_user_function_add%^RESET%^
.EI
.SI 5
int do_user_function_add(string def, string expr)
.EI
.SP 7 5

This method allows a function to be added to the base set of user 
functions.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
def - the name and arguements
.EP
.SP 9 5
expr - the expression itself
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0  on failure

.EP

.SI 3
* %^BOLD%^do_user_function_remove%^RESET%^
.EI
.SI 5
int do_user_function_remove(string name)
.EI
.SP 7 5

This method allows a function to be added to the base set of user 
functions.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
def - the name and arguements
.EP
.SP 9 5
expr - the expression itself
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0  on failure

.EP

.SI 3
* %^BOLD%^inform_of_royalties%^RESET%^
.EI
.SI 5
void inform_of_royalties(object player)
.EI
.SP 7 5

Tells the playe they have some royalties to pick up.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the player to tell about the royalties

.EP

.SI 3
* %^BOLD%^is_allowed_to_use_shop%^RESET%^
.EI
.SI 5
int is_allowed_to_use_shop(string person)
.EI
.SP 7 5

This method checks to see if the shop is a closed shop and then checks
to see if the person is allowed.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to check to see if they are allowed
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are allowed, 0 if not

.EP

.SI 3
* %^BOLD%^is_checkout_object%^RESET%^
.EI
.SI 5
int is_checkout_object(object ob)
.EI
.SP 7 5

This method checks to see if the specified item is a checked out
item.
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
1 if it is, 0 if not

.EP

.SI 3
* %^BOLD%^is_item_approved%^RESET%^
.EI
.SI 5
mixed is_item_approved(string seller, int value, string name, object * sellables, string category, int ref final_cost)
.EI
.SP 7 5

This method checks to see if the specified item is approved.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
seller - the name of the seller
.EP
.SP 9 5
value - the amount it is sold for
.EP
.SP 9 5
name - the name of the item being solde
.EP
.SP 9 5
sellables - the sold items
.EP
.SP 9 5
category - the category of the sold item
.EP
.SP 9 5
final_cost - the final cost of the item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if is approved, 0 if not, -1 if denied, string if it is approved
        with a list name

.EP

.SI 3
* %^BOLD%^query_approval_class%^RESET%^
.EI
.SI 5
class approval_obs query_approval_class(string name)
.EI
.SP 7 5

This method returns the approval class from the string name.  THis is
assumed to be called from inside a command.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the string name
.EP
.SP 9 5
approved - from the approved or waiting list
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the approval object

.EP

.SI 3
* %^BOLD%^query_approval_list%^RESET%^
.EI
.SI 5
class approval_obs * query_approval_list(int approved)
.EI
.SP 7 5

This method returns the current approval list for the shop.

.EP

.SI 3
* %^BOLD%^query_letter_value%^RESET%^
.EI
.SI 5
string query_letter_value(int pos)
.EI
.SP 7 5

This method returns the position as a two letter value.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
pos - the position to mangle
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the letter value

.EP

.SI 3
* %^BOLD%^query_maximum_inventory_size%^RESET%^
.EI
.SI 5
int query_maximum_inventory_size()
.EI
.SP 7 5

This method returns the maximum inventory size for the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maximum inventory size

.EP

.SI 3
* %^BOLD%^query_maximum_sale_value%^RESET%^
.EI
.SI 5
int query_maximum_sale_value(string person, object * obs)
.EI
.SP 7 5

This method will return the maximum value at which the set of items
will be denied.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person selling the item
.EP
.SP 9 5
obs - the set of objects
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the deny value

.EP

.SI 3
* %^BOLD%^query_maximum_sale_value_both%^RESET%^
.EI
.SI 5
int * query_maximum_sale_value_both(string person, object item)
.EI
.SP 7 5

This method finds out the maximum value that this item is allowed
to be sold for based on the current settings of the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person doing the selling
.EP
.SP 9 5
items - the item being sold
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
({ queue limit, deny limit })

.EP

.SI 3
* %^BOLD%^query_maximum_waiting_queue_size%^RESET%^
.EI
.SI 5
int query_maximum_waiting_queue_size()
.EI
.SP 7 5

This method returns the maximum inventory size for the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the maximum inventory size

.EP

.SI 3
* %^BOLD%^query_number_of_items_listed%^RESET%^
.EI
.SI 5
int query_number_of_items_listed(string name)
.EI
.SP 7 5

This method returns the number of items currently in the shop by
the specified person.  This includes items waiting for approval.

.EP

.SI 3
* %^BOLD%^query_number_value%^RESET%^
.EI
.SI 5
int query_number_value(string letter)
.EI
.SP 7 5

This method goes from a letter value to a number.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
letter - the letters
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number

.EP

.SI 3
* %^BOLD%^query_owners_money%^RESET%^
.EI
.SI 5
int query_owners_money()
.EI
.SP 7 5

This method returns the current level of the owners money.

.EP

.SI 3
* %^BOLD%^query_parcel_post%^RESET%^
.EI
.SI 5
string query_parcel_post()
.EI
.SP 7 5

This method tells us the current parcel post used for rejecting items.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current parcel post

.EP

.SI 3
* %^BOLD%^query_round_prices%^RESET%^
.EI
.SI 5
int query_round_prices()
.EI
.SP 7 5

This method returns the amount to round all sell values for.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the round price

.EP

.SI 3
* %^BOLD%^query_royalty%^RESET%^
.EI
.SI 5
int query_royalty(string name)
.EI
.SP 7 5

This method returns the royalties for the specific person.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the name of the person to get the royalties of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the the royalties

.EP

.SI 3
* %^BOLD%^query_shop_name%^RESET%^
.EI
.SI 5
string query_shop_name()
.EI
.SP 7 5

This method returns the name of the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the shop

.EP

.SI 3
* %^BOLD%^query_sign_object%^RESET%^
.EI
.SI 5
object query_sign_object()
.EI
.SP 7 5

This method returns the current sign object for the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current sign object

.EP

.SI 3
* %^BOLD%^query_transaction_string%^RESET%^
.EI
.SI 5
string query_transaction_string(class shop_transaction trans)
.EI
.SP 7 5

This method truns a transaction into a string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
trans - the transaction
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the transaction as a string

.EP

.SI 3
* %^BOLD%^remove_from_approval_list%^RESET%^
.EI
.SI 5
void remove_from_approval_list(class approval_obs approve)
.EI
.SP 7 5

This method removes the item from the approval list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
approve - the item to remove

.EP

.SI 3
* %^BOLD%^return_all_checkedout_objects%^RESET%^
.EI
.SI 5
void return_all_checkedout_objects()
.EI
.SP 7 5

This method returns all the checkedout objects to the shop.

.EP

.SI 3
* %^BOLD%^set_maximum_inventory_size%^RESET%^
.EI
.SI 5
void set_maximum_inventory_size(int size)
.EI
.SP 7 5

This method sets the maximum inventory size for the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
size - the maximum inventory size

.EP

.SI 3
* %^BOLD%^set_maximum_waiting_queue_size%^RESET%^
.EI
.SI 5
void set_maximum_waiting_queue_size(int size)
.EI
.SP 7 5

This method sets the maximum inventory size for the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
size - the maximum inventory size

.EP

.SI 3
* %^BOLD%^set_parcel_post%^RESET%^
.EI
.SI 5
void set_parcel_post(string parcel)
.EI
.SP 7 5

This method sets the parcel post to use for rejecting items.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
parcel - the parcel post office

.EP

.SI 3
* %^BOLD%^set_round_price%^RESET%^
.EI
.SI 5
void set_round_price(int round)
.EI
.SP 7 5

THis method sets the amount to round all the sell values for.  THis is
deal with annoying currencies like AM money which does not have a
'1' item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
round - the price to round at

.EP

.SI 3
* %^BOLD%^set_shop_name%^RESET%^
.EI
.SI 5
void set_shop_name(string name)
.EI
.SP 7 5

This method sets the name of the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the shop

.EP

.SI 3
* %^BOLD%^set_sign_object%^RESET%^
.EI
.SI 5
void set_sign_object(object ob)
.EI
.SP 7 5

This method sets the current sign object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
sign - the new sign object

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^check_remove_category%^RESET%^
.EI
.SI 5
void check_remove_category(string answer, string category)
.EI
.SP 7 5

This method is called into to check to see if we can remove the category.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
answer - their answer
.EP
.SP 9 5
category - the category to remove

.EP

.SI 3
* %^BOLD%^confirm_approval%^RESET%^
.EI
.SI 5
void confirm_approval(string answer, class approval_obs approve, int final_cost)
.EI

.SI 3
* %^BOLD%^create_auto_load_file%^RESET%^
.EI
.SI 5
int create_auto_load_file(mixed ob, int fixed_num)
.EI
.SP 7 5

This method creates a save file for the specified objects autoloading
capability.  If the number to write to is non-null then it will
write to that object.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to get an autoload number for
.EP
.SP 9 5
fixed_num - the file number to write to
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the auto load number
.EP

.SI 3
* %^BOLD%^create_real_auto_load_object%^RESET%^
.EI
.SI 5
object create_real_auto_load_object(int num, object player)
.EI
.SP 7 5

This method creates a real object from the save file number.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the save file number
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the nice shiny new object
.EP


