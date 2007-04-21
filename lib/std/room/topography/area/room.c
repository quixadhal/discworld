/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: room.c,v 1.3 2001/05/10 22:44:58 pinkfish Exp $
 *
 *
 */

/*
** room.c - area implementation of /std/room.
*/

inherit "/std/room/basic_room";
inherit "/std/room/inherit/topography/aroom";

//
// Need to fix up the inherited functions.
//

mixed* query_dest_other(string direc) {
   return aroom::query_dest_other(direc, basic_room::query_dest_other(direc));
} /* query_dest_other() */

string query_look(string direc) {
   return aroom::query_look(direc, basic_room::query_look(direc));
} /* query_dest_other() */

void create() {
   set_not_replaceable(1);
   ::create();
} /* create() */

string query_room_letter() {
   return "b";
} /* query_room_letter() */
