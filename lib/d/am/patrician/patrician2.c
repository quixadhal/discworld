#include "path.h"
inherit "/std/room";

void setup() {
  set_light(100);
  set_long(
"A small corridor running through the bunch of rooms that make up the "+
"Patrician's Office.  There are several people wandering around in these "+
"even if they are not rushing, they do appear to be going somewhere.  "+
"To the north and south you can see some doors, the corridor continues "+
"to the west and the waiting room is to the east.\n");
  set_short("Patrician's corridor");

  add_exit("west", ROOM+"patrician3", "corridor");
  add_exit("east", "/d/admin/room/patrician", "corridor");
  add_exit("north", ROOM+"pat_role_playing", "door");
  add_exit("south", ROOM+"pat_neverw", "door");
  set_zone("patricians office");

  add_item("several people",
           "They are slowly meandering to somewhere.  They don't appear "+
           "to be in any great hurry, obviously things don't happen "+
           "very quickly here.\n");
  add_item("north south door",
           "Well, it is very door like, made of wood stuff and generaly "+
           "you would even swear it was a door.  It has a small door "+
           "handle and looks open.\n");
  add_item("waiting room",
           "You can make out lots of frenzyed looking clerks rushing "+
           "around in this room.  It is quite a contrast to where "+
           "you are now standing.\n");
  add_item("corridor",
           "It is a long sterile looking corridor, it goes east west "+
           "through the office, you kind of like the wall paper.\n");
  add_item(({ "wallpaper", "wall paper" }),
           "Wonderful non puce pictures of fish swiming around in a "+
           "river.  It seems almost too life like, hmm did that one "+
           "move?\n");
}
