#include "path.h"
inherit CITYROOM;
 
#define NUM 2
 
void setup() {
  set_light(100);
  set_short("Matthews Street");
  set_long(
"You are on Matthews street.  Matthews street continues to the east and "+
"west from here.  There is an intersection to the east.\n");
 
  add_exit("west", ROOM+"matthews1", "road");
  add_exit("east", ROOM+"matthews3", "road");
 
  add_item("intersection",
"It is the intersection of Matthews and Rime");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
