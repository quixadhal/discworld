#include "path.h"
inherit CITYROOM;
 
#define NUM 1
#define OUTSIDE "/d/sur/PLAIN/entrance"
 
object guard;
 
void setup() {
  set_light(100);
  set_short("City Gates");
  set_long(
"You are at the northeastern gate of Ankh Morpork.  You can go west from "+
"here into Ankh Morpork, or east will take you into an open plain. "+
"You can see the Ramtops in the distance.\n");
 
  add_exit("west", ROOM+"filgree12", "road");
  add_exit("east", OUTSIDE, "gate");
 
  add_item("ramtops",
"These are the mountains on the hub of Discworld.  There is "+
"one giant mountain in the middle that is said to be the home "+
"of Discworld's gods.\n");
 
  add_item("forest",
     "A somewhat dark forest lies to the south of the plain. "+
     "The forest is flanked by the mountains to the northeast.\n");

  add_item("mountains",
     "Although they are so far away, you can make out the white "+
     "snowcaps on the tops of the highest peaks.\n");
  add_item("gate",
"This gate leads East out of the city.  In the far distance "+
"you can see the Ramtops.  There is a forest on the southern side "+
"the plain that lies to the east of here.\n");
 
  add_item("plain",
"This plain is flanked by mountains on the southern and western sides, "+
"and a swamp on its western side. It is quite desolate.\n");
 
  add_item("swamp",
      "A dark swamp rises out of the plain, following the Ankh River.\n");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
 
void reset() {
  ::reset();
  if(!guard) {
    guard = (object)"/d/am/hospital"->get_monster("cityguard");
    guard->move(this_object());
  }
}
