inherit "/std/object";

object worn_by;

void setup() {
  set_name("dress");
  add_adjective( ({ "long", "flowing", "beautiful", "wedding" }) );
  set_short("wedding dress");
  set_long("A beautiful long flowing white wedding dress.\n");
}


string query_type() {
  return "dress";
}

int set_worn_by(object ob) {
  if (!ob) {
    worn_by->remove_extra_look(this_object());
    worn_by = 0;
    return 1;
  }
  worn_by = this_player();
  worn_by->add_extra_look(this_object());
  return 1;
}

string query_plural() {
  if (worn_by)
    return ::query_plural()+" (worn)";
  return ::query_plural();
}

int query_armour() { return 1; }

string pretty_plural() {
  if (worn_by)
    return ::pretty_plural()+" (worn)";
  return ::pretty_plural();
}

string pretty_short(int dark) {
  if (worn_by)
    return ::short(dark)+" (worn)";
  return ::short(dark);
}

string short(int dark) {
  if (worn_by)
    return ::short(dark)+" (worn)";
  return ::short(dark);
}

string extra_look() {
  return "Is wearing a beautiful long flowing wedding dress.\n";
}

move(object dest, string mess_in, string mess_out) {
  int i;


  i = ::move(dest, mess_in, mess_out);
  if (!i && worn_by && dest != worn_by) {
    worn_by->remove_extra_look(this_object());
    worn_by = 0;
  }
  return i;
}
