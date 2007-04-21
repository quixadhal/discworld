inherit "/std/room/inherit/club_owned_room";
#include "path.h"

void setup() {
   set_short("The first owned room");
   set_long("A room that is owned.\n");

   set_club_controller(CLUB_CONTROLLER);
   set_club_owned_id("main office");
} /* setup() */
