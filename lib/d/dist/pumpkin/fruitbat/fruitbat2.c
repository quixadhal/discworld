#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Fruitbat Avenue");
  set_long("This is the middle of Fruitbat Avenue just west of "
           "Crumpt Circle.\n");
  set_light( 80 );
  add_exit("east", FRUITBAT + "fruitbat3", "road");
  add_exit("west", FRUITBAT + "fruitbat1", "road");
  add_exit("south", FRUITBAT + "general_store", "door");

  set_monster(1, "city");
}
