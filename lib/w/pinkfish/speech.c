inherit "/std/object";

void setup() {
  set_short("speech muncher");
  set_long("A small black box, it emits strange whistling noises.\n");
  set_name("muncher");
  add_adjective("speech");
  add_alias("box");
  add_adjective(({ "small", "black" }));
}

void init() {
  ::init();
  add_action("speech_mangle", "man");
}

/* ok, speech mangling... Hmm...
 * Rules.
 *   s -> sh
 *   r -> w
 */
int speech_mangle(string str) {
  if (!str) {
    notify_fail("Syntax: "+query_verb()+" <text>\n");
    return 0;
  }
  str = "&"+str+"&";
  str = implode(explode(str, "s"), "sh");
  str = implode(explode(str, "r"), "rr");
  str = implode(explode(str, "ing "), "in' ");
  return (int)this_player()->do_say(str[1..strlen(str)-2]);
}
