#include "path.h"
inherit "std/room";
 
#define OUTSIDE "d/am/am/short5"
 
void setup() {
  set_light(60);
  set_short("Office of The Watch");
  set_long(
"The Watch office for the town.  This place looks like it has seen better "+
"days.  The peeling paint and the doors coming off their hinges are a "+
"dead giveaway.  The place looks pretty much unattended.\n");
 
  add_exit("west", OUTSIDE, "door");
 
  set_zone("watch office");
}
