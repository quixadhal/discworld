/* allows someone to quitely pick something up.  Is based on the objects
 * bulk.  It is hard to palmn something big ;)
 */
#define SKILL "other.covert.sleight-of-hand"
#define TEACH_LEVEL 20
#define LEARN_LEVEL 5
#define COST 3

int palm(string str) {
  object *obs,
         *fail,
         *t_p,
         *g_f,
         *from;
  int i, skill, g_p;
  string s1;

  if (!str) {
    notify_fail("Syntax: palm <objects>\n");
    return 0;
  }
  
  from = ({ environment(this_player()) });
  if (sscanf(str, "%s from %s", str, s1) == 2) {
    from = find_match(s1, from);
    from = filter_array(from, "check_allowed", this_object(), "check_allowed");
    if (!sizeof(from)) {
      notify_fail("Cannot find "+s1+"\n");
      return 0;
    }
  }
  obs = find_match(str, from);
  if (!sizeof(obs)) {
    notify_fail("Cannot find "+str+"\n");
    return 0;
  }

  skill = (int)this_player()->query_skill_bonus(SKILL);
  t_p = fail = g_f = ({ });
  g_p = (int)this_player()->query_gp();

  for (i=0;i<sizeof(obs);i++)
    if (obs[i]->move(this_player()))
      fail += ({ obs[i] });
    else if (COST > g_p)
      g_f += ({ obs[i] });
    else {
      if (skill < random(200)*(1+obs[i]->query_weight()))
        t_p += ({ obs[i] });
      this_player()->adjust_gp(-COST);
      g_p -= COST;
    }

  if (sizeof(fail) == sizeof(obs)) {
    if (sizeof(g_f))
      notify_fail("You do not have enough guild points to palm "+
                  query_multiple_short(g_f)+".\n"+
                  "You cannot palm "+query_multiple_short(fail)+".\n");
    else
      notify_fail("You cannot palm "+query_multiple_short(fail)+".\n");
    return 0;
  }
  if (sizeof(fail))
    write("You cannot palm "+query_multiple_short(fail)+".\n");
  if (sizeof(g_f))
    write("You do not have enough guild points to palm "+
          query_multiple_short(g_f)+".\n");
  fail += g_f;
  write("You palm "+query_multiple_short(obs - fail)+".\n");
  if (sizeof(t_p)) {
    say(this_player()->query_cap_name()+" trys to palm "+
        query_multiple_short(t_p)+".\n");
  }
  return 1;
}

string help() {
  return 
"Syntax: palm <object> [from <objects>]\n\n"+
"Will silently pick up an object either from the ground or "+
"from another object.  This uses the skill "+SKILL+" to "+
"determine how good you are at it.\n\n"+
"Example:\n"+
"> palm coin\n"+
"You palm the coin.\n"+
"\nSee also:\n  slip\n";
}

int check_allowed(object ob) {
  return !ob->cannot_get_stuff();
}

int teach(object ob) {
  if (this_player()->query_skill(SKILL) < TEACH_LEVEL) {
    return -1;
  }
  if (ob->query_skill(SKILL) < LEARN_LEVEL) {
    return -2;
  }
  ob->add_known_command("palm");
  return 1;
}
