#include "path.h"
inherit CITYROOM;
 
#define NUM 4
#define TEMPLE "/d/am/priest/templeofsg"
 
void setup() {
  set_light(100);
  set_short("Small Gods Street outside Temple");
  set_long(
"You are on Small Gods Street outside the Temple of Small Gods.  "+
"A large gateway to the west leads into the Temple.  The "+
"street continues north and south.\n");
 
  add_exit("north", ROOM+"smallgod1", "road");
  add_exit("south", ROOM+"smallgod3", "road");
  add_exit("west", TEMPLE, "gate");
 
  add_item("temple",
"It is the Temple of Small Gods.  It is covered in small statues.\n");
  add_alias("temple of small gods", "temple");
  add_item("gateway",
"This is a large wrought-iron set of gates which have been open for so many years "+
"that they appear to have rusted permanently open.  Various and sundry "+
"religious symbols are situated over the top of the gateway.\n");
  add_alias("gate", "gateway");
  add_alias("gates", "gateway");
  add_item("symbols", "From the look of it the many different symbols are all "+
"collected from the myriad of small obscure gods that are worshipped "+
"in Ankh-Morpork.\n");
  add_alias("symbol", "symbols");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
