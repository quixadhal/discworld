#include "path.h"
inherit "/std/room";

object board;

void setup() {
  set_light(50);
  set_short("Flame room");
  set_long(
"A small dimly lit room decorated in black.  There is a large notice "+
"board covering the back wall covered in small notes.  Several of the "+
"walls advertise good assassins.  The remains of a coffee table is in "+
"the middle of the room.\n");

  set_zone("patricians office");

  add_exit("north", ROOM+"patrician3", "door");

  add_item("note",
           "The board is covered in them, most of them seem pretty "+
           "boring like 'Harry is a git' and 'Sally is a post office "+
           "frog'.  Some of them catch your eye.  Read them on the "+
           "board.\n");
  add_item("coffee table",
           "The tattered remains of a coffee table in the middle of "+
           "floor.  It looks like many people have had arm wrestles "+
           "over it, or used it in a fight.  Ignoring the blood you "+
           "see one magazine still sitting on it.\n");
  add_item("magazine",
           "A small magazine called 'War and how to win' is stuck "+
           "to the coffee table.  No matter how hard you try you "+
           "cannot move it or even open it, the matted blood and hair "+
           "seems to stop you.\n");

  board = clone_object("/obj/misc/board");
  board->set_board_name("flame_board");
  board->move(this_object());
}

void dest_me() {
  if (board)
    board->dest_me();
  ::dest_me();
}
