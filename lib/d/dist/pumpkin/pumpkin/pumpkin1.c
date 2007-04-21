#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Pumpkin Way");
  set_long("This is end of Pumpkin Way at the junction with Crumpet Circle.\n");
  set_light( 80 );
  add_exit("east", PUMPKIN + "pumpkin2", "road");
  add_exit("southwest", CRUMPT + "crumpt1", "road");

  set_monster(2, "pumpkin");
}
