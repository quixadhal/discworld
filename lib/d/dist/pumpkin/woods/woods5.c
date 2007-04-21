#include "path.h";

inherit "/std/outsides/woodland";

void setup() {
  set_short("Woods");
  set_long("This is Woods at the junction with Desert.
\n");
  set_light( 80 );
  add_exit("north", WOODS + "woods4", "road");
  add_exit("south", DESERT + "desert10", "road");
}
