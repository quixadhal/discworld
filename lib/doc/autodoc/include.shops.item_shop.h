.DT
item_shop.h
Disk World autodoc help
item_shop.h

.SH Description
.SP 5 5

The include file for dealing with an item shop.
This defines all the events and anything else needed to handle
the shop.
.EP
.SP 10 5


Written by Pinkfish

Started Thu May  3 15:12:42 PDT 2001

.EP

.SH Defines
.SI 3
* %^BOLD%^ITEM_SHOP_EVENT_BAD_LABEL%^RESET%^
.EI
.SP 7 5

The event when the shop has been given a bad item label.
The extra argument to this it the label they typed incorrectly.

.EP

.SI 3
* %^BOLD%^ITEM_SHOP_EVENT_BROWSE%^RESET%^
.EI
.SP 7 5

The event when the shop is being browsed.  The extra argument
to this event is the objects to browse and the message made by the
system.

.EP

.SI 3
* %^BOLD%^ITEM_SHOP_EVENT_BUY_THING%^RESET%^
.EI
.SP 7 5

The event when the shop is being bought from.  The extra argument
to this event is the item being bought.

.EP

.SI 3
* %^BOLD%^ITEM_SHOP_EVENT_CLOSE%^RESET%^
.EI
.SP 7 5

The event when the shop is closed.

.EP

.SI 3
* %^BOLD%^ITEM_SHOP_EVENT_LIST%^RESET%^
.EI
.SP 7 5

The event when the shop is listing something.  The extra argument
to this is the generated list message.

.EP

.SI 3
* %^BOLD%^ITEM_SHOP_EVENT_NOTHING%^RESET%^
.EI
.SP 7 5

The event when the shop has nothing to list.

.EP

.SI 3
* %^BOLD%^ITEM_SHOP_EVENT_SOLD_OUT%^RESET%^
.EI
.SP 7 5

The event when the shop has sold out of something.  The extra
argument to this is the thing they have sold out of.

.EP

.SI 3
* %^BOLD%^ITEM_SHOP_EVENT_TOO_EXPENSIVE%^RESET%^
.EI
.SP 7 5

The event when the item is too expensive to buy.  The extra
arguments to this is the item that is too expensive, the cost of
the item and the amount the player has left.

.EP

.SI 3
* %^BOLD%^ITEM_SHOP_INHERIT%^RESET%^
.EI
.SP 7 5

This is the main inherit for an item shop.

.EP

.SI 3
* %^BOLD%^ITEM_SHOP_ITEMS%^RESET%^
.EI
.SP 7 5

Where to find any items used by the item shop.

.EP

.SI 3
* %^BOLD%^ITEM_SHOP_OUTSIDE_INHERIT%^RESET%^
.EI
.SP 7 5

This is the main inherit for an item shop.

.EP

