/* -*- LPC -*- */
#include "path.h"

inherit "/std/room";

void setup() {
   set_terrain( "tutorial_mountain" );
   set_short( "mountain cabin attic" );
   set_long( "This is the second floor of a comfortable mountain cabin.  "
            "The bed occupying most of this room would seem to indicate "
            "that this is a bedroom.\n" );
   add_item( "bed", ({
      "long", "A big king-sized bed with a nice, thick, comforter on top.",
      "position", "the bed" }), 0 );
   add_item( "comforter",
      "This thick quilted comforter is made of some "
      "soft cotton fabric sandwiching a thick layer of down.");
   
   set_light( 50 );
} /* setup() */