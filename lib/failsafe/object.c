inherit "/failsafe/property";
inherit "/failsafe/read_desc";
inherit "/failsafe/misc";
inherit "/failsafe/id";

int sec_level;

void set_name(string str) {
  if (name) return ;
  name = str;
  short_d = capitalize(str);
}

void create() {
  id::create();
  property::create();
  misc::create();
  seteuid((string)"/secure/master"->creator_file(this_object()));
  this_object()->setup();
}

void set_sec_level(int i) { sec_level = i; }
int query_sec_level() { return sec_level; }
