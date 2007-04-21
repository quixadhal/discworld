#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("east edge of Squash Square");
  set_long("This is east side of Squash Square at the junction "
           "with Rabbit Lane.\n");
  set_light( 80 );
  add_exit("east", RABBIT + "rabbit1", "road");
  add_exit("west", SQUASH + "squash5", "road");
  add_exit("north", SQUASH + "squash3", "road");
  add_exit("south", SQUASH + "squash9", "road");
  add_exit("northwest", SQUASH + "squash2", "hidden");
  add_exit("southwest", SQUASH + "squash8", "hidden");

  add_property( "los", "open" );
  set_linker(({ PATH + "squash5", PATH + "squash3", PATH + "squash9",
                PATH + "squash2", PATH + "squahs8" }),
             "into", "", "Squash Square");

  set_zone("pumpkin");
  set_monster(1, "city");
}
