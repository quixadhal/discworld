#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Fruitbat Avenue");
  set_long("This is Fruitbat Avenue at the junction with Crumpet Circle.\n");
  set_light( 80 );
  add_exit("east", FRUITBAT + "fruitbat2", "road");
  add_exit("north", CRUMPT + "crumpt8", "road");

  set_monster(1, "city");
}
