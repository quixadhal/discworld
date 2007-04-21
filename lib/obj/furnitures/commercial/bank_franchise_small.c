// Pinkfish
// Started Wed May 30 21:37:15 PDT 2001

inherit "/std/room/furniture/bank_franchise";

void setup() {
   set_name( "counter" );
   set_short( "small bank counter" );
   add_alias("bank");
   add_adjective( ({ "small", "bank" } ));
   set_long( "A heavily armoured and hardwood counter "
             "with a safe embedded into the surface.\n");
   set_allowed_positions(({"sitting", "standing" }));
   set_allowed_room_verbs((["sitting" : "sits", "standing" : "stands" ]));
   set_weight(1000);
   set_value(400000);
   set_commercial_size(10);
   set_maximum_total_difference(200000);
}
