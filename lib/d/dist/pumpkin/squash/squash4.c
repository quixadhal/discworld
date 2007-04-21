#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("west edge of Squash Square");
  set_long("This is the west side of Squash Square at the junction "
           "with Fruitbat Avenue.\n");
  set_light( 80 );
  add_exit("east", SQUASH + "squash5", "road");
  add_exit("west", FRUITBAT + "fruitbat4", "road");
  add_exit("north", SQUASH + "squash1", "road");
  add_exit("south", SQUASH + "squash7", "road");
  add_exit("northeast", SQUASH + "squash2", "hidden");
  add_exit("southeast", SQUASH + "squash8", "hidden");

  add_property( "los", "open" );

  set_linker(({ PATH + "squash5", PATH + "squash1", PATH + "squash7",
                 PATH + "squash2", PATH + "squash8" }),
              "into", "", "Squash Square");

  set_zone("pumpkin");
  set_monster(1, "city");
}
