inherit "/std/outside";

#include "path.h"

object *objs;

make_objects() {
  if(!objs) {
    objs = allocate(1);
  }
/* start of object 0 - frog */
  objs[0] = clone_object("/obj/misc/board");
  objs[0]->set_datafile("frog");;
  objs[0]->move(this_object()); /* here */
/* end of object 0 */
}

void make_contents() {
  make_objects();
}


void setup() {
  set_short("A hairy womble");
  set_long(
"You are standing in a hairy womble." +
" The roof is covered in things." + "\n"
);
  set_light(60);
  set_zone("ddt");
  add_exit("east", ROOM+"workroom", "door");
  add_exit("south", "/w/common", "corridor");
  add_item("candle",
 "A small furry candle withyellow things hangig off it." + "\n"
);
  add_alias("common", "south");
}

void reset() {
  make_contents();
}

/* PLEASE ADD ALL ADITIONAL CODE AFTER THIS LINE */

