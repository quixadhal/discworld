inherit "/std/object";

void setup() {
  set_name("snooper");
  set_short("snooper");
  set_long("Type \"snoop\" <name> to snoop someone. dont add a name "+
           "to stop snooping.\n");
  reset_drop();
}

void init() {
  ::init();
  add_action("do_snoop", "snoop");
  add_action("do_snoop", "qsnoop");
}

int do_snoop(string str) {
  if (!str) {
    snoop(this_player());
    write("Ok, snoop cancelled.\n");
    return 1;
  }
  str = (string)this_player()->expand_nickname(str);
  if (!find_living(str)) {
    write("Cannot find "+str+"\n");
    return 1;
  }
  snoop(this_player(), find_living(str));
  write("Ok, snooping "+str+"\n");
  return 1;
}
