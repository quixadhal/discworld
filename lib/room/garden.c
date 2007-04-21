#include <config.h>

inherit "/std/room/basic_room";

void setup() {
  set_light(70);
  set_short("Statue garden");
  set_long(
"This is a nice garden.  Scattered amongst the garden you notice amazingly "+
"accurate statues of people.\n");

  add_exit("drum", CONFIG_START_LOCATION, "path");

  add_item("garden",
           "It is a lovely garden, nice little walk ways drifting off into "+
           "the distance.  Lovely little sheltered groves and grotos.");
  add_item("statue",
           "Very life like statues of people.  Amazing what they can do "+
           "these days.");
}
