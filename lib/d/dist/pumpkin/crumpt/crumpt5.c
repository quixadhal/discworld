#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Crumpet Circle");
  set_long("This is the end of Crumpet Circle\n");
  set_light( 80 );
  add_exit("southwest", CRUMPT + "crumpt7", "road");
  add_exit("north", CRUMPT + "crumpt3", "road");

  set_monster( 1, "city" );
}
