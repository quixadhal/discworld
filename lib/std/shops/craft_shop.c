/**
 * This room will handle craft shop like things, where you sell stuff to
 * the store and it will sell it onto other players after taking
 * a margin.  This can be used for potions, pottery, swords, anything!
 * @author Pinkfish
 * @started Thu Feb  5 15:39:57 CST 1998
 */
inherit "/std/room/basic_room";
inherit "/std/shops/inherit/craft_shop";

/** @ignore yes */
void create() {
   add_help_file("craft_shop");
   basic_room::create();
   craft_shop::create();
} /* create() */

/** @ignore yes */
void init() {
   basic_room::init();
   craft_shop::init();
} /* init() */

/** @ignore yes */
void dest_me() {
   craft_shop::dest_me();
   basic_room::dest_me();
} /* dest_me() */
