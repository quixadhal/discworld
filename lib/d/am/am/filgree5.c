#include "path.h"
inherit CITYROOM;
 
#define NUM 3
 
setup() {
  set_light(100);
  set_short("Filgree outside Flower Shop");
  set_long(
"You are on Filgree street.  There appears to be a street "+
"leading south to the west.  Filgree street "+
"continues to the east and west of here.  There is a small colourful "+
"shop to the north.  From which is emanating nice smells (unlike the "+
"rest of the city).\n");
 
  add_exit("east", ROOM+"filgree6", "road");
  add_exit("west", ROOM+"filgree4", "road");
  add_exit("north", ROOM+"flower_shop", "corridor");
 
  add_item("temple",
"You can't see it from here.  Further west perhaps.\n");
 
  add_item("street",
"There is a sign that tells you the street to the west is "+
"Small Gods Street.  There is a temple on it.\n");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
 
