#include "path.h"
inherit "/std/item-shop";

void setup() {
  set_short("Clothing shop");
  set_long("You are in a big clothing shop, it is one of the largest "+
           "shops you have ever seen.  The ailes go on forever, you "+
           "can get a list of their wares by going \"list\" and \"buy\" "+
           "them as well.\n");
  add_exit("west", ROOM+"smallgod1", "door");
  set_light(80);
  add_armour("soft leather cap",0);
  add_armour("cloth robe",0);
  add_armour("soft leathershoes",0);
  add_armour("leather gloves",0);
}
