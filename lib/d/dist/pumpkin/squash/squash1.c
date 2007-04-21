#include "path.h";

inherit CITYROOM;

void setup() {

  set_short("northwest corner of Squash Square");
  set_long("This is the north west corner of Squash Square\n");
  set_light( 80 );
  add_exit("east", SQUASH + "squash2", "road");
  add_exit("south", SQUASH + "squash4", "road");
  add_exit("southeast", SQUASH + "squash5", "hidden");

  add_property( "los", "open" );
  set_linker(({ PATH + "squash2", PATH + "squash4", PATH + "squash5" }),
             "to a point", "", "Squash Square");
  set_zone("pumpkin");

  set_monster(1, "city");
}
