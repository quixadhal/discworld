/**
 * This is the main inherit for the auction house.
 * @author Pinkfish
 * @started Tue May  1 00:46:51 PDT 2001
 */
inherit "/std/room/basic_room";
inherit "/std/shops/inherit/auction_house";

void create() {
   auction_house::create();
   basic_room::create();
   add_help_file("auction_house");
} /* create() */

void init() {
   basic_room::init();
   auction_house::init();
} /* init() */
