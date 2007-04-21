#include <npc/transport.h>
inherit "/obj/monster/transport/camel_base";

void setup() {
   set_race("camel");
   set_level(20);
   set_name("camel");
   set_long( "A ratty old camel with seemingly more knees than it should "
             "have.\n");
   set_inside_long( "Nothing smells worse than a camel.  Not even something "
                    "from Dibbler's tray.\n");

   // 12 royals in value ( they're exotic )
   set_value(40000 * 12);
   set_maximum_speed(RIDE_GALLOP);
   set_transport_type( "camel" );
   set_default_mount_difficulty(150);
   set_default_direction_difficulty(150);
   set_wander_time( 60 + random(20) );
   set_gender( 1 + random(2) );
   set_transport_speed(1);
   set_maximum_exhaustion(400);
  set_transport_colour("off-yellow");
} /* setup() */
