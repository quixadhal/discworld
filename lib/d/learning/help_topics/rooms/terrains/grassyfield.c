/* -*- LPC -*- */
#include "path.h"

inherit "/std/outside";

void setup() {
   set_terrain("tutorial_grassy_field");
   set_short("grassy field");
   set_long("This field is covered with nicely mowed grass.\n");
   add_item("grass", "It's grass.  There isn't much to say about it.");
} /* setup() */
