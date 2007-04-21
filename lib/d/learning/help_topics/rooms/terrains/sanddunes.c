/* -*- LPC -*- */
#include "path.h"

inherit "/std/outside";

void setup() {
   set_terrain( "tutorial_desert" );
   set_short( "sandy wasteland" );
   set_long( "This is just one small section of an endless "
            "expanse of sand.\n" );
   add_item( "sand", 
      "It's sand.  There isn't much to say about it.");
   
   add_property( "climate", ({ 40, -100, -100 }) );
} /* setup() */
