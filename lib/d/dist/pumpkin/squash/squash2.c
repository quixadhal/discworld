#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("north edge of Squash Square");
  set_long("This is the north of Squash Square at the "
           "junction with Pumpkin Way.\n");
  set_light( 80 );
  add_exit("east", SQUASH + "squash3", "road");
  add_exit("west", SQUASH + "squash1", "road");
  add_exit("north", PUMPKIN + "pumpkin6", "road");
  add_exit("south", SQUASH + "squash5", "road");
  add_exit("southwest", SQUASH + "squash4", "hidden");
  add_exit("southeast", SQUASH + "squash6", "hidden");

  set_linker(({ PATH + "squash3", PATH + "squash1", PATH + "squash5",
                PATH + "squash4", PATH + "squash6" }),
             "into", "", "Squash Square");

  set_zone("pumpkin");
  set_monster(1, "city");
}
