#include "path.h"
inherit CITYROOM;
 
#define NUM 2
 
setup() {
  set_light(100);
  set_short("Junction of Small Gods onto Filgree Street");
  set_long(
"Small Gods Steet runs south from here.  Filgree street "+
"continues to the east and west.\n");
 
  add_exit("east", ROOM+"filgree5", "road");
  add_exit("west", ROOM+"filgree3", "road");
  add_exit("south", ROOM+"smallgod1", "road");
 
  add_item("temple",
"You can see the temple of small gods to the south a bit.\n");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
 
