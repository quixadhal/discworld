#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Pumpkin Way");
  set_long("This is the middle of Pumpkin Way you can just see Squash Square "
           "to the south.\n");
  set_light( 80 );
  add_exit("north", PUMPKIN + "pumpkin3", "road");
  add_exit("south", PUMPKIN + "pumpkin5", "road");
  add_exit("west", PATH + "stable_entry", "gate");

  set_monster(2, "pumpkin");
}
