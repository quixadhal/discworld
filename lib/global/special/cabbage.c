inherit "/std/object";

void setup() {
  set_name("cabbage");
  add_adjective(({ "nice", "large", "happy", "looking" }));
  set_long(
"A nice large looking cabbage.  It looks very cabbagey.  You can see a "+
"small made in ankh stamp stuck on the stalky bit.\n");
  set_read_mess("Happy cabbage day!\n");
  reset_drop();
} /* setup() */

void init() {
  this_player()->add_command("eat", this_object());
  ::init();
} /* init() */

void init_arg(mixed arg) {
} /* init_arg() */

mixed query_static_auto_load() { return 0; }
mixed query_dynamic_auto_load() { return 0; }

int do_eat() {
  write("I feeeeel good, like a knew that I should.\n");
  call_out("dest_me", 0);
  return 1;
} /* do_eat() */
