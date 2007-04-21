/**
 * The auction house include file.
 * @author Pinkfish
 * @started Tue May  1 01:26:28 PDT 2001
 */
#ifndef __SYS_ROOM_AUCTION_H
/** @ignore yes */
#define __SYS_ROOM_AUCTION_H

/**
 * This define is used in the function for the item for the
 * finish auction phase.
 */
#define AUCTION_FINISH_PHASE 1
/**
 * This define is used in the function for the item for the
 * withdraw auction phase.
 */
#define AUCTION_WITHDRAW_PHASE 2
/**
 * This define is used in the function for the item for the
 * claim the item auction phase.
 */
#define AUCTION_CLAIM_PHASE 3
/**
 * This define is used in the function for the item for the
 * claim the item auction phase.
 */
#define AUCTION_RETRY_PHASE 4

/**
 * The event for listing the contents of the shop.  The extra arguement is
 * the generated shoip list text.
 */
#define AUCTION_EVENT_LIST "list"
/**
 * The event for browsing a specific item for auction.  The extra arguments
 * are the auction class, the objects and the generated string.
 */
#define AUCTION_EVENT_BROWSE "browse"
/**
 * The item index was bad, unable to find the item. The extra argument is the
 * id that was bad.
 */
#define AUCTION_EVENT_BAD_ITEM "bad_item"
/**
 * The event when the bid is too small.  The extra arguements are the
 * auction class, the value of the placed bid, the new minimum bid.
 */
#define AUCTION_EVENT_BID_TOO_SMALL "bid_to_small"
/**
 * The event when the bid is below the reserve.  The extra arguements are the
 * auction class, the value of the placed bid, the reserve.
 */
#define AUCTION_EVENT_BID_BELOW_RESERVE "bid_below_reserve"
/**
 * The even when a bid is made.  The extra arguements are the auction
 * class and the bid made.
 */
#define AUCTION_EVENT_BID "bid"
/**
 * The event when the bidder is too poor.  The extra arguements are
 * auction class, the players money, the value of the placed bid.
 */
#define AUCTION_EVENT_TOO_POOR "too_poor"
/**
 * The event when an item is withdrawn.  The extra arguements are
 * the auction class of the withdrawn item.
 */
#define AUCTION_EVENT_WITHDRAW "withdraw"
/**
 * The event when an item is unable to be withdrawn.  The extra arguements are
 * the auction class of the withdrawn item.
 */
#define AUCTION_EVENT_CANNOT_WITHDRAW "cannot_withdraw"
/**
 * The event when an item is to be claimed.  The extra arguements are
 * the auction class of the claimed item, the bid and the objects claimed.
 */
#define AUCTION_EVENT_CLAIM "claim"
/**
 * The event when the person has nothing to claim.  No extra arguements.
 */
#define AUCTION_EVENT_NOTHING_TO_CLAIM "nothing_to_claim"
/**
 * The event when the shop is too full to handle the extra item.  The
 * extra arguments are the name to sell it as, the objects to sell,
 * the reserve code and the length of the auction.
 */
#define AUCTION_EVENT_FULL "full"
/**
 * The event when the player is giving a bad money spec.   The
 * extra arguments are the name to sell it as, the objects to sell,
 * the reserve code and the length of the auction.
 */
#define AUCTION_EVENT_BAD_MONEY "bad_money"
/**
 * The event when the player is about to try and add something into the shop.
 * The extra arguments are the name to sell it as, the objects to sell,
 * the reserve code and the length of the auction and the generated
 * default string.
 */
#define AUCTION_EVENT_ADD_AUCTION "add_auction"
/**
 * Ths event when the player has aborted adding an auction to the shop.
 * The extra arguments are the name to sell it as, the objects to sell,
 * the reserve code and the length of the auction.
 */
#define AUCTION_EVENT_ADD_ABORT "add_abort"
/**
 * Ths event when the player has completely added the auction to the shop.
 * The extra arguments are the name to sell it as, the objects to sell,
 * the reserve code and the length of the auction and the generated
 * default string.
 */
#define AUCTION_EVENT_CONFIRM_AUCTION "confirm_auction"
/**
 * This event is when the player does an 'info' command to find out
 * how long each of the auctions is.  The extra argument is the generated
 * info string.
 */
#define AUCTION_EVENT_INFO "info"

#endif
