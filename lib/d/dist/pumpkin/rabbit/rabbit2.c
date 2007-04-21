#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Rabbit Lane");
  set_long("This is the middle of Rabbit Lane where a bunch of "
           "books and things seem to be.\n");
  set_light( 80 );
  add_exit("east", RABBIT + "rabbit3", "road");
  add_exit("west", RABBIT + "rabbit1", "road");
  add_exit("north", RABBIT + "bookshop", "door");
}
