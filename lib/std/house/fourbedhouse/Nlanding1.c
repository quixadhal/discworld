#include "path.h";
inherit PLAYER_HOUSE;

void setup() {
   set_light(50);
   set_theft_handler( HOSPITAL );
   set_save_file("%savedir%/%N%landing1");

/* DESCRIPTIONS */

   set_orientation(%orientation%);
  set_quit_handler(%out%);

   set_short("landing");
   set_base_desc( "a landing at the top of the stairs" );
   add_surface( "floor", "finished pine board" );
   add_surface( "ceiling", "plain plaster" );
   add_surface( "%0% wall", "plain plaster" );
   add_surface( "%4% wall", "plain plaster" );
   add_surface( "%6% wall", "plain plaster" );

/* EXITS */

   add_exit( "%0%", PATH +"%N%masterbed", "door" );
   modify_exit("%0%", ({ "door short", "bedroom door" }));

   add_exit( "%4%", PATH +"%N%bed2", "door" );
   modify_exit("%4%", ({ "door short", "bedroom door" }));


   add_exit("%2%", PATH+"%N%landing2", "corridor");

   add_exit("down", PATH+"%N%livingroom", "corridor");
   modify_exit("down", ({"downgrade", 9 }));
}

