.DT
pawn_shop.c
Disk World autodoc help
pawn_shop.c

.SH Description
.SP 5 5

This inherit is under development.  Use at your own risk.
All bug reports to me, please.

This is an inherit for pawn shops, which buy your items from you, but also
keep them for a short period of time for you to buy them back, before they
go into the shop's general inventory.

It is built around the general store inherit /std/shop.

.EP
.SP 10 5


Started 21st September 2002

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/shop.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h, /include/shops/bank.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_pawn%^RESET%^
.EI
.SI 5
int do_pawn(object * in_obs)
.EI
.SP 7 5

This handles pawning items to the shop.

.EP

.SI 3
* %^BOLD%^query_language%^RESET%^
.EI
.SI 5
string query_language()
.EI
.SP 7 5

This returns the language that the shop is using for receipts.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
[string] The language receipts are written in.

.EP

.SI 3
* %^BOLD%^query_pawn_markup%^RESET%^
.EI
.SI 5
int query_pawn_markup()
.EI
.SP 7 5

This returns the percentage that the shop will mark reclaim prices up by.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
[int] The percentage to add to reclaim prices.

.EP

.SI 3
* %^BOLD%^query_pawn_mess%^RESET%^
.EI
.SI 5
string query_pawn_mess()
.EI
.SP 7 5

This returns the message given out when somebody pawns something.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
[string] The message given when something is successfully pawned.

.EP

.SI 3
* %^BOLD%^query_pawn_ttl%^RESET%^
.EI
.SI 5
int query_pawn_ttl()
.EI
.SP 7 5

This returns the time to live of pawned items, in seconds.  That is, the
length of time between an item's being pawned and the shop selling that
item to recoup its costs.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
[int] The time in seconds before the shop will sell a pawned item.

.EP

.SI 3
* %^BOLD%^set_language%^RESET%^
.EI
.SI 5
void set_language(string lang)
.EI
.SP 7 5

This sets the language that the shop will use for receipts.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - The language to write receipts in.

.EP

.SI 3
* %^BOLD%^set_pawn_markup%^RESET%^
.EI
.SI 5
void set_pawn_markup(int percent)
.EI
.SP 7 5

This sets the percentage that the shop will mark reclaim prices up by.
The default is 20%.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
percent - The percentage to add to reclaim prices.

.EP

.SI 3
* %^BOLD%^set_pawn_mess%^RESET%^
.EI
.SI 5
void set_pawn_mess(mixed mess)
.EI
.SP 7 5

This sets the message given out when somebody pawns something.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - The message given when something is successfully pawned.

.EP

.SI 3
* %^BOLD%^set_pawn_ttl%^RESET%^
.EI
.SI 5
void set_pawn_ttl(int ttl)
.EI
.SP 7 5

This sets the time to live of pawned items, in seconds.  That is, the
length of time between an item being pawned and the shop selling that
item to recoup its costs.  The default is one week realtime.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ttl - The time in seconds before the shop will sell a pawned item.

.EP


