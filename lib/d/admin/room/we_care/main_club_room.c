#include "path.h"

inherit "/std/room/club_room";

void setup() {
   set_short("main club room");
   set_long("The main club room.\n");
   set_club_controller(CONTROLLER);
   set_light(70);
   set_leave_exit("south");

   add_exit("south", "/d/admin/room/meeting", "door");
} /* setup() */
