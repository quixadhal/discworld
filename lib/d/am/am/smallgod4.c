#include "path.h"
inherit CITYROOM;
 
#define NUM 1
 
void setup() {
  set_light(100);
  set_short("Small Gods Street");
  set_long(
"You are on Small Gods Street.  It continues east and west of here.  "+
"To the west it turns right and heads north to the temple.\n");
 
  add_exit("west", ROOM+"smallgod3", "road");
  add_exit("east", ROOM+"smallgod5", "road");
 
  add_item("temple",
"You can see the top of the Temple of Small Gods.\n");
  add_alias("temple of small gods", "temple");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
