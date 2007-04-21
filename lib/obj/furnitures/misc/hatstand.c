// Aquilo //

inherit "/std/room/furniture/surface";

void setup(){
  set_name("stand");
  set_short("tall hat stand");
  add_adjective( ({"tall", "hat"}) );
  add_alias( "hatstand" );
  add_plural("tall hat stands");
  set_long("This is a tall, serious looking hatstand.  It is made of "
    "dark sturdy teak wood and stands proud on four lavishly decorated "
    "legs.  The top has been carved into the grand shape of A'Tuin, "
    "upon which the elephants and the Disc have been elaborately "
    "sculpted, capturing the true beauty of the scene.  The turtle's "
    "flippers extend outwards and are where the coats and hats "
    "are hung.\n");
  set_weight(150);
  set_max_weight(200);
  set_allowed_room_verbs((["standing" : "stands",
                           "looming"  : "looms"  ]));
	set_put_aliases(({"hang"}));
  set_value(75000);

}

