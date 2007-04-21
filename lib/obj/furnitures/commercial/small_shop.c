// Pinkfish
// Started Wed May 30 21:37:15 PDT 2001

inherit "/std/room/furniture/shop_base";

void setup() {
   set_name( "counter" );
   set_short( "small shop counter" );
   add_adjective( ({ "small", "shop" } ));
   set_long( "The smooth polished surface of the small counter shines and "
             "glows at you.\n");
   set_allowed_positions(({"sitting", "standing" }));
   set_allowed_room_verbs((["sitting" : "sits", "standing" : "stands" ]));
   set_weight(1000);
   set_value(480000);
   set_maximum_inventory_size(50);
   set_commercial_size(40);
}
