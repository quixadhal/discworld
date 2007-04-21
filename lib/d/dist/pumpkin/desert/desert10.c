#include "path.h";

inherit "/std/outsides/desert";

void setup() {
  set_short("Desert");
  set_long("This is Desert at the junction with Woods.
\n");
  set_light( 80 );
  add_exit("west", DESERT + "desert9", "road");
  add_exit("north", WOODS + "woods5", "road");
}
