#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Rabbit Lane");
  set_long("This is the middle of Rabbit Lane\n");
  set_light( 80 );
  add_exit("east", RABBIT + "rabbit4", "road");
  add_exit("west", RABBIT + "rabbit2", "road");
  add_exit("north", RABBIT + "print_shop_foyer", "door");
}
