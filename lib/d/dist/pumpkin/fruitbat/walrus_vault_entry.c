#include "path.h"
inherit "/std/shops/vault_entry";

object _npc;

void setup() {
   set_short("Wulrus's vault");
   set_long(
"A big picture of a nice friendly looking walrus looks down on you "
"with a nice happy smile.\n");

   set_light(70);

   set_vault_name("walrus");
   set_visit_cost(400);
   set_open_cost(4000);

   add_exit("south", FRUITBAT + "fruitbat3", "door");
   set_vault_exit("north", FRUITBAT + "walrus_vault", "door");
   add_property("place", "Pumpkin");
} /* setup() */

void reset() {
   set_proprietor(_npc);
} /* reset() */
