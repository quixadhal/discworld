#include "path.h"

inherit "/std/shops/potion_shop";

void setup() {
   set_short("Carrot's Potion Store");
   set_long(
"This is a bright orange room with a green ceiling.\n");
   set_light(70);
 
   add_exit("south", RABBIT + "rabbit4", "door");

   set_save_dir(SAVE + "potions");
   set_cut(10);

   add_property("place", "Pumpkin");
} /* setup() */
