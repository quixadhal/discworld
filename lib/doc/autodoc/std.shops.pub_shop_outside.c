.DT
pub_shop_outside.c
Disk World autodoc help
pub_shop_outside.c

.SH Description
.SP 5 5

Based on the original "pub.c" code, this version allows you to buy an
actual drink (inna glass) instead of just forcefeeding you with whatever
you purchase.  You can also create food and serve it on plates, in boxes,
or whatever.


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
/std/shops/inherit/pub.c, /include/shops/pub_shop.h and /include/volumes.h

Added help file as requested by Gin on August 2001

.EP
.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/outside and /std/shops/inherit/pub.
.EP

