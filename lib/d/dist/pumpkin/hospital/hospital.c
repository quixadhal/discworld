#include "path.h"

inherit CITY "inherit/hospital";

void create() {
   set_hospital_type("Pumpkin");
   set_save_file("/d/dist/dist/pumpkin_hospital");
   set_max_living(100);
   set_log_directory("/d/dist/log");

   do_setup++;
   ::create();
   do_setup--;

   
   if (!do_setup) {
      set_short("Pumpkin Hospital");
      set_light(60);
   }

   set_npc_groups(DATA->compile_data( ({ HOSPITAL_DIR "bes_pelargic_group" }) ));
   set_npc_info(DATA->compile_data( ({ HOSPITAL_DIR "bes_pelargic_info" }) ));
} /* create() */
