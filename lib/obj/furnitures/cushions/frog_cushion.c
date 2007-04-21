// Rue 1.2000

inherit "/std/room/furniture/basic";

void setup() {
  set_name( "cushion" );
  set_short( "frog cushion" );
  add_adjective( ({ "frog", "green" }) );
  add_alias( "pillow" );
  set_long( "This is a cushion that is the approximate shape of a "
    "frog that has been flattened by a cart.  It is covered in vivid "
    "green cordueroy.\n" );
  set_allowed_positions(({"sitting", "lying", "kneeling", "meditating",
                            "crouching", "standing"}));
  set_allowed_room_verbs((["lying" : "lays", "hanging" : "hangs"]));
  set_weight(2);
  set_value(8000);
}

