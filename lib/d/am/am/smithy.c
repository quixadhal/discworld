#include "path.h"
inherit "/std/item-shop";

object cont;

void setup() {
  mixed *data;
  int i;
  object ob;

  set_zone("smithy");
  set_light(60);
  set_short("The smithy");
  set_long("A small cramped room with a large roaring fire blazing in "+
           "the center of the room.  There are various assorted smithy "+
           "tools scattered around the fire and hanging on the walls.  "+
           "The smithy is obviously very busy as there is a large "+
           "dirty order book lying open on a small table near the "+
           "door.  You can \"list\" the items for sale and \"buy\" them.\n");

  add_exit("south", ROOM+"filgree3", "door");

  add_item("fire", "A roaring fire blazing in the center of the room.  "+
                   "It is a coal fire built inside a stone walled "+
                   "well like thing.\n");
  add_item("table", "A small wooden table standing by the door, it "+
                    "appears to have a book resting on it.\n");
  add_item("book", "A large dirty order book lying on the table "+
                   "near the entrance to the room.  It has the words "+
                   "\"Worlds apart smithy\" written on the spine.\n");
  add_item("tools", "You know, smithy tools, forks, pokers that sort of "+
                    "thing.\n");
  add_armour("leather",0);
  add_armour("small wooden shield",0);
  add_armour("medium wooden shield",0);
  add_armour("studded leather",0);
  add_weapon("knife",0);
  add_weapon("short sword",0);
  add_weapon("dagger",0);
  add_weapon("mace",0);
  add_property("smithy", 1);
}
