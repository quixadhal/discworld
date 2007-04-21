#include "path.h";

inherit "/std/outsides/plain";

void setup() {
  set_short("Plains");
  set_long("This is Plains at the junction with Woods.
\n");
  set_light( 80 );
  add_exit("west", PLAIN + "plain4", "road");
  add_exit("south", WOODS + "woods1", "road");
}
