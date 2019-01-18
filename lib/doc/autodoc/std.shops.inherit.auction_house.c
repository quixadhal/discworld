.DT
auction_house.c
Disk World autodoc help
auction_house.c

.SH Description
.SP 5 5

This inherit handles auctioning stuff.
.EP
.SP 10 5


Written by Pinkfish

Started Mon Apr 30 22:47:38 PDT 2001

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/inherit/open_shop and /std/shops/inherit/shop_event.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/room/auction.h, /include/mail.h, /include/player_handler.h, /include/move_failures.h, /include/player.h, /include/am_time.h, /include/shops/bank.h, /include/money.h and /include/playtesters.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^auction%^RESET%^
class auction {
mixed save_stuff;
int time_started;
int auction_length;
string name;
string seller;
string current_bidder;
int bid;
int reserve;
int retries;
string end_function;
mixed extra;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_item_to_auction%^RESET%^
.EI
.SI 5
int add_item_to_auction(object * obs, string name, int reserve, string seller, int length, string end_function, mixed extra)
.EI
.SP 7 5

This method puts an item up for auction.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects to sell
.EP
.SP 9 5
name - the name of the item
.EP
.SP 9 5
reserve - the reserve price of the item
.EP
.SP 9 5
seller - the seller of the item
.EP
.SP 9 5
length - the length of the auction
.EP
.SP 9 5
end_function - the function to call when the item is sold
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^bid_on_item%^RESET%^
.EI
.SI 5
void bid_on_item(class auction auct, int bid, string person)
.EI
.SP 7 5

This method makes a bid on an item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
item - the item to bid on
.EP
.SP 9 5
bid - the amount to bid
.EP
.SP 9 5
person - the person making the bid

.EP

.SI 3
* %^BOLD%^confirm_auction%^RESET%^
.EI
.SI 5
void confirm_auction(string check, string name, object * obs, int reserve, int len)
.EI
.SP 7 5
Confirm the addition 
.EP

.SI 3
* %^BOLD%^do_add%^RESET%^
.EI
.SI 5
int do_add(string name, object * obs, string res_str, string length)
.EI
.SP 7 5

This method adds an item for bidding.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name to put them up for bidding as
.EP
.SP 9 5
obs - the objects to auction
.EP
.SP 9 5
res_str - the reserve price
.EP
.SP 9 5
length - the length of the auction
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failed

.EP

.SI 3
* %^BOLD%^do_bid%^RESET%^
.EI
.SI 5
int do_bid(string id, string bid)
.EI
.SP 7 5

This method makes a bid on the specified item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the item to bid on
.EP
.SP 9 5
bid - the amount to bid
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^do_browse%^RESET%^
.EI
.SI 5
int do_browse(string id)
.EI
.SP 7 5

This method allows you to browse the item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the auction thing to browse

.EP

.SI 3
* %^BOLD%^do_claim%^RESET%^
.EI
.SI 5
int do_claim()
.EI
.SP 7 5

This method claims anything that you can bought and is waiting for
pickup.

.EP

.SI 3
* %^BOLD%^do_info%^RESET%^
.EI
.SI 5
int do_info()
.EI
.SP 7 5

This method shoes information about the auction house, like the
amount of time for each type of auction.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success

.EP

.SI 3
* %^BOLD%^do_list%^RESET%^
.EI
.SI 5
int do_list()
.EI
.SP 7 5

This method lists all the current auctions.

.EP

.SI 3
* %^BOLD%^do_withdraw%^RESET%^
.EI
.SI 5
int do_withdraw(string id)
.EI
.SP 7 5

This method withdraws the item from bidding.  You pay a penalty when you
do this, 10% of the current item cost.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id to withdraw

.EP

.SI 3
* %^BOLD%^finish_auction%^RESET%^
.EI
.SI 5
void finish_auction(class auction auction)
.EI
.SP 7 5

This method finishes the aution on the specified item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
auction - the auction item that is being finished

.EP

.SI 3
* %^BOLD%^force_finish_auction%^RESET%^
.EI
.SI 5
void force_finish_auction(string id)
.EI
.SP 7 5

This method forcibly finishes the auction on the specified item.
This is only to be used for debugging purposes.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
auction - the auction item that is being finished

.EP

.SI 3
* %^BOLD%^load_me%^RESET%^
.EI
.SI 5
void load_me()
.EI
.SP 7 5

This method loads the auction house up.

.EP

.SI 3
* %^BOLD%^next_auction_finish%^RESET%^
.EI
.SI 5
void next_auction_finish()
.EI
.SP 7 5

This method figures out when the next auction finishes and sets up
a timeout.

.EP

.SI 3
* %^BOLD%^query_auction_end_string%^RESET%^
.EI
.SI 5
string query_auction_end_string(class auction auction)
.EI
.SP 7 5

This method printsd a nice string showing how long till the auction
ends.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
auction - the auction to get an end string for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
auction end string

.EP

.SI 3
* %^BOLD%^query_auction_item%^RESET%^
.EI
.SI 5
class auction query_auction_item(string id)
.EI
.SP 7 5

This method finds the item based on some useful string.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id to lookup
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the auction item, 0 in the case of error

.EP

.SI 3
* %^BOLD%^query_auction_objects%^RESET%^
.EI
.SI 5
object * query_auction_objects(class auction auct)
.EI
.SP 7 5

This method finds the objects associated with the auction item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
auction - the auction item
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the objects associated with it

.EP

.SI 3
* %^BOLD%^query_auction_times%^RESET%^
.EI
.SI 5
mapping query_auction_times()
.EI
.SP 7 5

This method returns the length of auctions in seconds.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the length of auctions.

.EP

.SI 3
* %^BOLD%^query_auctions%^RESET%^
.EI
.SI 5
class auction * query_auctions()
.EI
.SP 7 5

This method returns the current list of available auctions.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current auctions

.EP

.SI 3
* %^BOLD%^query_finished_auctions%^RESET%^
.EI
.SI 5
class auction * query_finished_auctions()
.EI
.SP 7 5

This method returns the current list of finished auctions.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current auctions

.EP

.SI 3
* %^BOLD%^query_last_auction_finish_time%^RESET%^
.EI
.SI 5
int query_last_auction_finish_time()
.EI
.SP 7 5

This method figures out when the last auction will finish.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
when the last auction finished

.EP

.SI 3
* %^BOLD%^query_save_file%^RESET%^
.EI
.SI 5
string query_save_file()
.EI
.SP 7 5

This method queries the save file name.

.EP

.SI 3
* %^BOLD%^query_time_end_string%^RESET%^
.EI
.SI 5
string query_time_end_string(int left)
.EI
.SP 7 5

This method prints a nice string showing how long it is till then
end of the thing.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
left - the time to get the difference for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
time end string

.EP

.SI 3
* %^BOLD%^remove_auction_item%^RESET%^
.EI
.SI 5
void remove_auction_item(class auction auction)
.EI
.SP 7 5

This method removes the specified auction from the list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
auction - the auction item that is being removed

.EP

.SI 3
* %^BOLD%^remove_finished_auction_item%^RESET%^
.EI
.SI 5
void remove_finished_auction_item(class auction auction)
.EI
.SP 7 5

This method removes the specified auction from the list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
auction - the auction item that is being removed

.EP

.SI 3
* %^BOLD%^retry_auction%^RESET%^
.EI
.SI 5
void retry_auction(class auction auction)
.EI
.SP 7 5

This method retries the aution on the specified item.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
auction - the auction item that is being finished

.EP

.SI 3
* %^BOLD%^save_me%^RESET%^
.EI
.SI 5
void save_me()
.EI
.SP 7 5

This method saves the auction house.

.EP

.SI 3
* %^BOLD%^set_allowed_add%^RESET%^
.EI
.SI 5
void set_allowed_add(int flag)
.EI
.SP 7 5

This method sets the allowed add flag.  If theis flag is set to 0
then it is not possible for players to add items for
auctions.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
flag - the allowed add flag

.EP

.SI 3
* %^BOLD%^set_auction_time%^RESET%^
.EI
.SI 5
void set_auction_time(string index, int i)
.EI
.SP 7 5

This method sets the length of auctions in seconds.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
index - the name of the auction type to change
.EP
.SP 9 5
i - the length of auctions.

.EP

.SI 3
* %^BOLD%^set_check_can_bid%^RESET%^
.EI
.SI 5
void set_check_can_bid(function f)
.EI
.SP 7 5

This method sets a function to be called to check if someone can
bid or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
func - the function to call

.EP

.SI 3
* %^BOLD%^set_max_auctions%^RESET%^
.EI
.SI 5
void set_max_auctions(int i)
.EI
.SP 7 5

This method sets the maximum number of simultaneous auctions.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the number of items.

.EP

.SI 3
* %^BOLD%^set_max_retries%^RESET%^
.EI
.SI 5
void set_max_retries(int i)
.EI
.SP 7 5

This method sets the number of times an item will be retried before
the auction is failed.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the number of seconds the item will wait for someone to claim it.

.EP

.SI 3
* %^BOLD%^set_min_bid_increase%^RESET%^
.EI
.SI 5
void set_min_bid_increase(int i)
.EI
.SP 7 5

This method sets the minimum bid increase (as a percentage of the
current bid).

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - percentage

.EP

.SI 3
* %^BOLD%^set_retrieval_time%^RESET%^
.EI
.SI 5
void set_retrieval_time(int i)
.EI
.SP 7 5

This method sets the length of the retrieval time.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the number of seconds the item will wait for someone to claim it.

.EP

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
void set_save_file(string fname)
.EI
.SP 7 5

This method sets the save file name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the save file name

.EP


