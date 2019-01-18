.DT
craft_shop_category.c
Disk World autodoc help
craft_shop_category.c

.SH Description
.SP 5 5

This room will handle craft shop like things, where you sell stuff to
the store and it will sell it onto other players after taking
a margin.  This can be used for potions, pottery, swords, anything!
.EP
.SP 10 5


Written by Pinkfish

Started Thu Feb  5 15:39:57 CST 1998

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/inherit/craft_shop.
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
* %^BOLD%^add_shop_category%^RESET%^
.EI
.SI 5
void add_shop_category(string category)
.EI
.SP 7 5

This method adds a category into the accept list immediately.  This should
really only be used for initialy setting up the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
category - the name of the category

.EP

.SI 3
* %^BOLD%^do_change_category%^RESET%^
.EI
.SI 5
int do_change_category(object * obs, string new_category)
.EI

.SI 3
* %^BOLD%^do_list_category%^RESET%^
.EI
.SI 5
int do_list_category(string category)
.EI
.SP 7 5

This method lists a specific category of items.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
category - the category to list

.EP

.SI 3
* %^BOLD%^query_default_category%^RESET%^
.EI
.SI 5
string query_default_category()
.EI
.SP 7 5

This method returns the current default category for the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the default category

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
* %^BOLD%^query_real_category_name%^RESET%^
.EI
.SI 5
string query_real_category_name(string in_cat)
.EI
.SP 7 5

This correctly cases the name.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the correctly cases name, or 0 for failure

.EP

.SI 3
* %^BOLD%^remove_shop_category%^RESET%^
.EI
.SI 5
void remove_shop_category(string category)
.EI
.SP 7 5

This method forcibly removes a category from the accepted list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
category - the category to remove

.EP

.SI 3
* %^BOLD%^set_category_description%^RESET%^
.EI
.SI 5
void set_category_description(string category, string description)
.EI
.SP 7 5

This method sets the description of the category.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
category - the name of the category
.EP
.SP 9 5
description - the new description

.EP

.SI 3
* %^BOLD%^set_category_information%^RESET%^
.EI
.SI 5
void set_category_information(string category, string information)
.EI
.SP 7 5

This method sets the description of the category.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
category - the name of the category
.EP
.SP 9 5
info - the new information

.EP

.SI 3
* %^BOLD%^set_default_category%^RESET%^
.EI
.SI 5
void set_default_category(string category)
.EI
.SP 7 5

This method sets the default category for the item.  This must be setup
for something useful to happen.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
category - the default category

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
* %^BOLD%^update_categories%^RESET%^
.EI
.SI 5
void update_categories()
.EI
.SP 7 5

You should run this when you change the system to the category
based system.  It will go through and set anything without
a category to have a category.

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^choose_category%^RESET%^
.EI
.SI 5
void choose_category(string category, function func, int value, string name, object * sellables, string * categories)
.EI
.SP 7 5

This method chooses which category to set the item in.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
category - the category to choose
.EP
.SP 9 5
func - the function to call on exit
.EP
.SP 9 5
value - the current value of the item
.EP
.SP 9 5
name - the name of object
.EP
.SP 9 5
sellables - the items for sale

.EP

.SI 3
* %^BOLD%^setup_category%^RESET%^
.EI
.SI 5
void setup_category(function func, int value, string name, object * sellables)
.EI
.SP 7 5

This method sets a category on the sellables.  It is called from
within the craft shop code itself.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to call on exit
.EP
.SP 9 5
value - the current value of the item
.EP
.SP 9 5
name - the name of object
.EP
.SP 9 5
sellables - the items for sale

.EP


