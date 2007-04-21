inherit "/std/room/furniture/bath";

void setup() {
  set_name("bathtub");
  set_short("cast iron bathtub");
  add_adjective(({"cast", "iron"}));
  add_alias( ({ "tub", "bath" }) );
  set_long("The white porcelain sparkles on this cast iron bathtub, it looks "
           "heavy but large enough for a couple of people to have a nice "
           "bath in.\n");
  set_allowed_positions( ({"sitting", "standing", "lying" }) );
  set_allowed_room_verbs((["sitting" : "sits", "standing" : "stands" ]));
  set_weight(350);
  set_value(14000);
}
