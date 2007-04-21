#include "path.h";

inherit PLAYER_HOUSE;

void setup() {
   set_light(50);
   set_theft_handler( HOSPITAL );
   set_save_file("%savedir%/flat%N%");
   
/* DESCRIPTIONS */
   set_orientation(%orientation%);
   set_quit_handler(%out%);

   set_short("front room");
   set_base_desc("a small and cosy front room" );
   add_surface( "floor", "finished pine board" );
   add_surface( "ceiling", "plain plaster" );
   add_surface( "%0% wall", "plain plaster" );
   add_surface( "%2% wall", "plain plaster" );
   add_surface( "%4% wall", "plain plaster" );
   add_surface( "%6% wall", "plain plaster" );
   
   /* EXITS */

   add_exit("out", %out%, "door");
   modify_exit("out", ({ "door short", "front door"}));
   add_exit("%2%", PATH+"flat%N%bed", "door");
   modify_exit("%2%", ({ "door short", "bedroom door"}));
   modify_exit("out", ({"exit mess", "$N leaves the flat.",
                         "enter mess",
                           ({ 1,
                              "$N enters from a flat.", 
                              "$N enter from a flat."
                           }),
                       }) );
   modify_exit( "out", ({ "closed", 1 }) );
}

