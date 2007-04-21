/*  -*- LPC -*-  */
/*
 * $Locker: pinkfish $
 * $Id: unh_old.c,v 1.2 1998/10/08 09:52:49 pinkfish Exp pinkfish $
 * $Log: unh_old.c,v $
 * Revision 1.2  1998/10/08 09:52:49  pinkfish
 * Fixed up to work correctly with things that do not wish to be unheld.
 *
 * Revision 1.1  1998/01/06 05:28:43  ceres
 * Initial revision
 * 
*/
inherit "/cmds/base";

#define TP this_player()

int cmd(object obs) {
  object *ok, *zip, *failed, ob, *using;
  string s;
  mixed ret;

  using = TP->query_holding();
  failed = ok = zip = ({ });

  foreach (ob in obs) {
    if (member_array(ob, using) != -1) {
      if (!sizeof(ret = TP->set_unhold(ob)) || !ret)
        failed += ({ ob });
      else
        ok += ({ ob });
    } 
    else
      zip += ({ ob });
  }
  if (!sizeof(ok)) {
    s = "";
    if (sizeof(failed))
      s += "Could not unhold "+query_multiple_short(failed)+".\n";
    if (sizeof(zip))
      s += capitalize("You are not holding " +
                      query_multiple_short(zip, "the") + ".\n");
    return notify_fail(s);
  }
  tell_object(TP, "You put down "+(s = query_multiple_short(ok))+".\n");
  tell_room(environment(TP), TP->one_short()+" puts down "+s+".\n", TP);
  TP->force_burden_recalculate();
  return 1;
} /* unhold() */

mixed *query_patterns() {
  return ({ "<indirect:object:me>", (: cmd($1) :) });
}
