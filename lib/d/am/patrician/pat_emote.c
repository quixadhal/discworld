#include "path.h"
inherit "/std/room";

object man;

void start_quest(object ob);

void reset() {
  if (man)
    return ;
  man = clone_object(CHAR+"roger");
  man->set_name("roger");
  man->set_long(
"A small wirey man with bright sparkling blue eyes.  He is wearing a flowing "+
"red cape with strange looking dragons embroidered on it.  Despite looking "+
"happy and cheerful, his scared face tends to suggest that he has been in "+
"many fearsome battles.\n");
  man->set_race("human");
  man->set_class("fighter");
  man->set_level(50);
  man->move(this_object());
}

void setup() {
  set_light(100);
  set_short("Bare room");
  set_long(
"A bare empty room with a chair standing in the middle of the room.  "+
"The rest of the room is unfurnished.  The walls are white washed and "+
"there is not even any boring wallpaper.\n");
  set_zone("patricians office");

  add_exit("south", ROOM+"patrician3", "door");

  add_item("chair",
           "A small light brown chair made of pine wood.  It looks "+
           "well used.\n");
  add_item(({ "wallpaper", "wall paper" }),
           "There is no wall paper don't you listen?\n");
}

void dest_me() {
  if (man)
    man->dest_me();
  ::dest_me();
}
