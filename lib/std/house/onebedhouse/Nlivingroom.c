#include "path.h";

inherit PLAYER_HOUSE;

void setup() {
   set_light(50);
   set_theft_handler( HOSPITAL );
   set_save_file("%savedir%/%N%livingroom");

/* DESCRIPTIONS */
   set_orientation(%orientation%);
   set_quit_handler(%out%);
   set_short("livingroom");
   set_base_desc("a comfortable livingroom");
   add_surface("floor", "finished pine board" );
   add_surface("ceiling", "plain plaster" );
   add_surface("%0% wall", "plain plaster");
   add_surface("%2% wall", "plain plaster");
   add_surface("%4% wall", "plain plaster");
   add_surface("%6% wall", "plain plaster");
   
   /* EXITS */
   add_exit("%4%", PATH+"%N%hall", "door");
   modify_exit("%4%", ({"door short", "hallway door"}));
   add_exit("up", PATH+"%N%bed", "stair");
   modify_exit("up", ({ "upgrade", 9 }));
}

