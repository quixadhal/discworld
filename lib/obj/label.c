inherit "/std/object";

void create() {
  ::create();
  add_property("writeable", 1);
  add_property("label", 1);
} /* create() */

void init() {
  ::init();
  this_player()->add_command("stick", this_object(), "%D %p %I");
} /* init() */

int do_stick(object *obs) {
  obs = obs - ({ this_object() });
  if (!sizeof(obs)) return 0;
  obs[0]->add_label(this_object());
  if (environment() != obs[0] && move(obs[0])) {
    write("Move failed.\n");
    obs[0]->remove_label(this_object());
    return 0;
  }
  this_player()->add_succeeded(obs[0]);
  return 1;
} /* do_stick() */

move(object ob, string s1, string s2) {
  object env;
  int i;

  env = environment();
  i = ::move(ob, s1, s2);
  if (!i && env)
    env->remove_label(this_object());
  return i;
} /* move() */

mixed query_static_auto_load() {
  if (file_name(this_object())[0..9] == "/obj/label")
    return int_query_static_auto_load();
  return ([ ]);
} /* query_static_auto_load() */

void dest_me() {
  if (environment())
    environment()->remove_label(this_object());
  ::dest_me();
} /* dest_me() */
