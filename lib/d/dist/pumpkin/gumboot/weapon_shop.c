#include "path.h"
inherit "/std/shops/item_shop";

void setup() {
   set_short("Pointy's Weapon Shop");
   set_long(
"Racks of weapons line the room, don't touch, they look sharp.\n");
   set_light(70);

   add_exit("east", PATH + "gumboot4", "door");

   add_object("morning star", 1 + random(20));
   add_object("black handled ceremonial bronze knife", 1 + random(20));
   add_object("carving knife", 1 + random(20));
   add_object("chisel", 1 + random(20));
   add_object("copper fighting knife", 1 + random(20));
   add_object("dagger", 1 + random(20));
   add_object("howondalandish throwing knife", 1 + random(20));
   add_object("klatchian steel dirk", 1 + random(20));
   add_object("knife", 1 + random(20));
   add_object("letter opener", 1 + random(20));
   add_object("meat cleaver", 1 + random(20));
   add_object("sharp butter knife", 1 + random(20));
   add_object("silver sickle", 1 + random(20));
   add_object("small copper knife", 1 + random(20));
   add_object("small knife", 1 + random(20));
   add_object("stiletto", 1 + random(20));
   add_object("tanto", 1 + random(20));
   add_object("white handled ceremonial bronze knife", 1 + random(20));


   add_property("place", "Pumpkin");
} /* setup() */
