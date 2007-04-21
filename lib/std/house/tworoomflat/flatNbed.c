#include "path.h";
inherit PLAYER_HOUSE;

void setup() {
   set_light(50);
   set_theft_handler( HOSPITAL );
   set_save_file("%savedir%/flat%N%bed");
   
/* DESCRIPTIONS */
   set_orientation(%orientation%);
   set_quit_handler(%out%);

   set_short("bedroom");
   set_base_desc( "a small bedroom" );
   add_surface( "floor", "finished pine board" );
   add_surface( "ceiling", "plain plaster" );
   add_surface( "%0% wall", "plain plaster" );
   add_surface( "%2% wall", "plain plaster" );
   add_surface( "%4% wall", "plain plaster" );
   add_surface( "%6% wall", "plain plaster" );

/* EXITS */

   add_exit( "%6%", PATH +"flat%N%", "door" );
   modify_exit("%6%", ({ "door short", "livingroom door" }));
   
}

