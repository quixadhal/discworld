/*
 * has all the give/get stuff.
 */
#include "move_failures.h"
#define TIME_DIV 5

void handle_commands() {
  add_action("take", "get");
  add_action("take", "take");
  add_action("give", "give");
  add_action("give", "put");
  add_action("drop_ob", "drop");
}

/* this will work as give and put...
 */
int give(string str, string verb, object *bing, string bing2, int blue) {
  string person,
         thing,
         sh, con, s1;
  int i, j, tot, max;
  object *obs,
         *per,
         *ret,
         *fail;
  if (!str && !verb) {
    notify_fail("Usage: "+query_verb()+" <object> <to/in> <object>\n");
    return 0;
  }
  if ((verb?verb:query_verb()) == "give")
    con = " to ";
  else
    con = " in ";
  if (bing) {
    obs = bing;
    thing = bing2;
    j = blue;
  } else {
    if (sscanf(str, "%s to %s", thing, person) != 2)
      if (sscanf(str, "%s in %s", thing, person) != 2) {
        notify_fail("Usage: "+query_verb()+" <object> <to/in> <object>\n");
        return 0;
      }
    obs = find_match(person, ({ this_object(), environment() }));
    if (!sizeof(obs)) {
      notify_fail("Cannot find "+person+" to "+query_verb()+" anything "+con+".\n");
      return 0;
    }
    per = obs;
  }
/* should check to see that we can give things to this person/thing... */
  if (!per) per = ({ });
  for (;j<sizeof(per);j++) {
    if (per[j] == this_object())
      continue;
    obs = find_match(thing, this_object());
    if (!sizeof(obs)) {
      write("Nothing to "+query_verb()+con+per[j]->short()+".\n");
      continue;
    }
    ret = ({ });
    fail = ({ });
    for (i=0;i<sizeof(obs);i++)
      if (((int)obs[i]->move(per[j])) == MOVE_OK) {
        ret += ({ obs[i] });
        tot += (int)obs[i]->query_weight();
      } else
        fail += ({ obs[i] });
    if (sizeof(ret)) {
      sh = query_multiple_short(ret);
      write("You "+query_verb()+" "+sh+con+
            (string)per[j]->short()+".\n");
      say((string)this_object()->query_cap_name()+" "+query_verb()+"s "+
           sh+con+(string)per[j]->short()+".\n", ({ per[j] }) );
      per[j]->event_say(this_object(), (string)this_object()->query_cap_name()+
          " "+query_verb()+"s "+sh+con+"you.\n", ({ }));
      if (living(per[j]) && (max = (int)per[j]->query_max_weight()))
        if ((max = tot*100/max) > 25)
          if (max >= 95) {
            tell_room(environment(),
                      per[j]->short()+" staggers under a weight "+
                      per[j]->query_pronoun()+" can only just carry.\n", ({ per[j] }) );
            per[j]->event_say(this_object(), 
                     "You stagger under a weight you can only just carry.\n");
          } else {
            tell_room(environment(),
                      per[j]->short()+ ({
              " is only mildly discomforted by the additional weight.\n",
              " braces "+per[j]->query_objective()+"self to take the load.\n",
              " stumbles as "+per[j]->query_pronoun()+" takes the load.\n"
                 })[(max/25)-1], ({ per[j] }) );
            per[j]->event_say(this_object(), "You"+ ({
              " are only mildly discomforted by the additional weight.\n",
              " brace yourself under the load.\n",
              " stumble as you take the load.\n"
                 })[(max/25)-1]);
          }
      this_player()->adjust_time_left(-tot/TIME_DIV);
      if (this_player()->query_time_left() < 0)
        this_player()->set_interupt_command("get_put_interupt", this_object(),
                                    ({ tot, ret, query_verb(), con, per, j, thing }));
    }
    if (sizeof(fail))
      write("You cannot "+query_verb()+" "+query_multiple_short(fail)+
             con+per[j]->short()+".\n");
  }
  return 1;
}

int drop_ob(string str) {
  int i, num;
  object *ob, *ret, *fail;
  string sh;

  if (!str) {
    notify_fail("Usage: "+query_verb()+" <object(s)>\n");
    return 0;
  }
  ob = find_match(str, this_object());
  if (!sizeof(ob)) {
    notify_fail("Cannot find "+str+" to drop.\n");
    return 0;
  }
  ret = ({ });
  fail = ({ });
  for (i=0;i<sizeof(ob);i++) {
    if (!ob[i]->short())
      continue;
    if (ob[i]->move(environment()) == MOVE_OK)
      ret += ({ ob[i] });
    else
      fail += ({ ob[i] });
  }
  if (sizeof(ret)) {
    sh = query_multiple_short(ret);
    if (!this_object()->query_hidden())
      say(this_object()->query_cap_name()+" drops "+sh+".\n");
    write("You drop "+sh+".\n");
  }
  if (sizeof(fail))
    write("You cannot drop "+query_multiple_short(fail)+".\n");
  num += sizeof(fail)+sizeof(ret);
  if (!num) {
    notify_fail("Cannot drop "+str+".\n");
    return 0;
  }
  return 1;
}

