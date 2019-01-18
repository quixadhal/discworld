.DT
pub.c
Disk World autodoc help
pub.c

.SH Description
.SP 5 5

This is the inheritable bit of the pub.  The actual file you should
inherit is /std/shops/pub_shop.c


Based on the original "pub.c" code, this version allows you to buy an actual drink (inna glass) instead of just forcefeeding you with whatever you purchase. You can also create food and serve it on plates, in boxes, or whatever. 

The items on sale are either cloned from the armoury, cloned from a file or cloned in the "create_item()" code in the pub code itself. Containers for these items are done in the same way, using the "create_container()" function. This is the same as the "create_object()" system in "item_shop.c". You can also buy items for other people (or groups of other people) in the pub. 

There are several standard glasses and plates available for drinks and food in the "pub_shop.h" header file, if you don't want to create the glasses and plates yourself. These plates use the "/obj/misc/plate.c" inheritable file. 

A "counter" will automatically be placed in the pub. If any item cannot be moved into the person who ordered it (i.e they are carrying too much ) it will be placed on the counter. Empty glasses and so on will be cleared from the counter after 3 seconds. To ensure that this happens, make sure that the container object you use has the property "pub item" added to it. Items will only be cleared if they are empty.
.EP
.SP 10 5


Written by Lemming

Started 23/10/1999
.EP



.SH See also
.SP 5 5
/std/shops/pub_shop.c, /include/shops/pub_shop.h and /include/volumes.h

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/inherit/shop_event and /std/shops/inherit/open_shop.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h, /include/shops/bank.h, /include/armoury.h, /include/map.h, /include/money.h and /include/shops/pub_shop.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_menu_alias%^RESET%^
.EI
.SI 5
void add_menu_alias(mixed alias, string actual)
.EI
.SP 7 5

This method allows you to add an alias to an item sold in the pub.  Many
aliases are added by default so you shouldn't have to use this too
often.  See the help on "set_no_standard_alias()" to see what aliases are
added automatically.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
alias - the alias to add
.EP
.SP 9 5
alias - the real item that the alias refers to
.EP

.SI 3
* %^BOLD%^add_menu_aliases%^RESET%^
.EI
.SI 5
void add_menu_aliases(string * aliases, string actual)
.EI
.SP 7 5

This method allows you to add multiple aliases at once.  Many aliases are
added by default so you shouldn't have to use this too often.  See the 
help on "set_no_standard_alias()" to see what aliases are added 
automatically.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
aliases - an array of aliases to add
.EP
.SP 9 5
alias - the real item that the aliases refer to
.EP

.SI 3
* %^BOLD%^add_menu_item%^RESET%^
.EI
.SI 5
varargs void add_menu_item(string name, int type, int cost, string item, string container, int volume, int intox)
.EI
.SP 7 5

This is the method used to add items to the menu.  Only the first four
parameters are required, the rest are optional.


The "type" parameter is used to determine which section of the menu the item should reside in, and should be selected from those in the "pub_shop.h" header file. 

The "item" parameter is used to generate the actual product on sale. This value can be: 
.EP
.SO 8 2 -12

	*	a name, which is passed to the "create_item()" function in the room code 
	*	a filename, which is cloned, or 
	*	an armoury identifier, which is passed to the armoury handler. 
.EO
.SP 7 5


The "container" parameter is the same as the "item" parameter, except that it refers to the container the item comes in and is passed to "create_container()" instead of "create_item()". You can also use the standard containers listed in the "pub_shop.h" header file. It is optional, and setting it to 0 will cause it to be ignored. 

The "volume" parameter is optional and is passed directly to "set_amount()" on the object cloned from "item". It can be used to alter the volume of a liquid cloned from a file, so the file itself does not have to be changed. It is optional, and setting it to 0 will cause it to be ignored. If this is set to 0 then the item will fill up the container. Standard volume definitions can be found in the "volumes.h" header file. 

The last parameter, "intox", is optional and is used only by NPCs to determine how alcoholic an item is. This should be between 0 and 10, with 0 being non-alcoholic and 10 being something like Suicider. It defaults to 0. Note: This has no effect on the actual alcohol content of the item. 

The different volumes of standard containers can be taken from /include/volumes.h which has defines for all the standard volumes.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the description to be displayed on the menu
.EP
.SP 9 5
type - the type of item
.EP
.SP 9 5
cost - the cost of the item
.EP
.SP 9 5
item - the name, filename or armoury identifier for the item itself
.EP
.SP 9 5
container - the name, filename or armoury identifier for the
container (optional)
.EP
.SP 9 5
volume - the volume that the item should be set to (optional)
.EP
.SP 9 5
intox - the intoxification value, on a scale of 0 to 10 (optional)
.EP

.SI 3
* %^BOLD%^make_counter%^RESET%^
.EI
.SI 5
object make_counter()
.EI
.SP 7 5

This function creates the counter for the pub.  It defaults to cloning
PUB_COUNTER_FILE and making it a hidden object.
If you are creating your own counter object then making it hidden is a
good idea.  If it's not hidden, make sure it at least cannot be moved or
buried.
The counter should clear empty objects with the "pub item" property if
they are placed on it.  Making it clear non-empty objects is a bad idea,
since items will be placed here if the purchaser is unable to carry
them.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the new counter object
.EP

.SI 3
* %^BOLD%^query_counter%^RESET%^
.EI
.SI 5
object query_counter()
.EI
.SP 7 5

This method returns the object currently being used as a counter in the
pub.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the file name of the counter
.EP

.SI 3
* %^BOLD%^query_discount%^RESET%^
.EI
.SI 5
float query_discount(object ob)
.EI
.SP 7 5

