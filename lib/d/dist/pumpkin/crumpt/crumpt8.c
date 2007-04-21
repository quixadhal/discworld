#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Crumpet Circle");
  set_long("This is Crumpet Circle at the junction with Fruitbat Avenue.
\n");
  set_light( 80 );
  add_exit("east", CRUMPT + "crumpt9", "road");
  add_exit("north", CRUMPT + "crumpt6", "road");
  add_exit("south", FRUITBAT + "fruitbat1", "road");

  set_monster( 1, "city" );

}
