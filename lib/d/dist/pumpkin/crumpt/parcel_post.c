#include "path.h"

inherit "/std/room/parcel_inherit";

void setup() {
   set_short("Parcel Post");
   set_long(
"Livid orange strips criss cross the room contrasting just beautifully "
"against the black background.  The walls almost seem to be pulsing with "
"colour.\n");
   set_light(70);

   add_exit("south", CRUMPT + "post_office", "south");

   add_property("place", "Pumpkin");
} /* setup() */
