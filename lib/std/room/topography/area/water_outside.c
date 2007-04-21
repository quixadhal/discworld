/*
 * water_inside.c - area implementation of /std/room/water_inside.
 */

inherit "/std/room/water_outside";
inherit "/std/room/inherit/topography/aroom";

//
// Need to fix up the inherited functions.
//

mixed* query_dest_other(string direc) {
   mixed*fluff;

   fluff =  water_outside::query_dest_other(direc);
printf("%O\n", fluff);
   return aroom::query_dest_other(direc, fluff);
} /* query_dest_other() */

string query_look(string direc) {
   return aroom::query_look(direc, water_outside::query_look(direc));
} /* query_dest_other() */

void create() {
   set_not_replaceable(1);
   ::create();
} /* create() */

string query_room_letter() {
   return "~";
} /* query_room_letter() */