This function can be masked and used to determine a discount that is
applied to all items sold, so for instance you could check the guild of
'ob' and give a discount to Witches, or something.  By default the
discount is zero, so query_discount returns 1.0
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object doing the buying
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a float to multiply the price by
.EP

.SI 3
* %^BOLD%^query_display_header%^RESET%^
.EI
.SI 5
int query_display_header()
.EI
.SP 7 5

This method returns a flag stating whether display of the menu header
is on or off.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for header, 0 for no header
.EP

.SI 3
* %^BOLD%^query_display_subheadings%^RESET%^
.EI
.SI 5
int query_display_subheadings()
.EI
.SP 7 5

This method returns a flag stating whether display of the menu subheadings
is on or off.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for headings, 0 for no headings
.EP

.SI 3
* %^BOLD%^query_language%^RESET%^
.EI
.SI 5
string query_language()
.EI
.SP 7 5

This method returns the language used in the shop.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the language used in the shop

.EP

.SI 3
* %^BOLD%^query_menu%^RESET%^
.EI
.SI 5
object query_menu()
.EI
.SP 7 5

This method returns the object currently being used as a menu in the
pub.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the file name of the menu

.EP

.SI 3
* %^BOLD%^query_menu_aliases%^RESET%^
.EI
.SI 5
mapping query_menu_aliases()
.EI
.SP 7 5

This method returns a list of all the aliases currently available in
the pub.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
alias : real name
.EP

.SI 3
* %^BOLD%^query_menu_header%^RESET%^
.EI
.SI 5
string query_menu_header()
.EI
.SP 7 5

This method returns the current menu header text.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the menu header text
.EP

.SI 3
* %^BOLD%^query_menu_items%^RESET%^
.EI
.SI 5
mapping query_menu_items()
.EI
.SP 7 5

This method returns a list of all the items currently available in
the pub.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
items available, listing type, price and intoxification value
.EP

.SI 3
* %^BOLD%^query_menu_subheadings%^RESET%^
.EI
.SI 5
string * query_menu_subheadings()
.EI
.SP 7 5

This method returns the current menu subheading text.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the menu subheadings
.EP

.SI 3
* %^BOLD%^query_no_standard_alias%^RESET%^
.EI
.SI 5
int query_no_standard_alias()
.EI
.SP 7 5

This method returns a flag stating whether standard aliases will be added
or not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if standard aliases will be added, 1 if not
.EP

.SI 3
* %^BOLD%^query_pub%^RESET%^
.EI
.SI 5
int query_pub()
.EI
.SP 7 5

This method checks to see if this is a pub.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
always return 1

.EP

.SI 3
* %^BOLD%^read%^RESET%^
.EI
.SI 5
string read()
.EI
.SP 7 5

This method produces the menu from the item information, with the menu
header at the top, all items available grouped by type and sorted by
cost.  If you don't want the menu printed this way then mask this
function and return your own.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the menu text
.EP

.SI 3
* %^BOLD%^remove_menu_alias%^RESET%^
.EI
.SI 5
int remove_menu_alias(string alias)
.EI
.SP 7 5

This method allows you to remove an alias from those currently available
in the pub.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
alias - the alias to remove from the list
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 if unsuccessful
.EP

.SI 3
* %^BOLD%^remove_menu_item%^RESET%^
.EI
.SI 5
int remove_menu_item(string name)
.EI
.SP 7 5

This method allows you to remove an item from those currently available
in the pub.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the item to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 if unsuccessful
.EP

.SI 3
* %^BOLD%^set_display_header%^RESET%^
.EI
.SI 5
void set_display_header(int value)
.EI
.SP 7 5

This method allows you to switch the menu header (defaults to "The menu
reads:") that appears at the top of the menu on and off.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
value - set to 1 to display header or 0 to remove them
.EP

.SI 3
* %^BOLD%^set_display_subheadings%^RESET%^
.EI
.SI 5
void set_display_subheadings(int value)
.EI
.SP 7 5

This method allows you to switch the subheadings ("Alcoholic Beverages",
"Meals", etc) that appear above different types of items on and off.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
value - set to 1 to display subheadings or 0 to remove them
.EP

.SI 3
* %^BOLD%^set_language%^RESET%^
.EI
.SI 5
void set_language(string language)
.EI
.SP 7 5

This method sets the language to use in the shop.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
language - the language to use

.EP

.SI 3
* %^BOLD%^set_menu_header%^RESET%^
.EI
.SI 5
void set_menu_header(string header)
.EI
.SP 7 5

This method sets the header that appears at the top of the menu.  By
default this is "The menu reads:".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
header - the new menu header
.EP

.SI 3
* %^BOLD%^set_menu_subheadings%^RESET%^
.EI
.SI 5
void set_menu_subheadings(int subheading, string text)
.EI
.SP 7 5

This method sets the subheadings that appear at the top of the menu.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
subheading - the subheading to change (use the #defines listed in
"pub_shop.h")
.EP
.SP 9 5
text - the new menu subheading text
.EP

.SI 3
* %^BOLD%^set_no_standard_alias%^RESET%^
.EI
.SI 5
void set_no_standard_alias(int flag)
.EI
.SP 7 5

This method allows you to turn on or off the addition of standard aliases
when new menu items are added.  By default it is turned on.  Standard
aliases are added as follows:
If you added an item called "Lancre vintage wine" the aliases added would
be:

.EP
.SO 8 2 -12

	*	vintage wine 
	*	lancre wine 
	*	wine 
.EO
.SP 7 5
An alias of the name in lowercase is always added regardless of whether or not this flag is turned on or off. You may wish to turn this off if you are adding several items which could be mistaken for each other, for instance "Lancre beer" and "Morporkian beer".
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
flag - 0 if standard aliases should be added, 1 if not
.EP


