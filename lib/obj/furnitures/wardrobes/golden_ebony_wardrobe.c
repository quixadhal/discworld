inherit "/std/room/furniture/storage";

void setup() {
    set_name("wardrobe");
    set_short("golden ebony wood wardrobe");
    add_adjective(({"golden", "ebony", "wood"}));
    set_long("Tall and imposing this wardrobe looms towards the ceiling.  "
        "The crafter obviously gave a great deal of thought to its "
        "construction in selecting different woods to present constrasts "
        "and blends.  The darkest of ebony seems to swallow the light "
        "highlighting minature scrollwork picked out in gold across its "
        "surface, giving way to dusky madrona the spirals and knots give "
        "the impression of smoke curling slowly around the borders of "
        "the doors and around the brass handles.\n");

    set_weight(900);
    set_max_weight(500);
    set_value(740000);

    set_allowed_room_verbs((["squatting" : "squats" ]));
    set_pick_skill("covert.lockpick.doors");
    set_difficulty(4);
} /* setup() */

void init() {
    string key_id;
    object key;
  
    if(!query_key()) {
        key_id = "furniture_lock_"+random(1000); 
        set_key(key_id);
    
        key = clone_object("/std/key");
        key->set_key("small iron", key_id);
        key->set_long("This small, yet sturdy, key looks like it is for a "
                  "piece of furniture.\n");
        key->move(this_object());
        set_closed();
    }
  
    ::init();
    
} /* init() */
