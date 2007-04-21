#include "path.h"

inherit "/std/shops/print_shop";

void setup() {
   set_short("Flopsies Print Shop");
   set_long(
"This place appears to have been made into a shrine for a rabbit, you "
"can see a small altar and carrot covered cloth on the counter.\n");
   add_item("counter", ({ "long", "The counter runs across the "
            "front of the room and there is a small altar set up "
            "on the edge of it.",
            "position", "the counter" }) );
   add_item("altar", ({ "long", "A nice altar with a little sign "
            "under it.",
            "read", "In memory of 'Mopsy'.\n" }) );
   set_light(70);

   add_exit("north", RABBIT + "rabbit3", "door");

   add_property("place", "Pumpkin");
} /* setup() */
