/*
 * This will create free fireworks...  Not a hot idea to use
 * in general....
 */
inherit "/std/object";

void setup() {
  set_name("firework");
  set_long(
"Its a small long shaped thingy.  The only reason you can tell its "+
"a firework is the large letters on the side in blue that say 'firework' "+
"gives it away a bit really.  You might even be able to use it.\n");
  set_read_mess("%^BLUE%^F I R E W O R K S%^RESET%^");
  adjust_money(10, "platinum");
} /* setup() */

void init() {
  ::init();
  this_player()->add_command("use", this_object());
} /* init() */

int do_use() {
  "/std/spells/wizard/chars/firework"->random_firework();
  call_out("dest_me", 2);
  return 1;
} /* do_use() */
