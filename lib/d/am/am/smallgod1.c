#include "path.h"
inherit CITYROOM;
 
#define NUM 2
 
void setup() {
  set_light(100);
  set_short("Small Gods Street");
  set_long(
"You are on Small Gods Street.  To the north is Filgree Street and "+
"Small Gods Street continues south to the entrance to the Temple of " +
"Small Gods.  To the east a huge building towers, it has a huge sign out "+
"the front that reads \"Fred's Emporium and Clothing Shop\".\n");
 
  add_exit("north", ROOM+"filgree4", "road");
  add_exit("south", ROOM+"smallgod2", "road");
  add_exit("east", ROOM+"clothing", "door");
 
  add_item("temple",
"It is the Temple of Small Gods.  It is covered in small statues.\n");
  add_alias("temple of small gods", "temple");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
