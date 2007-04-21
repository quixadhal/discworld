/* -*- LPC -*- */
#include "path.h"

inherit "/std/outside";

void setup() {
   set_terrain( "tutorial_grassy_field" );
   set_short( "doorstep");
   set_long( "This is the doorstep to a small cottage.  All around "
            "here is a field covered with nicely cut grass.\n" );
   add_item( "grass", "It's grass.  There isn't much to say about it." );
   add_item( "cottage", "A plain, one-room cottage, painted white, "
            "with pink shutters on both the windows.  *hack* *cough*" );
   
   add_exit( "cottage", PATH "foyer", "corridor" );
} /* setup() */
