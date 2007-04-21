inherit "/std/room/furniture/surface";

void setup() {
  set_name("stove");
  set_short("small stove");
  add_adjective("small");
  set_long("This is a small stove.\n");
  set_allowed_room_verbs(([
    "sitting" : "sits",
    "squatting" : "squats",
  ]));
  set_weight(495);
  set_max_weight(45);
  set_value(8000);
} /* setup() */
