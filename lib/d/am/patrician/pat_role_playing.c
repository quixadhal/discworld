#include "path.h"
inherit "/std/room";

object board;

void setup() {
  set_light(100);
  set_short("RPG room");
  set_long(
"A nice big room with maps and strange figure things scattered over hundreds "+
"of large tables.  There is a large bulletin board hanging on the east wall.  "+
"Gathered around the tables are several people staring intently at "+
"pages filled with obscure numbers and arguing about the chance of being "+
"hit with a insta-death cannon at point blank range.\n");

  add_exit("south", ROOM+"patrician2", "door");

  add_item("large old cute maps",
           "Large maps of strange looking places are all over the "+
           "walls, tables and floor.  They look well used with coloured "+
           "pencil marks all over them.  The some of the ones on the "+
           "table have small miniature people and horses on them.\n");
  add_item(({ "strange figure", "thing", "miniature" }),
           "Some of the maps have small miniature horses and people "+
           "engaged in miniature battles.  The blood looks quite "+
           "realistic, you are impressed.\n");
  add_item("table",
           "The tables are covered in maps, what little you can see of "+
           "them appears to be made of a dark wood.  They all appear "+
           "well used with numerous coffee stains all over them.\n");
  add_item("person",
           "The people are all gathered around the tables involved "+
           "in the terriblely complex games being played.  They ignore "+
           "you completely.\n");
  board = clone_object("/obj/misc/board");
  board->set_board_name("role_playing");
  board->move(this_object());
}
void dest_me()
{
  if(board) board->dest_me();
  ::dest_me();
}
