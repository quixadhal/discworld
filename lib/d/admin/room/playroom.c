#include <config.h>

inherit "/std/room/basic_room";

object board;

void reset() {
  if(!board) {
    board = clone_object("obj/misc/board");
    board->set_datafile("playerhelp");
    board->move(this_object());
  }
}

void setup() {
  set_light(60);
  set_short("Player Help Room");
  set_long(
"You are in a strange room with hundreds of frozen statues in the likenesses "+
"of many players on the DiscWorld.  They all seem to be begging, and "+
"looking at you with mournful eyes.\n");
  add_item(({"player","statue"}),
"They all look as if they are asking you questions, but without "+
"moving their lips.  Some of them look genuinly hopeless, but most "+
"have the look of treachery and deceit.  Quite a few of them have many "+
"faces looking in several directions.  Beware them all.\n");
  add_exit("east", "/w/common", "door");
}

void dest_me() {
  if (board)
    board->dest_me();
  ::dest_me();
}

void event_enter(object ob) {
  if (!ob->query_creator() && !ob->query_property("demon")) {
      tell_object(ob, "Whoops!  You don't seem to be a creator.\n");
      call_out("do_move", 2, ob);
   }
} /* event_enter() */

void do_move(object ob) {
   tell_object( ob, "A trap door opens in the floor and you fall through!\n" );
   ob->move_with_look( "/d/am/am/mendeddrum", "$N fall$s through a trap "+
         "door in the ceiling.", "A trap door opens in the floor and "+
         "$N fall$s through." );
} /* do_move() */
