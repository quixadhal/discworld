/* -*- LPC -*- */
#include "path.h"

#include <terrain.h>

inherit "/std/outside";

void setup() {
   set_room_size( 20 );
   set_terrain( "tutorial_mountain" );
   set_short( "tree in the mountains" );
   set_long( "This tree has defied all the odds, and grows straight and "
            "tall in the blustery wastes." );
   add_extra_look( this_object() );
   
   add_property( "climate", ({ -40, 50, 40 }) );
} /* setup() */

string extra_look() {
   if ( co_ord[ 2 ] < 800200 )
      return "\n";
   else
      return "  You can just make out a cabin in the snows to your "
             "northeast.\n";
} /* extra_look() */
