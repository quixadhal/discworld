#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Gumboot Road");
  set_long("This is end of Gumboot Road at the edge of the city of Pumpkin.\n");
  set_light( 80 );
  add_exit("east", TERRAIN_HANDLER + ":0:1:0", "road");
  add_exit("north", GUMBOOT + "gumboot4", "road");

  set_monster(1, "city");
}
