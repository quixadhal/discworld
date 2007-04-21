#include "path.h"; 
inherit CITYROOM;

void setup() {
  set_short("in the middle of Squash Square");
  set_long("This is the middle of Squash Square\n");
  set_light( 80 );
  add_exit("east", SQUASH + "squash6", "road");
  add_exit("west", SQUASH + "squash4", "road");
  add_exit("north", SQUASH + "squash2", "road");
  add_exit("south", SQUASH + "squash8", "road");
  add_exit("northeast", SQUASH + "squash3", "hidden");
  add_exit("northwest", SQUASH + "squash1", "hidden");
  add_exit("southeast", SQUASH + "squash9", "hidden");
  add_exit("northwest", SQUASH + "squash7", "hidden");

  add_property( "los", "open" );
  set_linker(({ PATH + "squash6", PATH + "squash4", PATH + "squash2",
                PATH + "squash8", PATH + "squash3", PATH + "squash1",
                PATH + "squash9", PATH + "squash7" }),
             "to a point", "", "Squash Square");

  set_monster(1, "city");

  set_zone("pumpkin");
  set_co_ord(({ 0, 0, 0 }));
}
