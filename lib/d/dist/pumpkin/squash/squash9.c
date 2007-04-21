#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("south east corner of Squash Square");
  set_long("This is the south east corner Squash Square.\n");
  set_light( 80 );
  add_exit("west", SQUASH + "squash8", "road");
  add_exit("north", SQUASH + "squash6", "road");
  add_exit("northwest", SQUASH + "squash5", "hidden");
  add_exit("east", PATH + "beer_bar", "door");

  add_property( "los", "open" );

  set_linker(({ PATH + "squash8", PATH + "squash6", PATH + "squash5" }),
             "to a point", "", "Squash Square");

  set_zone("pumpkin");
  set_monster(1, "city");
}
