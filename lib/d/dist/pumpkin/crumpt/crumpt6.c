#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Crumpet Circle");
  set_long("This is the end of Crumpet Circle\n");
  set_light( 80 );
  add_exit("northeast", CRUMPT + "crumpt4", "road");
  add_exit("south", CRUMPT + "crumpt8", "road");
  add_exit("north", PATH + "club_office", "door");

  set_monster( 1, "city" );
}
