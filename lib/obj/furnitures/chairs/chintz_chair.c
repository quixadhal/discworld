// Rue 1.2000

inherit "/std/room/furniture/basic";

void setup() {
  set_name( "chair" );
  set_short( "overstuffed chintz chair" );
  add_adjective( ({ "overstuffed", "chintz" }) );
  set_long("This is the type of fat, comfortable chair typically found "
    "in the parlours of fat, comfortable ladies.  The fabric covering "
    "it is bright yellow decorated with a rather aggressive-looking "
    "rose and cabbage design.\n");
  set_allowed_positions(({ "sitting", "meditating", "lounging" }));
  set_allowed_room_verbs(([
    "sitting" : "sits",
  ]));
  set_weight(400);
  set_value(6000);
} /* setup() */
