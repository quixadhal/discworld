#include "path.h"
inherit CITYROOM;
 
#define NUM 3
 
void setup() {
  set_light(100);
  set_short("Matthews Street");
  set_long(
"You are on Matthews Street.  It continues to the east and west.  You "+
"can see a small run down looking gambling house to the south. "+
"Matthews street continues east and west.  Just to the west is an "+
"intersection.\n");
 
  add_exit("west", ROOM+"matthews3", "road");
  add_exit("east", ROOM+"matthews5", "road");
  add_exit("south", ROOM+"gambling", "door");
 
  add_item("intersection",
"It is the intersection of Matthews and Rime.\n");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
