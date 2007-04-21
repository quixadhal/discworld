/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: eat_edible.c,v 1.2 1998/02/20 05:55:47 pinkfish Exp $
 * $Log: eat_edible.c,v $
 * Revision 1.2  1998/02/20 05:55:47  pinkfish
 * Fixed a runtime error and made it more ribust by using
 * function points for the function calls.
 *
 * Revision 1.1  1998/01/06 04:39:52  ceres
 * Initial revision
 * 
*/
/*
 * The eat edible objects shadow.
 */
inherit "/std/effect_shadow";

protected void real_eat(object ob);
protected void do_eat_edible();

mixed move(mixed dest, string messin, string messout) {
  int ret;

  ret = player->move(dest, messin, messout);
  if (ret == 0) {
    call_out((: do_eat_edible :), 0);
  }
  return ret;
} /* move() */

protected void do_eat_edible() {
  int i;
  object *obs;

  obs = all_inventory(player);
  for (i=0;i<sizeof(obs);i++) {
    if (obs[i]->query_edible()) {
      call_out((: real_eat :), 1, obs[i]);
    }
  }
} /* do_eat_edible() */

protected void real_eat(object ob) {
  if (ob && player) {
    player->do_command("eat "+ob->query_name());
  }
} /* real_eat() */

void event_enter(object ob, string mess, object from) {
  player->event_enter(ob, mess, from);
  if (ob->query_edible()) {
    call_out("real_eat", 2, ob);
  }
} /* event_enter() */
