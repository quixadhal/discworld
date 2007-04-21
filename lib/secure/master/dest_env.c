/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: dest_env.c,v 1.2 2003/02/12 16:26:52 wodan Exp $
 * $Log: dest_env.c,v $
 * Revision 1.2  2003/02/12 16:26:52  wodan
 * removed <TAB>
 *
 * Revision 1.1  1998/01/06 05:12:03  ceres
 * Initial revision
 * 
*/
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
