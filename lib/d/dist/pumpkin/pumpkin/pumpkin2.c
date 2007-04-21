#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Pumpkin Way");
  set_long("This is middle of Pumpkin Way just north of Crumpt Circle.\n");
  set_light( 80 );
  add_exit("west", PUMPKIN + "pumpkin1", "road");
  add_exit("south", PUMPKIN + "pumpkin3", "road");

  set_monster(2, "pumpkin");
}
