inherit "std/pawn";

#include "path.h"

void setup() {
  set_light(60);
  set_short("Pawn Shop");
  set_long("You are in the pawn shop of Ankh Morpork.  There is a sign on "+
           "the wall describing how it works.\n");
  add_exit("north", ROOM+"filgree8", "door");
  add_item("sign",
"It is a sign.  It has some writing on it.\n");
}
