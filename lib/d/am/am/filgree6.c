#include "path.h"
inherit CITYROOM;
 
#define NUM 2
 
setup() {
  set_light(100);
  set_short("Filgree Street");
  set_long(
"You are in the middle of Filgree Street.  "+
"Over to the east there is a small disreputable looking building with "+
"a small sign out the front.  From the looks of all the drunk "+
"people staggering around the street, it must be the pub.\n" );
 
  add_item("sign", "It reads - The Mended Drum.\n");

  add_exit("east", ROOM+"filgree7", "road");
  add_exit("west", ROOM+"filgree5", "road");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
