#include "path.h"
inherit CITYROOM;
 
#define NUM 1
 
void setup() {
  set_light(100);
  set_short("Corner of Filgree and Mulligan");
  set_long(
"You are at the eastern end of Filgree Street.  You can see the "+
"Unseen University entrance to the south.  Filgree Street "+
"leads west from here, and a guard post can be seen to the east.\n");
 
  add_exit("south", ROOM+"outsideuu", "road");
  add_exit("west", ROOM+"filgree11", "road");
  add_exit("east", ROOM+"filgree13", "road");
 
  add_item("university",
"You can't see very well from here.  You could see "+
"better if you went further south.\n");
  add_alias("uni", "university");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
