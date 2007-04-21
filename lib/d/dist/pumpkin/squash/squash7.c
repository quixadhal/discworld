#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("south west corner of Squash Square");
  set_long("This is the south west corner Squash Square\n");
  set_light( 80 );
  add_exit("east", SQUASH + "squash8", "road");
  add_exit("north", SQUASH + "squash4", "road");
  add_exit("northeast", SQUASH + "squash3", "hidden");

  add_property( "los", "open" );
  set_linker(({ PATH + "squash8", PATH + "squash4", PATH + "squash3" }),
             "to a point", "", "Squash Square");

  set_zone("pumpkin");
  set_monster(1, "city");
}
