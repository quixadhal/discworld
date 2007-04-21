#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("north east corner of Squash Square");
  set_long("This is the north east corner of Squash Square\n");
  set_light( 80 );
  add_exit("west", SQUASH + "squash2", "road");
  add_exit("south", SQUASH + "squash6", "road");
  add_exit("southwest", SQUASH + "squash5", "hidden");

  add_property( "los", "open" );
  set_linker(({ PATH + "squash2", PATH + "squash6", PATH + "squash5" }),
             "to a point", "", "Squash Square");

  set_zone("pumpkin");
  set_monster(1, "city");
}
