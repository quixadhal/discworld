#include "path.h"
inherit CITYROOM;

#define NUM 2
 
setup() {
  set_light(100);
  set_short("West end of Filgree Street");
  set_long(
"You are at the west end of Filgree Street.  There are some stables "+
"to the west and a gate that leaves the city to the north.  Filgree "+
"street continues to the east.\n");
 
  add_exit("east", ROOM+"filgree2", "road");
  add_exit("west", ROOM+"stable", "gate");
  add_exit("north", ROOM+"citygaten", "road");
 
  add_item("stables",
"The stables have many horses, some owned and some unowned "+
"It is a smelly place, but doesn't compare to the rest of "+
"Ankh Morpork.\n");
  add_alias("stable", "stables");
 
  add_item("gate",
"This gate leads North out of the city.  In the distance, "+
"you can see the Ramtops.\n");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
 
