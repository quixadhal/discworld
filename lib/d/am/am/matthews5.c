#include "path.h"
inherit CITYROOM;
 
#define NUM 1
 
void setup() {
  set_light(100);
  set_short("Corner of Matthews and Muligan");
  set_long(
"You are at the end of Matthews street.  Muligan street goes north from "+
"here and Matthews street leads west.  There is a large building in the "+
"distance to the north.  To the east is a city gate.\n");
 
  add_exit("west", ROOM+"matthews4", "road");
  add_exit("north", ROOM+"muligan4", "road");
  add_exit("east", ROOM+"matthews6", "road");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
