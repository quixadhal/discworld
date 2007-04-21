/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: finger.c,v 1.12 2001/04/05 03:57:58 ceres Exp $
 */
/* finger command, trial out by Turrican for a commands daemon. */
inherit "/cmds/base";

#include <clubs.h>

mixed cmd() {
  string ret;
  object ob, *obs;
  string type;
  string str;

    obs = users();
    obs = sort_array(obs, (: strcmp($1->query_cap_name(), 
    $2->query_cap_name()) :));
    str = sprintf("%-12.12s    %-20.20s %-20.20s %-20.20s\n",
                  "Name", "Real name", "Where", "Birthday");
    foreach (ob in obs) {
      string euid;
      type = (ob->query_earmuffs() ? "e" : " ");
      euid = geteuid(ob);
      type += ob->query_object_type();
      /*
       * above changed to use query_object_type by ember 16-feb-93 ... does
       * query_object_type get the 'X' as well?
       */
      str += sprintf("%-12.12s %2.2s %-20.20s %-20.20s %-20.20s\n",
      (ob->query_invis()?"("+ob->query_cap_name()+")":(string)ob->query_cap_name()),
                    type,
                    ((ret = (string)ob->query_real_name())?ret:"-"),
                    ((ret = (string)ob->query_where())?ret:"-"),
                    ((ret = (string)ob->query_birthday())?ret:"-"));
    }
    write("$P$Finger$P$" + str);
    return 1;
} /* cmd() */

int finger_player(string str) {
  string ret;
  string mud;

  if (str) {
    str = lower_case(str);
    str = (string)this_player()->expand_nickname(str);
  }
  ret = (string)"/secure/finger"->finger_info(str);
  if (ret) {
    write("$P$finger: " + str + "$P$" + ret);
    return 1;
  } else if (this_player()->query_creator() &&
             sscanf(str, "%s@%s", mud, mud) == 2) {
    "/net/daemon/out_finger"->do_finger(str);
/* Commented out by Turrican, since the daemon gives a message already.
    write("Intermud finger on its way.\n");
*/
    return 1;
  } else {
    add_failed_mess("No one going by the name of " + str + " has ever visited " 
      "" + mud_name() + ".\n");
    return 0;
  }
}

mixed *query_patterns() {
   return ({ "", (: cmd() :),
             "<string'name|domain|club'>", (: finger_player($4[0]) :) });
} /* query_patterns() */
