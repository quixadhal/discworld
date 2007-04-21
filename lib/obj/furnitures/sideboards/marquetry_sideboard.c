inherit "/std/room/furniture/surface";

void setup() {
  set_name( "sideboard" );
  set_short( "marquetry sideboard" );
  add_alias( "table" );
  add_adjective( ({ "marquetry"}) );
  set_long("This is a long table specifically designed to rest "
    "against a wall.  The marquetry effect on the top surface has been "
    "arranged to represent the Assassins' Guild coat of arms. \n");
  set_allowed_positions(({ "sitting", "lying", "squatting", "standing"}));
  set_allowed_room_verbs((["sitting" : "sits", "standing" : "stands"]));
  set_weight(360);
  set_max_weight(200);
  set_value(100000);
  set_allow_use_internals(1);    
} /* setup() */


