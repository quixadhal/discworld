#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Crumpet Circle");
  set_long("This is the end of Crumpet Circle\n");
  set_light( 80 );
  add_exit("northeast", CRUMPT + "crumpt1", "road");
  add_exit("southwest", CRUMPT + "crumpt4", "road");
  add_exit("northwest", CRUMPT + "post_office", "door");

  set_monster( 1, "city" );
}
