#include "path.h"

inherit "/std/shops/item_shop";

void setup() {
   set_short("Squiggles Clothing Store");
   set_long(
"You are standing in a veritable forest of clothes, they hang off racks "
"and almost completely hide the floor.  Unfortunately they seem to be "
"reall limited in the type clothes they sell, obviously they belive in "
"volume rather than breadth of stock.\n");
   set_light(70);

   add_exit("east", PATH + "gumboot2", "door");

   add_object("green bathrobe", 1 + random(10));
   add_object("green cloak", 1 + random(10));
   add_object("green pants", 1 + random(10));
   add_object("green peaked hat", 1 + random(10));
   add_object("green silk sash", 1 + random(10));
   add_object("green wool shirt", 1 + random(10));
   add_object("white apron", 1 + random(10));
   add_object("white burnous", 1 + random(10));
   add_object("white cotton bodice", 1 + random(10));
   add_object("white cotton robe", 1 + random(10));
   add_object("white cotton shirt", 1 + random(10));
   add_object("white cotton toga", 1 + random(10));
   add_object("white cotton tunic", 1 + random(10));
   add_object("white linen tunic", 1 + random(10));
   add_object("white linen skirt", 1 + random(10));
   add_object("white short-sleeved shirt", 1 + random(10));
   add_object("white silk lace bodice", 1 + random(10));
   add_object("white silk pinafore", 1 + random(10));
   add_object("white silk sash", 1 + random(10));
   add_object("white socks", 1 + random(10));

   add_property("place", "Pumpkin");
} /* setup() */
