/* has all the give/get stuff.
 */
#include "move_failures.h"
inherit "failsafe/simul_efun";

/* this will work as give and put...
 */
int give(string str) {
  string person,
         thing;
  int       i;
  object *obs,
         per;
  if (!str) {
    notify_fail("Usage: "+query_verb()+" <object> <to/in> <person>\n");
    return 0;
  }
  if (sscanf(str, "%s to %s", thing, person) != 2) 
    if (sscanf(str, "%s in %s", thing, person) != 2) {
      notify_fail("Usage: "+query_verb()+" <object> <to/in> <object>\n");
      return 0;
    }
  if (!(obs = find_match(person, environment())) || !sizeof(obs)) {
    notify_fail("Cannot find "+person+" to give anything to.\n");
    return 0;
  }
  if (sizeof(obs) != 1) {
    notify_fail("To many things to give to.\n");
    return 0;
  }
  per = obs[0];
/* should check to see that we can give things to this person/thing... */
  obs = find_match(thing, this_object());
  if (!sizeof(obs)) {
    notify_fail("Nothing to give!\n");
    return 0;
  }
  for (i=0;i<sizeof(obs);i++)
    if (((int)obs[i]->move(per)) == MOVE_OK) {
      write("You "+query_verb()+" "+(string)obs[i]->short()+
            (query_verb() == "give" ? " to " : " in ")+
            (string)per->query_name()+".\n");
      say((string)this_object()->query_cap_name()+" "+query_verb()+" "+
          (string)obs[i]->short()+ (query_verb() == "give" ? " to " : " in ")+
          (string)per->query_name()+"\n");
    }
  return 1;
}

int drop_ob(string str) {
  int i, num;
  object *ob;

  if (!str) {
    notify_fail("Usage: "+query_verb()+" <object(s)>\n");
    return 0;
  }
  ob = find_match(str, this_object());
  if (!sizeof(ob)) {
    notify_fail("Cannot find "+str+" to drop.\n");
    return 0;
  }
  for (i=0;i<sizeof(ob);i++) 
    if (ob[i]->move(environment()) == MOVE_OK) {
      num++;
      say(this_object()->query_cap_name()+" drops "+ob[i]->short()+"\n");
      write("You drop "+ob[i]->short()+"\n");
    }
  if (!num) {
    notify_fail("Cannot drop "+str+"\n");
    return 0;
  }
  return 1;
}

int take(string str) {
  object *dest, *obs;
  string s2;
  int i, num, j;

  if (!str) {
    notify_fail("Usage: "+query_verb()+" <object> [from <object>]\n");
    return 0;
  }
  dest = ({ environment() });
  if (sscanf(str, "%s from %s", str, s2) == 2) {
/* since we only allow one lvl anyway... (*/
    dest = find_match(str, environment());
    if (!sizeof(dest)) {
      notify_fail("Cannot find any "+s2+" here.\n");
      return 0;
    }
  }
  for (i=0;i<sizeof(dest);i++) {
    obs = find_match(str, dest[i]);
    for (j=0;j<sizeof(obs);j++) {
/* but I forget all the cases... */
      if (obs[j]->move(this_object()) == MOVE_OK) {
        write("You get the "+(string)obs[j]->short()+" from the "+
                                      (string)dest[i]->short()+"\n");
        say((string)this_object()->query_name()+" get's the "+
            (string)obs[j]->short()+" from the "+(string)dest[i]->short()+"\n");
        num++;
      }
    }
  }
  if (!num) {
    notify_fail("Nothing to "+query_verb()+"\n");
    return 0;
  }
  return 1;
}
