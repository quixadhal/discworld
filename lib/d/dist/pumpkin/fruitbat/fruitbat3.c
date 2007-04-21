#include "path.h";

inherit CITYROOM;

void setup() {
  set_short("Fruitbat Avenue");
  set_long("This is the middle of Fruitbat Avenue just south of the "
           "vault.\n");
  set_light( 80 );
  add_exit("east", FRUITBAT + "fruitbat4", "road");
  add_exit("west", FRUITBAT + "fruitbat2", "road");
  add_exit("north", FRUITBAT + "walrus_vault_entry", "door");

  set_monster(1, "city");
}
