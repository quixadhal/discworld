/*
 * Called if the environment is dested and there is no where else
 * to go...
 */
void destruct_environment_of(object ob) {
  object env;

  env = environment(ob);
  if (env) {
      string *a;
      a = (string *)env->query_dest_dir();
      if (catch(ob->move_player(a[0], a[1], "stumbles"))) {
	  ob->move_player("void", "/room/void", "is sucked into the");
      }
  }
} /* destruct_environment_of() */
