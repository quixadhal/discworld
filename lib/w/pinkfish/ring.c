inherit "/std/object";

void setup() {
  set_name("ring");
  set_short("engagement ring");
  set_long("It is a beatiful diamond engagement ring with the "+
           "diamond shaped like a small fish.  Engraved on the under "+
           "side of the ring are the words, \"To Khaos with love "+
           "Pinkfish\".\n");
  add_adjective("engagement");
}

int query_auto_load() { return 1; }
