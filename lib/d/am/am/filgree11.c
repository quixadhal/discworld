#include "path.h"
inherit CITYROOM;
 
#define NUM 2
 
void setup() {
  set_light(100);
  set_short("Filgree Street");
  set_long(
"You are on Filgree Street.  There is a shop on the street to the "+
"west of here.  Filgree street continues east and west of here.\n");
 
  add_exit("east", ROOM+"filgree12", "road");
  add_exit("west", ROOM+"filgree10", "road");
 
  add_item("Shop", "It looks disreputable.\n");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
