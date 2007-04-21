.DT
peddler.c
DW_wibble autodoc help
peddler.c

.SH Description
.SP 5 5

This is the peddler object, it acts as a walking talking item
shop.
Added event_enter so to stop food from decaying when the peddler
sells it. Siel, October 2001


.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /obj/monster and /std/shops/inherit/clone_on_demand.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/money.h, /include/move_failures.h, /include/armoury.h, /include/virtual.h and /include/shops/bank.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^cost_string%^RESET%^
.EI
.SI 5
string cost_string(object thing, string place, object buyer)
.EI
.SP 7 5

This method returns the costs of the item as a string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing to buy
.EP
.SP 9 5
place - the money area it is being bought in
.EP
.SP 9 5
buyer - the person who is buying the object
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string money value

.EP

.SI 3
* %^BOLD%^do_browse%^RESET%^
.EI
.SI 5
int do_browse(mixed indirect_obs, string dir_match, string indir_match, string * words)
.EI
.SP 7 5

The main entrace to the browse for things command.
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
int do_buy(mixed indirect_obs, string dir_match, string indir_match, string * words)
.EI
.SP 7 5

The main entrace to the buy things command.
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

The main entrace to the list stuff command.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^event_enter%^RESET%^
.EI
.SI 5
void event_enter(object ob, string mess, object from)
.EI

.SI 3
* %^BOLD%^event_exit%^RESET%^
.EI
.SI 5
void event_exit(object ob, string message, object to)
.EI

.SI 3
* %^BOLD%^query_cost%^RESET%^
.EI
.SI 5
int query_cost(object thing, object buyer)
.EI
.SP 7 5

This method returns the cost of the specified item to the buyer.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the thing to cost
.EP
.SP 9 5
buyer - who is buying it
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the cost of the item

.EP

.SI 3
* %^BOLD%^sell_thing%^RESET%^
.EI
.SI 5
void sell_thing(object player, object thing)
.EI
.SP 7 5

The main entrace to the sell stuff command.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^set_all_out_mess%^RESET%^
.EI
.SI 5
void set_all_out_mess(string s)
.EI
.SP 7 5

Overrides the default chat of the item being out of stock when a player
tries to buy it
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the chat to replace the default with.  Must include $item$, which
will be replaced by the item short.

.EP

.SI 3
* %^BOLD%^set_busy_mess%^RESET%^
.EI
.SI 5
void set_busy_mess(string s)
.EI
.SP 7 5

Overrides the default chat of the peddler being busy selling
something already.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the chat to replace the default with

.EP

.SI 3
* %^BOLD%^set_dont_sell_mess%^RESET%^
.EI
.SI 5
void set_dont_sell_mess(string s)
.EI
.SP 7 5

Overrides the default chat of the peddler not selling an item
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the chat to replace the default with.  Must include $item$, which
will be replaced by the item short.

.EP

.SI 3
* %^BOLD%^set_in_combat_mess%^RESET%^
.EI
.SI 5
void set_in_combat_mess(string s)
.EI
.SP 7 5

Overrides the default chat of the peddler being in combat and hence unable
to sell his wares!
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the chat to replace the default with

.EP

.SI 3
* %^BOLD%^set_list_item_mess%^RESET%^
.EI
.SI 5
void set_list_item_mess(string s)
.EI
.SP 7 5

Overrides the default chat of listing a stock item's price and stock
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the chat to replace the default with.  Must include $num_left$,
$display$ and $cost$, which will be replaced by the appropriate
strings.

.EP

.SI 3
* %^BOLD%^set_list_mess%^RESET%^
.EI
.SI 5
void set_list_mess(string s)
.EI
.SP 7 5

Overrides the default chat to announcing the stock list
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the chat to replace the default with

.EP

.SI 3
* %^BOLD%^set_missing_punter_mess%^RESET%^
.EI
.SI 5
void set_missing_punter_mess(string s)
.EI
.SP 7 5

Overrides the default chat to the player having wandered off
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the chat to replace the default with

.EP

.SI 3
* %^BOLD%^set_no_stock_mess%^RESET%^
.EI
.SI 5
void set_no_stock_mess(string s)
.EI
.SP 7 5

Overrides the default reply to having no listable stock
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the chat to replace the default with

.EP

.SI 3
* %^BOLD%^set_none_left_mess%^RESET%^
.EI
.SI 5
void set_none_left_mess(string s)
.EI
.SP 7 5

Overrides the default reply to having none left
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the chat to replace the default with

.EP

.SI 3
* %^BOLD%^set_out_of_item_mess%^RESET%^
.EI
.SI 5
void set_out_of_item_mess(string s)
.EI
.SP 7 5

Overrides the default chat of being out of an item that is browsed
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the chat to replace the default with.  Must include $item$, which
will be replaced by the item short.

.EP

.SI 3
* %^BOLD%^set_show_item_mess%^RESET%^
.EI
.SI 5
void set_show_item_mess(string s)
.EI
.SP 7 5

Overrides the default chat of showing an item to a player
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the chat to replace the default with.  Must include $item$ and
$cost$, which will be replaced by the item short and the cost of the item
respectively.

.EP

.SI 3
* %^BOLD%^set_too_heavy_mess%^RESET%^
.EI
.SI 5
void set_too_heavy_mess(string s)
.EI
.SP 7 5

Overrides the default chat of the player being too burdened to carry an
item
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the chat to replace the default with.  Must include $item$, which
will be replaced by the item short.

.EP

.SI 3
* %^BOLD%^set_too_poor_mess%^RESET%^
.EI
.SI 5
void set_too_poor_mess(string s)
.EI
.SP 7 5

Overrides the default chat of the player being too poor to buy an item
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the chat to replace the default with.  Must include $item$, which
will be replaced by the item short.

.EP


