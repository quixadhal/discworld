#include "path.h"
inherit CITYROOM;
 
#define NUM 1
 
void setup() {
  set_light(100);
  set_short("Matthews Street");
  set_long(
"You are on Matthews street.  To the west is Short Street and to the "+
"east Matthews street continues.  North of you is the side of the "+
"Watches building.\n");
 
  add_exit("west", SHORT+"short6", "road");
  add_exit("east", ROOM+"matthews2", "road");
 
  add_item("building",
"It is the side of the Watch Building.  The entrance is from Short Street.\n");
  add_alias("watch", "building");
  add_alias("office", "building");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
}
