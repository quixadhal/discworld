#include "path.h";

inherit PLAYER_HOUSE_OUTSIDE;

void setup() {
   set_light(50);
   set_theft_handler( HOSPITAL );
   set_save_file("%savedir%/%N%garden");

   /* DESCRIPTIONS */

   set_orientation(%orientation%);
  set_quit_handler(%out%);

   set_short("small garden");
   set_base_desc("a quiet and secluded garden");
   add_surface("floor", "grass");
   add_surface("%0% wall", "red brick" );
   add_surface( "%2% wall", "red brick" );
   add_surface( "%6% wall", "red brick" );
   
   /* EXITS */

   add_exit("%4%", PATH+"%N%familyroom", "door");
   modify_exit("%4%", ({ "door short", "garden door" }));

   //add_exit("%5% window", PATH+"%N%familyroom", "window");
   add_exit("window", PATH+"%N%diningroom", "window");
}

