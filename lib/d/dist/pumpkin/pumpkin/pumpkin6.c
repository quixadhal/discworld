#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Pumpkin Way");
  set_long("This is the end of Pumpkin Way at the junction with "
           "Squash Square.\n");
  set_light( 80 );
  add_exit("north", PUMPKIN + "pumpkin5", "road");
  add_exit("south", SQUASH + "squash2", "road");

  set_monster(2, "pumpkin");
}
