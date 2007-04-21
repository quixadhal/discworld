inherit "/std/room/furniture/surface";

void setup() {
  set_name("lectern");
  set_short("oak lectern");
  add_adjective(({"oak", "ornate"}));
  set_long("The lectern has a heavy wooden base carved with"
					 " twining leaves and a flat top perfect for a book.\n");
  set_allowed_positions(({"sitting"}));
  set_allowed_room_verbs((["sitting" : "sits", "standing" : "stands" ]));
  set_weight(350);
  set_max_weight(50);
  set_value(1400);
}
