#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Gumboot Road");
  set_long("This is north end of Gumboot Road at the junction with Squash Square.\n");
  set_light( 80 );
  add_exit("north", SQUASH + "squash8", "road");
  add_exit("south", GUMBOOT + "gumboot2", "road");

  set_monster(1, "city");
}
