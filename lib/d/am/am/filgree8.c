#include "path.h"
inherit CITYROOM;
 
#define NUM 1
 
void setup() {
  set_light(100);
  set_short("Outside the pawn shop");
  set_long(
"There is a pub on the road to the west of here.  Filgree street "+
"continues to the east and west.  There is a Pawn shop to the south.\n");
 
  add_exit("east", ROOM+"filgree9", "road");
  add_exit("west", ROOM+"outsidedrum", "road");
  add_exit("south", ROOM+"pawnshop", "door");
 
  add_item("pub", "It is called the Mended drum.\n");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
