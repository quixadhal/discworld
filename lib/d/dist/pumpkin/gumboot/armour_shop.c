#include "path.h"
inherit "/std/shops/item_shop";

void setup() {
   set_short("Smashers Armour Store");
   set_long(
"The room is filled with the smell of leather and oil, you can see racks "
"some suits of armour in various stages of being made around the store.\n");
   set_light(70);

   add_exit("west", PATH + "gumboot3", "door");

   add_object("brigandine armour", 1 + random(4));
   add_object("hardened leather breastplate", 1 + random(4));
   add_object("jazerant armour", 1 + random(2));
   add_object("leather boxer shorts", 1 + random(2));
   add_object("leather hat", 1 + random(2));
   add_object("leather jerkin", 1 + random(2));
   add_object("padded aketon", 1 + random(2));
   add_object("studded leather jerkin", 1 + random(2));

   add_property("place", "Pumpkin");
} /* setup() */
