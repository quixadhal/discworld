inherit "/std/room/furniture/storage";

void setup() {
  set_name("safe");
  set_short("small iron safe");
  add_adjective(({"small", "iron"}));
  set_long("This is a small iron safe.  It bears the logo of "
           "Joseph Thighbiter a renowned safe maker.  It appears "
           "to feature one of his patented locks.\n");

  add_drawer("top", 50);
  add_drawer("middle", 25);
  add_drawer("bottom", 25);

  // the drawers are inside and not visible if the door is shut.
  set_drawers_inside(1);
  
  set_weight(900);
  set_value(120000);

  set_allowed_room_verbs((["squatting" : "squats" ]));
  set_pick_skill("covert.lockpick.safes");
  set_difficulty(4);

  setup_trap(300, "pick",
             "Hidden inside the lock is a small ampule which, if touched by "
             "an unwary tool would trigger a puff of poisonous gas.",
             ({"A small cloud of gas puffs out of the safe.\n",
                 "A small cloud of gas puffs out of the safe.\n"}),
             ({"/std/effects/poisons/poison_gas", 600}));
}

void init() {
  string key_id;
  object key;
  
  if(!query_key()) {
    key_id = "safe"+random(10000); 
    set_key(key_id);
    
    key = clone_object("/std/key");
    key->set_key("small iron", key_id);
    key->set_long("This small, yet sturdy, key looks like it is for a "
                  "safe.\n");
    key->move(this_object());
    set_closed();
  }
  

  ::init();
}
