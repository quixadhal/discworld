inherit "/std/basic/label";

static string short_d,
       plural_d,
       long_d;

void set_short(string str) { short_d = str; }
void set_long(string str) { long_d = str; }
void set_main_plural(string str) { plural_d = str; }

string query_short() { return short_d; }
string query_long() { return long_d; }
string query_main_plural() { return plural_d; }

string short(int dark) {
  return short_d; 
}

string pretty_short(int dark) {
  return (string)this_object()->short();
}

string long(string str, int dark) {
  string s;

  if (sizeof(labels))
    s = "There is "+query_multiple_short(labels)+" stuck on "+short_d+".\n";
  else
    s = "";
  if (!long_d)
    return "You see nothing particularly interesting.\n" + s;
  return long_d + s;
}

string query_plural() {
  if (!plural_d)
    if (!short(0))
      return 0;
    else
      return pluralize(short(0));
  return plural_d;
}

mixed pretty_plural() {
  return query_plural();
}

/* std/basic/desc: */
mixed *query_init_data() {
   return ::query_init_data() +
      ({ "short", short_d, "set_short/p/",
         "long", long_d, "set_long/p/",
         "main_plural", plural_d, "set_main_plural/p/" });
} /* query_init_data() */
