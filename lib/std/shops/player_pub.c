/**
 * Uses the base pub inheritable to make a player controlled pubs.
 * @author Pinkfish
 * @started Mon Feb 14 23:34:14 PST 2000
 * @see /std/shops/inherit/pub.c
 * @see /include/shops/pub_shop.h
 * @see /include/volumes.h
 */

inherit "/std/room/basic_room";
inherit "/std/shops/inherit/pub";

private mapping _cold_drinks;
private mapping _food;
private mapping _hot_drinks;

/** @ignore */
void create() {
   do_setup++;
   basic_room::create();
   pub::create();
   do_setup--;
   if (!do_setup) {
      this_object()->setup();
      this_object()->reset();
   }
} /* create() */

/** @ignore */
void init() {
   basic_room::init();
   pub::init();
} /* init() */

/** @ignore */
void dest_me() {
   pub::dest_me();
   basic_room::dest_me();
} /* dest_me() */


