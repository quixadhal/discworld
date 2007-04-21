#include <config.h>

inherit "/std/room/basic_room";

void setup() {
   set_short("entrance");
   set_long("You are standing at the entrance to a big wide, well quite small, "
            "area.  Welcome to the Discworld mud distribution lib entrance.\n");
   set_light(70);

   add_exit("pumpkin", CONFIG_START_LOCATION, "road");
} /* setup() */
