#include "path.h"
inherit CITYROOM;
 
#define NUM 2
 
void setup() {
  set_light(100);
  set_short("Small Gods Street");
  set_long(
"You are on Small Gods Street which continues to the " +
"east and west from here.  You can see Short Street in the distance "+
"to the east.\n");
 
  add_exit("west", ROOM+"smallgod4", "road");
  add_exit("east", ROOM+"smallgod6", "road");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
