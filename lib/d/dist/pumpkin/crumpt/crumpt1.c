#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Crumpet Circle");
  set_long("This is Crumpet Circle at the junction with Pumpkin Way.
\n");
  set_light( 80 );
  add_exit("northeast", PUMPKIN + "pumpkin1", "road");
  add_exit("southwest", CRUMPT + "crumpt2", "road");
  add_exit("south", CRUMPT + "crumpt3", "road");

  set_monster( 1, "city" );
}
