#include <npc/transport.h>
inherit "/obj/monster/transport/horse_base";

void setup() {
   set_race("horse");
   set_level(20);
   set_name("horse");
   set_gender( query_mount_gender() );
   set_long( "A nice dark dashing horse, " + query_pronoun() + " looks "
             "strong and happy as its brown eyes stare into your own.\n" );
   set_inside_long( "You can see the horses hair and ear's flicking at the "
                    "slightest sounds.  The smell of horse is quite "
                    "strong.\n" );

   // 10 royals in value
   set_value(40000 * 10);
   set_maximum_speed(RIDE_GALLOP);
   set_transport_type("horse");
   set_default_mount_difficulty(100);
   set_default_direction_difficulty(100);
   set_wander_time( 40 + random(10) );
   set_transport_speed(2);
   set_maximum_exhaustion(300);
} /* setup() */
