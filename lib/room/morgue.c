#include <config.h>
inherit "/std/room/basic_room";

void setup() {
   set_short( "morgue");
   add_property( "determinate", "the " );
   add_property( "no teleport", 1 );
   set_light( 100 );
   set_long("Lots of bodies stacked end on end in long rows.  It looks "
            "like they have stopped sleeping.\n");
} /* setup() */

int *query_co_ord() { return 0; }

void clean_up(int i) { return; }
