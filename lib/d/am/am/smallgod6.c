#include "path.h"
inherit CITYROOM;
 
#define NUM 2
 
void setup() {
  set_light(100);
  set_short("Small Gods Street");
  set_long(
"You are at the east end of Small Gods Street.  The junction with Short " +
"Street can be seen further east.\n");
 
  add_exit("west", ROOM+"smallgod5", "road");
  add_exit("east", SHORT+"short3", "road");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
