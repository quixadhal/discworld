#include "path.h"
inherit CITYROOM;
 
#define NUM 1
#define OUTSIDE "/d/sur/FOREST/forest/room/entrance"
 
object guard;
 
void setup() {
  set_light(100);
  set_short("Hub Gate");
  set_long(
"You are at the hubward gate of Ankh-Morpork. You can go west from "+
"here into Ankh-Morpork, or east will take you into the cabbage "+
"fields that surround the city and contribute to its famed odour.\n");
 
  add_exit("west", ROOM+"matthews5", "road");
  add_exit("east", OUTSIDE, "gate");
 
 
  add_item("forest",
     "The forest seems inviting enough.  All sorts of curious plants "+
     "seems to be growing there.  The forest seems to have a slight "+
     "octarine glow to it.\n");

  add_item("mountains",
     "Although they are so far away, you can make out the white "+
     "snowcaps on the tops of the highest peaks.\n");
  add_item("gate",
"This gate leads East out of the city.  In the far distance "+
"you can see the mountains.  On the southern side of the forest, "+
"it melts into a shimmering octarine mist, about which you can see "+
"little detail.\n");
 
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

void dest_me()
{
  if(guard) guard->dest_me();
  ::dest_me();
}
