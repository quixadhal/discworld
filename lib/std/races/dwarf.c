inherit "/std/races/standard";

void setup() {
  set_long("A nice dwarf like person.  Short stumpy and strong, classic "+
           "tolkien mold.\n");
   set_name("dwarf");
}

string query_desc(object ob) {
  return "Thickly built strong looking dwarf.\n";
}
