inherit "/std/races/standard";

void setup() {
  set_long("Creators and lords are the curious creatures that assist the " +
           "gods in the creation of the world.\n");
   set_name("creator");
}

string query_desc(object ob) {
  return "An example of Discworld's finest.\n";
}
