#include "path.h"
inherit CITYROOM;
 
#define NUM 1
 
setup() {
  set_light(100);
  set_short("Outside Filgree Smithy");
  set_long(
"You are on Filgree street.  The street continues to the "+
"east and west.  There is a smithy to the north.\n");
 
  add_exit("east", ROOM+"filgree4", "road");
  add_exit("west", ROOM+"filgree2", "road");
  add_exit("north", ROOM+"smithy", "door");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
 
