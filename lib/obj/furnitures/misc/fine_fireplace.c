// Aquilo //
/* This fireplace uses all the default settings in the inherit as 
   those are pretty nifty
*/

inherit "/std/room/furniture/fireplace";

void setup(){
  /* Functions below can be called in the setup of the inheriting 
     fireplace to over-ride these and modify its appearance.
     See help on each function for further info.
   */
  set_allowed_positions(({"sitting"}));
  set_allowed_room_verbs((["sitting" : "sits" ]));
  set_weight(700);
  set_max_weight(100); // About 4 logs worth

  set_short("finely crafted fireplace");
  set_long("This fireplace is surrounded by an ornately carved mahogany "
     "framework and has a beautifully finished brass hearth.\n");
  set_value(900000);
}