int take(string str, string verb, object *bing, string bing2, int blue) {
  object *dest, *obs, *fail, *ret_a;
  mixed *ret;
  string s2, sh;
  int i, num, j, cap, perc, we, tot;

  if (!str && !verb) {
    notify_fail("Syntax: "+query_verb()+" <object> [from <object>]\n");
    return 0;
  }
  cap = (int)this_object()->query_max_weight();
  if (verb) {
    dest = bing;
    i = blue;
    str = bing2;
  } else {
    dest = ({ environment() });
    if (sscanf(str, "%s from %s", str, s2) == 2) {
  /* since we only allow one lvl anyway... (*/
      dest = find_match(s2, ({ environment(), this_player() }));
      if (!sizeof(dest)) {
        notify_fail("Cannot find any "+s2+" here.\n");
        return 0;
      }
    }
    i = 0;
  }
  for (;i<sizeof(dest);i++) {
    if (dest[i]->cannot_get_stuff())
      continue;
    obs = find_match(str, dest[i]);
    ret = ({ ({ }), ({ }), ({ }), ({ }), ({ }), });
    ret_a = ({ });
    fail = ({ });
    tot = 0;
    for (j=0;j<sizeof(obs);j++)
      if (obs[j]->move(this_object()) == MOVE_OK) {
        if ((perc = (we = (int)obs[j]->query_weight())*100/cap) >= 95)
          ret[4] += ({ obs[j] });
        else
          ret[perc/25] += ({ obs[j] });
        tot += we;
        ret_a += ({ obs[j] });
      } else if (!living(obs[j]))
        fail += ({ obs[j] });
    for (j=0;j<sizeof(ret);j++)
      if (sizeof(ret[j])) {
        sh = query_multiple_short(ret[j]);
        s2 = query_multiple_short(dest[i..i]);
        write("You "+({ "get", "get with a bit of difficulty",
                            "struggle somewhat to get",
                            "find it very difficult to get",
                   "use all your strength and just barely manage to get" })[j]+
            " "+sh+" from "+s2+".\n");
/*
            ((s2=dest[i]->query_property("determinate"))?s2:"the ")+
            (string)dest[i]->short()+".\n");
 */
         if (!this_object()->query_hidden())
          say((string)this_object()->query_cap_name()+" "+
                     ({ "gets", "gets with a bit of difficulty",
                            "struggles somewhat to get",
                            "finds it very difficult to get",
                   "uses all "+this_player()->query_pronoun()+
                   " strength and just barely manages to get" })[j]+
            " "+sh+" from "+s2+".\n");
/*
dest[i]->query_property("determinate")+
            (string)dest[i]->short()+".\n");
 */
        num += sizeof(ret[j]);
      }
    if (tot) {
      this_player()->adjust_time_left(-tot/TIME_DIV);
      if (this_player()->query_time_left() < 0) {
        this_player()->set_interupt_command("get_put_interupt", this_object(),
                                    ({ tot, ret_a, "get", "from", dest, i, str }));
      }
    }
    if (sizeof(fail))
      write("You cannot get "+query_multiple_short(fail)+".\n");
    num += sizeof(fail);
  }
  if (!num) {
    notify_fail("Nothing to "+query_verb()+".\n");
    return 0;
  }
  return 1;
}

void get_put_interupt(int left, mixed arg) {
  int tot, j, i, weight;
  object *obs, *per;
  string sh, thing, sh2;

  per = arg[4];
  j = arg[5];
  thing = arg[6];
  obs = arg[1];
  if (left > 0) {
    object dest;
/* We have been stopped!  How rude :) */
    tot = arg[0] - left; /* This is how much they pick up */
    if (arg[2] != "get")
      dest = this_player();
    else
      dest = per[j];

    for (i=0;i<sizeof(obs);i++) {
      weight = (int)obs[i]->query_weight();
      if (tot - weight < 0) {
        if (!i)
          sh = "none of the above";
        else
          sh = query_multiple_short(obs[0..i-1]);
        sh2 = query_multiple_short(obs);
        for (;i<sizeof(obs);i++)
          obs[i]->move(dest);
        write("Your interupted "+arg[2]+"ing "+sh2+" "+arg[3]+" "+
              per[j]->short()+", only "+arg[2]+"ing "+sh+".\n");
        say(this_player()->query_cap_name()+" is interupted "+arg[2]+"ing "+
            sh2+" "+arg[3]+" "+per[j]->short()+", only "+arg[2]+"ing "+sh+".\n");
        return ;
      }
      tot -= weight;
    }
    write("Hmmm...  here,,,?\n");
    return ;
  }
  if (j < sizeof(per)) {
/* Keep going... */
    switch (arg[2]) {
      case "give" :
      case "put" :
        give(0, arg[2], per, thing, j+1);
        break;
     case "get" :
        take(0, arg[2], per, thing, j+1);
        break;
    }
  }
}
