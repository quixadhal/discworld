#include "path.h";
inherit PLAYER_HOUSE;

void setup() {
   set_light(50);
   set_theft_handler( HOSPITAL );
   set_save_file("%savedir%/flat%N%bed");

/* DESCRIPTIONS */

   set_orientation(%orientation%);
  set_quit_handler(%out%);

   set_short("hallway");
   set_base_desc( "a hallway" );
   add_surface( "floor", "finished pine board" );
   add_surface( "ceiling", "plain plaster" );
   add_surface( "%0% wall", "plain plaster" );
   add_surface( "%2% wall", "plain plaster" );
   add_surface( "%4% wall", "plain plaster" );

/* EXITS */

   add_exit( "%0%", PATH +"%N%bed4", "door" );
   modify_exit("%0%", ({ "door short", "bedroom door" }));

   add_exit( "%4%", PATH +"%N%bed3", "door" );
   modify_exit("%4%", ({ "door short", "bedroom door" }));

   add_exit( "%6%", PATH +"%N%landing1", "corridor" );

}

