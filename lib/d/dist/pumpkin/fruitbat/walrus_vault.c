#include "path.h"
inherit "/std/shops/vault";

void setup() {
   set_short("Wulrus's vault");
   set_long(
"A big picture of a nice friendly looking walrus looks down on you "
"with a nice happy smile.\n");

   set_light(70);

   set_vault_name("walrus");

   set_vault_exit("south", FRUITBAT + "walrus_vault_entry", "door");
   add_property("place", "Pumpkin");
} /* setup() */
