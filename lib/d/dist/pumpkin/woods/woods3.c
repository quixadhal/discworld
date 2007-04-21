#include "path.h";

inherit "/std/outsides/woodland";

void setup() {
  set_short("Woods");
  set_long("This is the end of Woods\n");
  set_light( 80 );
  add_exit("north", WOODS + "woods2", "road");
  add_exit("south", WOODS + "woods4", "road");
}
