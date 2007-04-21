#include "path.h";

inherit PLAYER_HOUSE;

void setup() {
   set_light(50);
   set_theft_handler( HOSPITAL );
   set_save_file("%savedir%/%N%diningroom");

/* DESCRIPTIONS */

   set_orientation(%orientation%);
  set_quit_handler(%out%);

   set_short("spacious dining room");
   set_base_desc("a spacious dining room");
   add_surface( "floor", "finished pine board" );
   add_surface( "ceiling", "plain plaster" );
   add_surface( "%0% wall", "plain plaster" );
   add_surface( "%2% wall", "plain plaster" );
   add_surface( "%4% wall", "plain plaster" );
   add_surface( "%6% wall", "plain plaster" );
   
/* EXITS */

   add_exit("%3%", PATH+"%N%livingroom", "corridor");
   add_exit("%2%", PATH+"%N%kitchen", "corridor");
   add_exit("window", PATH+"%N%garden", "window");
}

