/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: snoope_rs.c,v 1.1 1998/01/06 05:25:41 ceres Exp $
 * $Log: snoope_rs.c,v $
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
/* snoopers command, trial out by Turrican for a commands daemon. */

#include <creator.h>

mixed cmd(string str) {
  object *obs;
  int i;

  obs = (object *)PEOPLER->get_people(str);
  obs = filter_array(obs, "check_snoop", this_object());
  if (!sizeof(obs)) {
    notify_fail("No one is being snooped by anyone.\n");
    return 0;
  }
  for (i=0;i<sizeof(obs);i++)
    write(obs[i]->query_cap_name()+" is snooping "+
          obs[i]->query_snoopee()->query_cap_name()+".\n");
  return 1;
}

int check_snoop(object ob) {
  if((object)ob->query_snoopee())
    return 1;
  else
    return 0;
}

void dest_me() {
  destruct(this_object());
}

void clean_up() {
  dest_me();
}

void reset() {
  dest_me();
}
