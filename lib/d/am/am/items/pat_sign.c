inherit "/std/object";

void setup() {
  set_read_mess("dummy");
  reset_get();
  set_name("sign");
}

string long(string str2, int dark) {
  int plevel;
  string str;

  str = "";
  if(this_player()->query_creator()) {
    str = "As a creator you can :-\n"+
          "  banish <name> <reason>\n";
  }
  if(this_player()->query_property("guest")) {
    str += "As a guest you can :-\n"+
          "  start     - remove guest property.\n";
  }
  if(!this_player()->query_property("guest")) {
    str += "As a player you can :-\n"+
          "  vote\n"+
          "  apply     - apply to a house to become a creator.\n"+
          "  complain\n";
  }
  return str;
}

int do_read() {
  write(long("sign",0));
  return 1;
}
