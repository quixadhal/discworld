#include "path.h"
inherit CITYROOM;
 
#define NUM 2
 
void setup() {
  set_light(100);
  set_short("Outside the post office");
  set_long(
"You are on Filgree Street.  Filgree street continues "+
"to the east and west.  A shabby, moderately large building to the south has the "+
"logo 'Ankh-Morpork Post.  We Deliver' in large beaurocratic "+
"letters emblazened on the door.\n");
 
  add_exit("east", ROOM+"filgree10", "road");
  add_exit("west", ROOM+"filgree8", "road");
  add_exit("south", POST_OFFICE, "door");
 
  set_zone("ankh morpork");
 
  set_monster(NUM, "city");
  add_item("building", "The building houses the GPO for Ankh-Morpork.\n");
  add_item("door", "This is the entrance to the Ankh-Morpork General Post "+
"Office.  It has the Postal Company's logo emblazened on it.\n");
  add_item("logo", "The logo comprises the phrase \"Ankh-Morpork Post.  "+
"We Deliver.\" circling a stylized picture of a chicken poised for flight.\n");
}
