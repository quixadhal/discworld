/* A pumpkin chest.
 * by Ptoink
 *  - 05/02/2003
*/

inherit "/std/room/furniture/storage";

void setup() {

  set_name("chest");
  set_short("pumpkin chest");
  add_adjective(({"pumpkin","fairytale"}));

  set_long(
    "Shaped and painted like a perfectly ripe pumpkin, this chest is "
    "held just off the ground by four sturdy wooden wheels.  "
    "Unfortunately, these appear to be merely decorative in nature, and "
    "don't actually turn.  A deep green stem has been attached to the "
    "top of the chest to act as a handle.\n");
    
  set_weight(1200);
  set_max_weight(200);
  set_value(590000);
  set_allowed_room_verbs((["squatting" : "squats", 
                               "sitting" : "sits"
  ]));    
}