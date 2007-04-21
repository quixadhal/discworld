// Rue 2.2000

inherit "/std/room/furniture/basic";

void setup() {
  set_name( "birdbath" );
  set_short( "stone bird bath" );
  add_adjective( ({ "stone", "bird", "bath" }) );
  add_alias( ({ "bath", "bird bath" }) );
  set_long( "This is a block of stone that has been chiseled and hacked into "
   "a vaguely bird bath shape.  It is really quite ugly.  Fortunately, birds "
   "don't seem to be art critics.  The base of the bird bath is made of some "
   "sort of porous stone that resembles Dwarf Bread in texture.\n" );
  set_allowed_positions(({"sitting","standing"}));
  set_allowed_room_verbs((["standing" : "stands" ]));
  set_weight(200);
  set_value(4000);
}

