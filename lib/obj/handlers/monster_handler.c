int num;

void create() {
  seteuid("Root");
  restore_object("/save/monster-handler");
} /* create() */


string query_shadow_name() {
  num++;
  save_object("/save/monster-handler");
  return "/tmp/mon-shad/frog-"+num;
}
