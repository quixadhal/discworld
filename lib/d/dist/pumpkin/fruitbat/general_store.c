#include "path.h"
inherit "/std/shops/general_shop";

void setup() {
   set_short("Elephant's Stamp");
   set_long(
"A huge picture of an elephant covers tha back room of the shop.  You can "
"see the elephant guarding a huge pile of goods.\n");

   add_exit("north", FRUITBAT + "fruitbat2", "door");
   set_light(70);
   set_store_room(FRUITBAT + "storeroom");

   add_property("place", "Pumpkin");
} /* setup() */
