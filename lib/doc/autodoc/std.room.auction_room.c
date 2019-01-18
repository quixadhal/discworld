.DT
auction_room.c
Disk World autodoc help
auction_room.c

.SH Description
.SP 5 5

Auction Room Inheritable
This room will give you everything you need to make an auction
room.
.EP
.SP 10 5


Written by Terano

Started 3 October, 1998
.EP

.SH Change history
.SP 5 5
24 March, 1999 Many exciting things. Terano.

2 October, 1999 Added stability! Excitement! 
An excessively complicated browse function! Terano.

8 January, 2000! Made Y2K compliant! (Just kidding)
Added some advertising stuff.
Fixed a missing case as well.

11 January, 2000 Fixed some advertising stuff.

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room and /global/auto_load.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/move_failures.h, /include/shops/bank.h, /include/mail.h and /include/money.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^item%^RESET%^
class item {
string name;
string * alias;
string * adjectives;
string long;
string read_mess;
string enchant_mess;
}

.EI
.SP 7 5

In this new version, we aren't keeping the objects loaded,
because the auto_load code seems flaky outside of 
/global/player.c
Instead keep a whole bunch of info about it on file.

.EP

.SI 3
* %^BOLD%^lot%^RESET%^
class lot {
int case_code;
class store assoc_store;
string seller;
string status;
int expiration_time;
int reserve_price;
mapping bids;
mixed * lot_autoload;
mixed * current_bid;
string * auto_bids;
string notes;
string * excluded;
}

.EI
.SP 7 5

A few notes about this class, I didn't like the way the old one
worked, when a lot moved to expired_lots, there was no real way to recover it
Instead, we will keep one array of all lots, with differing status's depending
on whats happening.
IE: Open: Accepting bids.
    Waiting: Seller: It did not take any acceptable bids and it is waiting collection
      by the seller.
    Waiting: Buyer: A bid was taken and we are waiting for payment.
    Waiting: Buyer (lapsed): A bid was taken, but lapsed. There are no 
      other bidders, so in the next cycle the seller will be mailed and
      status changed to Waiting: Seller.
    Waiting Creator Intervention:  A lot has fallen over in some way,
      and needs a creator to fix it.
    Closed: The bid has been collected (by either buyer or seller),
      lots will stay in this status for 2 hours after collection.
      Then payment will be credited to the payment array and the lot will 
      be deleted.
Case code is used in reference to the store class.
The rest are the same as they were in the last version

.EP

.SI 3
* %^BOLD%^store%^RESET%^
class store {
int case_code;
string colour;
string inv_string;
class item * inventory;
}

.EI
.SP 7 5

In this new version, we aren't keeping the objects loaded,
because the auto_load code seems flaky outside of 
/global/player.c
Instead keep a whole bunch of info about it on file.

.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^adjust_money%^RESET%^
.EI
.SI 5
void adjust_money(int amount, object player)
.EI

.SI 3
* %^BOLD%^automated_deposit%^RESET%^
.EI
.SI 5
int automated_deposit(object * things, int auction_length, string function_cb, string object_cb, int res_price, string int_desc)
.EI
.SP 7 5

This function allows an object to deposit an item for auction,
Things are the items to auction, auction length is the length in seconds,
function_cb and object_cb are function names and object paths to allow a
call back to be scheduled for when the auction is over, res price is
the reserve price and int_desc is a description that will be written on
the case. If an error code is generated, it is upon the object calling
this function to pull those objects back. An object reference isn't
kept here.

.EP

.SI 3
* %^BOLD%^do_bid%^RESET%^
.EI
.SI 5
int do_bid(string offer, object * boxes)
.EI
.SP 7 5

Basic add command function.. lets you bid on stuff

.EP

.SI 3
* %^BOLD%^do_browse%^RESET%^
.EI
.SI 5
int do_browse(object * boxes, string target, int identifier)
.EI
.SP 7 5

This function will be a bit horrible, but it is a nicer way of doing it 
then keeping the objects around.
This function uses Terano's cheap and dirty parser (All rights reserved).
To be added - plural support! (using query_plural)

.EP

.SI 3
* %^BOLD%^do_collect%^RESET%^
.EI
.SI 5
int do_collect(string pattern)
.EI

.SI 3
* %^BOLD%^do_deposit%^RESET%^
.EI
.SI 5
int do_deposit(object * things, int auction_time, string time_type, string res_price)
.EI

.SI 3
* %^BOLD%^do_describe%^RESET%^
.EI
.SI 5
int do_describe(object * boxes, string int_desc)
.EI

.SI 3
* %^BOLD%^do_exclude%^RESET%^
.EI
.SI 5
int do_exclude(object * boxes, string excluded)
.EI

.SI 3
* %^BOLD%^do_list%^RESET%^
.EI
.SI 5
int do_list()
.EI

.SI 3
* %^BOLD%^do_unexclude%^RESET%^
.EI
.SI 5
int do_unexclude(object * boxes, string excluded)
.EI

.SI 3
* %^BOLD%^do_withdraw%^RESET%^
.EI
.SI 5
int do_withdraw(object * boxes)
.EI
.SP 7 5

Withdraw an item from bidding

.EP

.SI 3
* %^BOLD%^extra_look%^RESET%^
.EI
.SI 5
string extra_look(object ob)
.EI

.SI 3
* %^BOLD%^generate_advertising_string%^RESET%^
.EI
.SI 5
string generate_advertising_string(class lot lot)
.EI
.SP 7 5

This function generates a string that can be used in any advertising
you might want to do, it returns a human friendly string that passes
information about the lot. (Specifically - contents, cost and exp time)
It takes a lot as an arg.

.EP

.SI 3
* %^BOLD%^generate_random_adv_string%^RESET%^
.EI
.SI 5
string generate_random_adv_string()
.EI
.SP 7 5

A more usable version of above function, call it and it picks a lot
at random and generates a string for it.
returns "NULL" if there are no lots.
Also returns NULL if the lot chosen isnt open

.EP

.SI 3
* %^BOLD%^generate_store%^RESET%^
.EI
.SI 5
class store generate_store(object * things, int lot_id)
.EI
.SP 7 5

Return a store class with info on the objects

.EP

.SI 3
* %^BOLD%^load_file%^RESET%^
.EI
.SI 5
void load_file()
.EI

.SI 3
* %^BOLD%^lot_check%^RESET%^
.EI
.SI 5
void lot_check()
.EI
.SP 7 5

A major component, this function is called once every few minutes to
change the state of our bids. It puts new bids out on display, and 
modifies the state of in game lots as required. It also removes old ones.

.EP

.SI 3
* %^BOLD%^query_payments%^RESET%^
.EI
.SI 5
mapping query_payments()
.EI

.SI 3
* %^BOLD%^recover_lot%^RESET%^
.EI
.SI 5
mixed recover_lot(mixed lot_autoload)
.EI

.SI 3
* %^BOLD%^save_file%^RESET%^
.EI
.SI 5
void save_file()
.EI

.SI 3
* %^BOLD%^set_admin_cost%^RESET%^
.EI
.SI 5
void set_admin_cost(int value)
.EI

.SI 3
* %^BOLD%^set_commission%^RESET%^
.EI
.SI 5
void set_commission(int percent)
.EI

.SI 3
* %^BOLD%^set_currency%^RESET%^
.EI
.SI 5
void set_currency(string cur)
.EI

.SI 3
* %^BOLD%^set_save_path%^RESET%^
.EI
.SI 5
void set_save_path(string path)
.EI

.SI 3
* %^BOLD%^set_shop_name%^RESET%^
.EI
.SI 5
void set_shop_name(string _name)
.EI


