#include "path.h"
inherit "/std/room";

object board;

void setup() {
  set_light(100);
  set_short("Never Wending Story room");
  set_long(
"Heaped all over the tables, chairs and other things in this room are "+
"pages and pages of parchment.  On closer inspection they all appear "+
"to be part of a giant story that has been going since the beginning "+
"of time.  There are a few people in this room staring into space "+
"and chewing on pencils.\n");

  set_zone("patricians office");

  add_exit("north", ROOM+"patrician2", "door");

  add_item(({ "pages", "page", "paper", "parchment", "pages of parchment" }),
           "The pages have lots of tiny writing on them, bits of it are "+
           "unreadable and they seem to be in no set order.  The story "+
           "confused to say the least.  You are not sure if it about a "+
           "frog prince or a prince frog or if it about that at all.\n");
  add_item( ({ "people", "person" }),
           "The people are stare vacantly into space and chewing on "+
           "pencils, they appear to be muttering something about "+
           "writers block.\n");
  add_item( ({ "pencil", "pencils" }),
           "You stare rudely at someone's pencil.  It doesn't "+
           "look like it has seen much use apart from being chewed on.\n");
  add_item( ({ "table", "tables" }),
           "The tables are covered in parchments of all shapes and sizes.  "+
           "They have been 'filed' in no particular order.\n");
  add_item( ({ "chair", "chairs" }),
           "The chairs are well, they havent been used for sitting on for "+
           "a while.  The dead rat you find on one of them gives it away.\n");

  board = clone_object("/obj/misc/board");
  board->set_board_name("never wending story");
  board->move(this_object());
}

void dest_me()
{
  if(board) board->dest_me();
  ::dest_me();
}
