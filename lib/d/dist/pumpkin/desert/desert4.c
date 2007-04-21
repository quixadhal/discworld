#include "path.h";

inherit "/std/outsides/desert";

void setup() {
  set_short("Desert");
  set_long("This is the end of Desert\n");
  set_light( 80 );
  add_exit("east", DESERT + "desert5", "road");
  add_exit("west", DESERT + "desert3", "road");
}
