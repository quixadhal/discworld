#include "path.h"
inherit CITYROOM;
 
#define NUM 3
 
setup() {
  set_light(100);
  set_short("North of the Fighters' Guild");
  set_long(
"There is a pub on the street to the east of here.  Filgree street "+
"wanders off into the distance to the west of here.  Sounds of "+
"sword-play come from a large courtyard and building to the south.\n");
 
  add_exit("east", ROOM+"outsidedrum", "road");
  add_exit("west", ROOM+"filgree6", "road");
  add_exit("south", FGUILD+"lobby", "corridor");
 
  add_item("pub", "It is called the Mended drum.\n");
 
  add_item("distance", "This is where Filgree street goes.\n");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
