// Pinkfish
// Started Wed May 30 21:37:15 PDT 2001

inherit "/std/room/furniture/bank_master";

void setup() {
   set_name( "counter" );
   set_short( "master bank counter" );
   add_alias("bank");
   add_adjective( ({ "master", "bank" } ));
   set_long( "A heavily armoured and hardwood counter "
             "with a safe embedded into the surface.\n");
   set_allowed_positions(({"sitting", "standing" }));
   set_allowed_room_verbs((["sitting" : "sits", "standing" : "stands" ]));
   set_weight(1000);
   set_value(16000000);
   set_commercial_size(91);

   set_shop_use_types(({ "bank", "withdraw", "deposit" }));
}
