static string short_d,
       long_d;

void set_short(string str) { short_d = str; }
void set_long(string str) { long_d = str; }

string query_short() { return short_d; }
string query_long() { return long_d; }

string short() { 
  return short_d; 
}

string pretty_short() {
  return short_d;
}

string long(string str) {
  if (!long_d)
    return "You see nothing particularly interesting.\n";
  return long_d;
}
