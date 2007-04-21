#include "path.h";

inherit "/std/rooftop";

void setup() {
  set_light(100);
  set_short("half-roof");
  set_long("This is a half-roof, a piece of sloping roof halfway up the "
           "front of the house.  There is a window to the %2% and another "
           "to the %6%.\n");

  set_slope(30, "half-roof", %out%);
  
  add_exit("%6% window", PATH+"%N%bed2", "window");
  add_exit("%2% window", PATH+"%N%bed3", "window");
  
  set_wall( ({ "bottom", ({ %out", 250 }), }) );
  set_wall( ({ "move", ({ "down", 30, %out%,
                            "$N climb$s down from above to join you." }) }) );
}
