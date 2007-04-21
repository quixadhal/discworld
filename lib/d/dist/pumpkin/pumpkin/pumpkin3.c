#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Pumpkin Way");
  set_long("This is the middle of Pumpkin Way north of Squash Square.\n");
  set_light( 80 );
  add_exit("north", PUMPKIN + "pumpkin2", "road");
  add_exit("south", PUMPKIN + "pumpkin4", "road");

  set_monster(2, "pumpkin");
}
