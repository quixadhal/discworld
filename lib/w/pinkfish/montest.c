inherit "/obj/monster";

void setup() {
  set_short("big test monster");
  set_long("A nice big test monster with bulging muscles and other things "+
           "you hope it goes away.\n");
  set_name("monster");
  add_adjective(({ "nice", "big", "test" }) );
  add_triggered_action("bing", "say", this_object(), "test_bing");
}

void test_bing(object ob, string str) {
  write_file("/w/pinkfish/mon-log", str);
}
