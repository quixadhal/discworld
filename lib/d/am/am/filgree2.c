#include "path.h"
inherit CITYROOM;
 
#define NUM 1
 
setup() {
  set_light(100);
  set_short("Filgree Street");
  set_long(
"You are on Filgree street.  There is a stable in the distance "+
"to the west.  The street continues to the east and west.\n");
 
  add_exit("east", ROOM+"filgree3", "road");
  add_exit("west", ROOM+"filgree1", "road");
 
  add_item("stables",
"You can see and just smell some stables in the distance to the west.\n");
  add_alias("stable", "stables");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
 
 
