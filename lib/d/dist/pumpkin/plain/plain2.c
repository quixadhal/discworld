#include "path.h";

inherit "/std/outsides/plain";

void setup() {
  set_short("Plains");
  set_long("This is the end of Plains\n");
  set_light( 80 );
  add_exit("east", PLAIN + "plain3", "road");
  add_exit("west", PLAIN + "plain1", "road");
}
