#include "path.h"
inherit CITYROOM;
 
#define NUM 3
 
void setup() {
  set_light(100);
  set_short("Intersection of Matthews and Rime");
  set_long(
"You are at the intersection of Matthews and Rime.  Matthews street "+
"continues to the east and west.  Rime street goes south before "+
"turning a corner, in the distance.\n");
 
  add_exit("west", ROOM+"matthews2", "road");
  add_exit("east", ROOM+"matthews4", "road");
  add_exit("south", ROOM+"rime9", "road");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
