#include "path.h"
inherit CITYROOM;
 
#define NUM 1
#define OUTSIDE "/d/sur/JUNGLE/entrance" 
 
object guard;
 
void setup() {
  set_light(100);
  set_short("City Gates");
  set_long(
"You are at the northern gate of Ankh Morpork.  You can go south from "+
"here into Ankh Morpork, or north will take you into a dense jungle. "+
"You can see the Ramtops far to the north and east. "+
"There appears to be a stable on the street to the south.\n");
 
  add_exit("south", ROOM+"filgree1", "road");
  add_exit("north", OUTSIDE, "gate");
 
  add_item("ramtops",
"These are the mountains on the hub of Discworld.  There is "+
"one giant mountain in the middle that is said to be the home "+
"of Discworld's gods.\n");
 
  add_item("gate",
"This gate leads North out of the city.  In the far distance "+
"you can see the Ramtops.  You can see nothing but thick jungle"+
"between the city and the Ramtoops.  However, a small trail runs "+
"north from here.\n");
 
  add_item("jungle",
"You study the most thickly-vegetated jungle you have ever seen. "+
"You can hear strange noises coming from the jungle, some natural, "+
"some rather odd.\n");
 
  add_item("stable",
"It is a stable.  There are some horses tied up outside.\n");
 
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
