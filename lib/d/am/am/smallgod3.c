#include "path.h"
inherit CITYROOM;
 
#define NUM 2
 
void setup() {
  set_light(100);
  set_short("Small Gods Street");
  set_long(
"You are on Small Gods Street.  From here it leads north and east. "+
"To the north is the entrance to the Temple of Small Gods.\n");
 
  add_exit("north", ROOM+"smallgod2", "road");
  add_exit("east", ROOM+"smallgod4", "road");
 
  add_item("temple",
"It is the Temple of Small Gods.  It is covered in small statues.\n");
  add_alias("temple of small gods", "temple");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
