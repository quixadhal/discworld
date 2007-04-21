/**
 * The include file for dealing with an item shop.
 * This defines all the events and anything else needed to handle
 * the shop.
 * @author Pinkfish
 * @started Thu May  3 15:12:42 PDT 2001
 */
#ifndef __SYS_SHOPS_ITEM_SHOP
/** @ignore yes */
#define __SYS_SHOPS_ITEM_SHOP

/**
 * This is the main inherit for an item shop.
 */
#define ITEM_SHOP_INHERIT "/std/shops/item_shop"
/**
 * This is the main inherit for an item shop.
 */
#define ITEM_SHOP_OUTSIDE_INHERIT "/std/shops/item_shop_outside"

/**
 * Where to find any items used by the item shop.
 */
#define ITEM_SHOP_ITEMS "/obj/misc/shops/"

/**
 * The event when the shop is closed.
 */
#define ITEM_SHOP_EVENT_CLOSE "close"
/**
 * The event when the shop has nothing to list.
 */
#define ITEM_SHOP_EVENT_NOTHING "nothing"
/**
 * The event when the shop has nothing to list.
 */
#define ITEM_SHOP_EVENT_NOTHING "nothing"
/**
 * The event when the shop is listing something.  The extra argument
 * to this is the generated list message.
 */
#define ITEM_SHOP_EVENT_LIST "list"
/**
 * The event when the shop has been given a bad item label.
 * The extra argument to this it the label they typed incorrectly.
 */
#define ITEM_SHOP_EVENT_BAD_LABEL "bad_label"
/**
 * The event when the shop has sold out of something.  The extra
 * argument to this is the thing they have sold out of.
 */
#define ITEM_SHOP_EVENT_SOLD_OUT "sold_out"
/**
 * The event when the item is too expensive to buy.  The extra
 * arguments to this is the item that is too expensive, the cost of
 * the item and the amount the player has left.
 */
#define ITEM_SHOP_EVENT_TOO_EXPENSIVE "too_expensive"
/**
 * The event when the shop is being browsed.  The extra argument
 * to this event is the objects to browse and the message made by the
 * system.
 */
#define ITEM_SHOP_EVENT_BROWSE "browse"
/**
 * The event when the shop is being bought from.  The extra argument
 * to this event is the item being bought.
 */
#define ITEM_SHOP_EVENT_BUY_THING "buy_thing"

#endif
