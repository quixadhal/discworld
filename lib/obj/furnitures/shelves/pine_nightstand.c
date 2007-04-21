// Triana Mar.2000, modified from white_plank_cabinet by
// Rue 1.2000

inherit "/std/room/furniture/storage";

void setup() {
  set_name( "nightstand" );
  set_short( "pine nightstand" );
  add_adjective( ({ "pine", "bedside" }) );
  add_alias("cabinet");
  set_long( "This is a small bedside cabinet, made of thick hublandish "
            "pine planks.  The wood is oiled, to bring out the characteristic "
            "colour, and the stripes and knots texture in it.  It's just the "
            "kind of thing that is thought to have a rustic look, by "
            "people who don't have much of a clue what rustic is.\n" );

  add_drawer( "top", 50 );
  
  // 15 pounds sounds to me like more than enough for a small cabinet :)
  set_weight(135);
  set_value(10000);

  set_allowed_positions(({"sitting"}));
  set_allowed_room_verbs((["sitting" : "sits" ]));
}

void init() {
    string key_id;
  object key;
  
  if(!query_key()) {
    key_id = "nightstand_"+random(100); 
    set_key(key_id);
    set_difficulty(1);
    
    key = clone_object("/std/key");
    key->set_key("small", key_id);
    key->set_long("This small key looks like it is probably for a piece of "
                  "furniture.\n");
    key->move(this_object());
    
    set_closed();
  }

  ::init();
}
