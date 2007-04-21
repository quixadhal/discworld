#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Rabbit Lane");
  set_long("This is end of Rabbit Lane at the exit of the city of Pumpkin.\n");
  set_light( 80 );
  add_exit("east", TERRAIN_HANDLER + ":0:18:0", "road");
  add_exit("west", RABBIT + "rabbit3", "road");
  add_exit("north", RABBIT + "potion_shop", "door");
}
