.DT
auction.h
Disk World autodoc help
auction.h

.SH Description
.SP 5 5

The auction house include file.
.EP
.SP 10 5


Written by Pinkfish

Started Tue May  1 01:26:28 PDT 2001

.EP

.SH Defines
.SI 3
* %^BOLD%^AUCTION_CLAIM_PHASE%^RESET%^
.EI
.SP 7 5

This define is used in the function for the item for the
claim the item auction phase.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_ADD_ABORT%^RESET%^
.EI
.SP 7 5

Ths event when the player has aborted adding an auction to the shop.
The extra arguments are the name to sell it as, the objects to sell,
the reserve code and the length of the auction.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_ADD_AUCTION%^RESET%^
.EI
.SP 7 5

The event when the player is about to try and add something into the shop.
The extra arguments are the name to sell it as, the objects to sell,
the reserve code and the length of the auction and the generated
default string.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_BAD_ITEM%^RESET%^
.EI
.SP 7 5

The item index was bad, unable to find the item. The extra argument is the
id that was bad.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_BAD_MONEY%^RESET%^
.EI
.SP 7 5

The event when the player is giving a bad money spec.   The
extra arguments are the name to sell it as, the objects to sell,
the reserve code and the length of the auction.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_BID%^RESET%^
.EI
.SP 7 5

The even when a bid is made.  The extra arguements are the auction
class and the bid made.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_BID_BELOW_RESERVE%^RESET%^
.EI
.SP 7 5

The event when the bid is below the reserve.  The extra arguements are the
auction class, the value of the placed bid, the reserve.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_BID_TOO_SMALL%^RESET%^
.EI
.SP 7 5

The event when the bid is too small.  The extra arguements are the
auction class, the value of the placed bid, the new minimum bid.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_BROWSE%^RESET%^
.EI
.SP 7 5

The event for browsing a specific item for auction.  The extra arguments
are the auction class, the objects and the generated string.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_CANNOT_WITHDRAW%^RESET%^
.EI
.SP 7 5

The event when an item is unable to be withdrawn.  The extra arguements are
the auction class of the withdrawn item.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_CLAIM%^RESET%^
.EI
.SP 7 5

The event when an item is to be claimed.  The extra arguements are
the auction class of the claimed item, the bid and the objects claimed.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_CONFIRM_AUCTION%^RESET%^
.EI
.SP 7 5

Ths event when the player has completely added the auction to the shop.
The extra arguments are the name to sell it as, the objects to sell,
the reserve code and the length of the auction and the generated
default string.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_FULL%^RESET%^
.EI
.SP 7 5

The event when the shop is too full to handle the extra item.  The
extra arguments are the name to sell it as, the objects to sell,
the reserve code and the length of the auction.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_INFO%^RESET%^
.EI
.SP 7 5

This event is when the player does an 'info' command to find out
how long each of the auctions is.  The extra argument is the generated
info string.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_LIST%^RESET%^
.EI
.SP 7 5

The event for listing the contents of the shop.  The extra arguement is
the generated shoip list text.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_NOTHING_TO_CLAIM%^RESET%^
.EI
.SP 7 5

The event when the person has nothing to claim.  No extra arguements.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_TOO_POOR%^RESET%^
.EI
.SP 7 5

The event when the bidder is too poor.  The extra arguements are
auction class, the players money, the value of the placed bid.

.EP

.SI 3
* %^BOLD%^AUCTION_EVENT_WITHDRAW%^RESET%^
.EI
.SP 7 5

The event when an item is withdrawn.  The extra arguements are
the auction class of the withdrawn item.

.EP

.SI 3
* %^BOLD%^AUCTION_FINISH_PHASE%^RESET%^
.EI
.SP 7 5

This define is used in the function for the item for the
finish auction phase.

.EP

.SI 3
* %^BOLD%^AUCTION_RETRY_PHASE%^RESET%^
.EI
.SP 7 5

This define is used in the function for the item for the
claim the item auction phase.

.EP

.SI 3
* %^BOLD%^AUCTION_WITHDRAW_PHASE%^RESET%^
.EI
.SP 7 5

This define is used in the function for the item for the
withdraw auction phase.

.EP

