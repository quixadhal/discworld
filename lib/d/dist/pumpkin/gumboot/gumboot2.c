#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Gumboot Road");
  set_long("This is the middle of Gumboot Road\n");
  set_light( 80 );
  add_exit("north", GUMBOOT + "gumboot1", "road");
  add_exit("south", GUMBOOT + "gumboot3", "road");
  add_exit("west", PATH + "clothes_shop", "door");

  set_monster(1, "city");
}
