// Rue - with help from Aquilo, 4.2000 //

inherit "/std/room/furniture/surface";

void setup(){
  set_name( "stand" );
  set_short( "wrought iron hat stand" );
  add_adjective( ({ "wrought", "iron", "hat" }) );
  add_alias( ({ "hatstand", "rack", "hat rack", "hatrack" }) );
  add_plural( "wrought iron hat stands" );
  set_long("This is a solid hat stand made of gracefully designed "
    "wrought iron.  It rests on four clawed feet, while the base "
    "of the stand wraps around itself in a spiral.  There are "
    "plenty of hooks at the top.\n");
  set_weight(250);
  set_max_weight(200);
  set_allowed_room_verbs(([ "standing" : "stands" ]));
	set_put_aliases( ({ "hang" }) );
  set_value(9000);

}

