inherit "std/object";

void setup() {
  set_name("paper");
  set_short("Piece of paper");
  set_main_plural("Pieces of paper");
  add_plural("papers");
  set_long("A piece of paper\n");
  add_property("writeable", 1);
  set_max_size(1000);
}

void init() {
  ::init();
  this_player()->add_command("eat", this_object());
} /* init() */

int do_eat() {
  call_out("dest_me", 0);
  return 1;
} /* do_eat() */
