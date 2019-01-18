.DT
potion_shop.c
Disk World autodoc help
potion_shop.c

.SH Description
.SP 5 5
 
This room is a craft shop which handles potions and salves.
Do not forget important things like set_save_dir() and
set_theft_handler().
.EP
.SP 10 5


Written by Gruper

Started Thu Oct 29 1998
.EP



.SH See also
.SP 5 5
help::craft_shop

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/craft_shop_category.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^is_potion_or_salve%^RESET%^
.EI
.SI 5
int is_potion_or_salve(object ob)
.EI
.SP 7 5

This is the default function for checking if something can
be sold in a potion shop.  If you override this, there is no
reason to inherit this file, so you can't.  It will accept any
object that contains something which has an eat effect or an
apply effect.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - The object that someone is trying to sell to the shop
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
success 1 if the object can be sold, 0 if not

.EP

.SI 3
* %^BOLD%^query_message_function%^RESET%^
.EI
.SI 5
function query_message_function()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Pointer to the message function

.EP

.SI 3
* %^BOLD%^query_shopkeeper%^RESET%^
.EI
.SI 5
string query_shopkeeper()
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
The name of the shopkeeper

.EP

.SI 3
* %^BOLD%^set_message_function%^RESET%^
.EI
.SI 5
void set_message_function(function wossname)
.EI
.SP 7 5

This method sets the message function for the shop.
The message function can be useful to handle messages to
the player via the shopkeeper. The function should be of
type string -> void.  If message_function is not set, we
default to a rather nice function.  If message_function
is explicitly set to 0, no message will be given apart from
the standard messages from craft_shop.  It is called from
is_potion_or_salve() and can also be used in do_buy() etc.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
wossname - A pointer to the function
.EP

.SI 3
* %^BOLD%^set_shopkeeper%^RESET%^
.EI
.SI 5
void set_shopkeeper(string name)
.EI
.SP 7 5

This method sets the %^BOLD%^name %^RESET%^of the shopkeeper that the shop will look for when giving messages to the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - A name that identifies the shopkeeper to find_match

.EP

.SI 3
* %^BOLD%^shopkeeper_say%^RESET%^
.EI
.SI 5
void shopkeeper_say(string message)
.EI
.SP 7 5

This is the default message function.
If the shopkeeper is in the shop, they will try to say message.

.EP


