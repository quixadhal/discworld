inherit "/std/object";

object our_pub;

void create() {
  ::create();
  set_name("menu");
  reset_get();
}

void set_pub(object pub) { our_pub = pub; }
object query_pub() { return our_pub; }

void init() {
  this_player()->add_command("read", this_object());
  this_player()->add_command("list", this_object());
}

int do_read() {
  write(our_pub->read());
  return 1;
}

int do_list() {
  return do_read();
}

string long(string str, int dark) {
  return (string)our_pub->read();
}
