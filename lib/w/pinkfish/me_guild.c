inherit "/std/guild";

void setup() {
  set_short("Fighter guild");
  set_long("The fighters guild.  Type \"advance\" to advance your levels "+
          "and \"join\" to join up.\n");
  add_exit("east", "/w/pinkfish/workroom", "door");
  add_exit("drum", "/d/am/am/mendeddrum", "door");
  add_exit("north", "/w/pinkfish/shop", "door");
  set_light(100);
  set_guild("/std/guilds/fighter");
}

void enter(object ob) {
  object weath;

}
