#include "path.h";

inherit "/std/outsides/desert";

void setup() {
  set_short("Forest");
  set_long("This is the end of Forest\n");
  set_light( 80 );
  add_exit("west", PLAIN + "plain9", "road");
  add_exit("north", PLAIN + "plain5", "road");
}
