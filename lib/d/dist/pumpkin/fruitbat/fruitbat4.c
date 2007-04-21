#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Fruitbat Avenue");
  set_long("This is east end of Fruitbat Avenue at the junction "
           "with Squash Square.\n");
  set_light( 80 );
  add_exit("east", SQUASH + "squash4", "road");
  add_exit("west", FRUITBAT + "fruitbat3", "road");

  set_monster(1, "city");
}
