#include "path.h";

inherit "/std/outsides/plain";

void setup() {
  set_short("Plains");
  set_long("This is Plains at the junction with Rabbit Lane.
\n");
  set_light( 80 );
  add_exit("east", PLAIN + "plain2", "road");
  add_exit("west", RABBIT + "rabbit4", "road");
}
