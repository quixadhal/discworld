inherit "std/object";

string type;

void set_type(string str) {
  remove_adjective(type);
  type = str;
  add_adjective(type);
  set_main_plural(type + " keys");
  set_short(type + " key");
}
string query_type() {
  return type;
}

void create() {
  ::create();
  set_name("key");
  set_main_plural("normal keys");
  add_plural("keys");
  type = "normal";
  set_short(query_type() + " key");
  set_long("A key.  Wonder where it fits?.\n");
  set_weight(1);
  adjust_money(1, "brass");
}
