/* -*- LPC -*- */
#include "path.h"

inherit "/std/outside";

void setup() {
   set_terrain( "tutorial_desert" );
   set_short( "desert oasis" );
   set_long( "The trees in this oasis provide welcome relief from the harsh "
            "sunlight of the desert.\n" );
   add_item( "sand", 
      "It's sand.  There isn't much to say about it other than "
      "that it's much moister here than elsewhere in the desert." );
   add_item( "tree", 
      "The palm trees grow majestically all around you, "
      "providing all the shade you could want." );
   
   add_property( "climate", ({ 10, -100, -100 }) );
} /* setup() */
