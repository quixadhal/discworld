#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Pumpkin Way");
  set_long("This is the middle of Pumpkin Way, quite close to the junction "
           "with Squash Square to the south.\n");
  set_light( 80 );
  add_exit("north", PUMPKIN + "pumpkin4", "road");
  add_exit("south", PUMPKIN + "pumpkin6", "road");
  //add_exit("east", PATH + "stable_shop", "gate");

  set_monster(2, "pumpkin");
}
