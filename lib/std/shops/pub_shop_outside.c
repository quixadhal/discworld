/**
 * Based on the original "pub.c" code, this version allows you to buy an
 * actual drink (inna glass) instead of just forcefeeding you with whatever
 * you purchase.  You can also create food and serve it on plates, in boxes,
 * or whatever.
 * <p>
 * The items on sale are either cloned from the armoury, cloned from a file
 * or cloned in the "create_item()" code in the pub code itself.  Containers
 * for these items are done in the same way, using the "create_container()"
 * function.  This is the same as the "create_object()" system in
 * "item_shop.c".  You can also buy items for other people (or groups of
 * other people) in the pub.
 * <p>
 * There are several standard glasses and plates available for drinks and
 * food in the "pub_shop.h" header file, if you don't want to create the
 * glasses and plates yourself.  These plates use the "/obj/misc/plate.c"
 * inheritable file.
 * <p>
 * A "counter" will automatically be placed in the pub.  If any item cannot
 * be moved into the person who ordered it (i.e they are carrying too much )
 * it will be placed on the counter.  Empty glasses and so on will be
 * cleared from the counter after 3 seconds.  To ensure that this happens,
 * make sure that the container object you use has the property "pub item"
 * added to it.  Items will only be cleared if they are empty.
 * @author Lemming
 * @started 23/10/1999
 * @see /std/shops/inherit/pub.c
 * @see /include/shops/pub_shop.h
 * @see /include/volumes.h
 *
 * Added help file as requested by Gin on August 2001
 */

inherit "/std/room/outside";
inherit "/std/shops/inherit/pub";

/** @ignore */
void create() {
   add_help_file("pub_shop");
   do_setup++;
   outside::create();
   pub::create();
   do_setup--;
   if (!do_setup) {
      this_object()->setup();
      this_object()->reset();
   
   }

} /* create() */

/** @ignore */
void init() {
   outside::init();
   pub::init();
} /* init() */

/** @ignore */
void dest_me() {
   pub::dest_me();
   outside::dest_me();
} /* dest_me() */
