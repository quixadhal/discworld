/*
 * woodland.c - area implementation of /std/room/woodland
 */

inherit "/std/outsides/woodland";
inherit "/std/room/inherit/topography/aroom";

//
// Need to fix up the inherited functions.
//

mixed* query_dest_other(string direc) {
   return aroom::query_dest_other(direc, woodland::query_dest_other(direc));
} /* query_dest_other() */

string query_look(string direc) {
   return aroom::query_look(direc, woodland::query_look(direc));
} /* query_dest_other() */

void create() {
   set_not_replaceable(1);
   ::create();
} /* create() */

string query_room_letter() {
   return "%";
} /* query_room_letter() */
