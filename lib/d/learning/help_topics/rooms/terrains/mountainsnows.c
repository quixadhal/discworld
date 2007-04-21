/* -*- LPC -*- */
#include "path.h"

inherit "/std/outside";

void setup() {
   set_terrain( "tutorial_mountain" );
   set_short( "the snowy wastes" );
   add_property( "determinate", "" );
   set_long( "This snow covered land is swept by strong winds night and "
            "day.  No trees can stand the harsh climate.\n" );
   add_item( "tree", "No, I said there _weren't_ any trees." );
   
   add_property( "climate", ({ -40, 50, 40 }) );
} /* setup() */
