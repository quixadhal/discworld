#include "path.h"
inherit CITYROOM;
 
#define NUM 3
 
void setup() {
  set_light(100);
  set_short("Filgree Street outside shop");
  set_long(
"You are on Filgree Street.  There is a shop to the north of here. "+
"Filgree street continues to the east and west.\n");
 
  add_exit("east", ROOM+"filgree11", "road");
  add_exit("west", ROOM+"filgree9", "road");
  add_exit("north", ROOM+"filgreeshop", "door");
 
  add_item("shop", "It looks as though it is owned by someone shifty.\n");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
