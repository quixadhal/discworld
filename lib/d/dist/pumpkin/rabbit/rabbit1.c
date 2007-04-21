#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Rabbit Lane");
  set_long("This is Rabbit Lane at the junction with Squash Square.\n");
  set_light( 80 );
  add_exit("east", RABBIT + "rabbit2", "road");
  add_exit("west", SQUASH + "squash6", "road");
}
