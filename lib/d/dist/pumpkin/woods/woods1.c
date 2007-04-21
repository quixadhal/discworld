#include "path.h";

inherit "/std/outsides/woodland";

void setup() {
  set_short("Woods");
  set_long("This is Woods at the junction with Plains.
\n");
  set_light( 80 );
  add_exit("north", PLAIN + "plain5", "road");
  add_exit("south", WOODS + "woods2", "road");
}
