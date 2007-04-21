#include "path.h";
inherit PLAYER_HOUSE;

void setup() {
   set_light(50);
   set_theft_handler( HOSPITAL );
   set_save_file("%savedir%/%N%bed");
   
/* DESCRIPTIONS */
   set_orientation(%orientation%);
   set_quit_handler(%out%);

   set_short("bedroom");
   set_base_desc( "a small bedroom" );
   add_surface( "floor", "stone paving" );
   add_surface( "ceiling", "roughly finished wood" );
   add_surface( "%0% wall", "roughly finished wood" );
   add_surface( "%2% wall", "roughly finished wood" );
   add_surface( "%4% wall", "roughly finished wood" );
   add_surface( "%6% wall", "roughly finished wood" );

/* EXITS */

   add_exit( "%4%", PATH +"%N%livingroom", "door" );
   modify_exit("%4%", ({ "door short", "living room door" }));
   
}

