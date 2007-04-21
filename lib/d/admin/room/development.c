#include "path.h"
inherit "/std/room/basic_room";

object board;

void reset() {
  if(!board) {
    board = clone_object("obj/misc/board");
    board->set_datafile("development");
    board->move(this_object());
  }
}

void setup() {
  set_light(80);
  set_short("Mud Development Office");
  set_long(
    "You are in the Development Office of the mud. Maps and charts of the "+
    "world adorn the panelled walls. Forms sit on a table near the door "+
    "for registration of new projects. A few bored looking clerks rustle "+
    "some papers and try to look busy.\n");
  add_item("clerk", "They seem more interested in coffee than work.\n");
  add_item("map", "Type 'list' to see available maps, and 'read' to look "+
           "at one.\n");
  add_exit("west", "w/common", "door");
  add_exit("east", ROOM+"quest_room", "door");
  add_exit("north", ROOM+"mail_room", "door");
  add_exit("south", ROOM+"domain_control", "corridor");
  add_exit("northeast", ROOM+"cloner_control", "door");
   add_exit( "southeast", ROOM +"terrain", "door" );
}

void dest_me() {
  if (board)
    board->dest_me();
  ::dest_me();
}
