#include "path.h";

inherit "/std/outsides/desert";

void setup() {
  set_short("Desert");
  set_long("This is Desert at the junction with Gumboot Road.
\n");
  set_light( 80 );
  add_exit("east", DESERT + "desert2", "road");
  add_exit("west", GUMBOOT + "gumboot5", "road");
}
