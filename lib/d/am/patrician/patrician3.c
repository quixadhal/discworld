#include "path.h"
inherit "/std/room";

void setup() {
  set_light(100);
  set_short("Corridor in the Patrician's Office");
  set_long(
"The end of a not overly long corridor.  There are doors to the north "+
"and south.  The corridor vanishes off to the east.  To the east is "+
"the inner and quite private bits of the Patrician's office."+
"  There are doors to the north and south.\n");
  set_zone("patricians office");

  add_exit("east", ROOM+"patrician2", "corridor");
  add_exit("north", ROOM+"pat_emote", "door");
  add_exit("south", ROOM+"pat_flame", "door");

  add_item("north south door",
           "Solid looking doors leading to untold somethings.  Open them "+
           "to find out.  Oh yes, they look open.\n");
  add_item("corridor",
           "Its great.  You have never seen a corridor of its like before, "+
           "you wonder why they don't make all corridor's like this.  "+
           "The fish wall paper really makes the place.  Great ambiance.  "+
           "You really fell like you are in a corridor.\n");
  add_item("fish wallpaper",
           "Its a really realistic looking scene of fish swiming around "+
           "in a river.  One thing you know for sure this is the ankh "+
           "river, you can see through it.\n");
}
