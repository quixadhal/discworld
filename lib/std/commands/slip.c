/* allows someone to quitely pick something up.  Is based on the objects
 * bulk.  It is hard to palmn something big ;)
 */
#define SKILL "other.covert.sleight-of-hand"
#define COST 3
#define LEARN_LEVEL 3
#define TEACH_LEVEL 15

int slip(string str) {
  object *obs,
         *obs2,
         *fail,
         *t_p,
         *g_f,
         *from;
  int i, j, ret, skill, g_p;
  string s1;

  if (!str) {
    notify_fail("Syntax: slip <objects> to <objects>\n");
    return 0;
  }
  
  if (sscanf(str, "%s to %s", str, s1) != 2) {
    notify_fail("Syntax: slip <objects> to <objects>\n");
    return 0;
  }
  obs = find_match(s1, environment(this_player()));
  if (!sizeof(obs)) {
    notify_fail("Slip to who?\n");
    return 0;
  }

  skill = (int)this_player()->query_skill_bonus(SKILL);
  g_p = (int)this_player()->query_gp();

  for (i=0;i<sizeof(obs);i++) {
    t_p = fail = g_f = ({ });
    obs2 = find_match(str, this_player());
    for (j=0;j<sizeof(obs2);j++)
      if (obs2[j]->move(obs[i]))
        fail += ({ obs2[j] });
      else if (COST > g_p)
        g_f += ({ obs2[j] });
      else {
        if (skill < random(200)*(1+obs2[j]->query_weight()))
          t_p += ({ obs2[j] });
        this_player()->adjust_gp(-COST);
        g_p -= COST;
      }
    if (sizeof(fail))
      write("You cannot slip "+query_multiple_short(fail)+
            " to "+obs[i]->query_cap_name()+".\n");
    if (sizeof(g_f))
      write("Not enough guild points to slip "+query_multiple_short(g_f)+
            " to "+obs[i]->query_cap_name()+".\n");
    fail += g_f;
    if (sizeof(t_p))
      tell_room(environment(this_player()),
          this_player()->query_cap_name()+" tries to slip "+
          query_multiple_short(t_p)+" to "+obs[i]->query_cap_name()+".\n",
          ({ this_player(), obs[i] }));
    if (sizeof(fail) != sizeof(obs2)) {
      write("You slip "+query_multiple_short(obs2-fail)+
            " to "+obs[i]->query_cap_name()+".\n");
      ret += sizeof(obs2 - fail);
      tell_object(obs[i], this_player()->query_cap_name()+
                  " slips "+query_multiple_short(obs2-fail)+" to you.\n");
    }
  }

  notify_fail("Oh no.\n");
  if (ret)
    this_player()->adjust_gp(-ret*COST);
  return ret;
}

string help() {
  return 
"Syntax: slip <objects> to <objects>\n\n"+
"This command allows you to slip things to players or containers "+
"without generating a give message.  The use of this command will "+
"cost you "+COST+" guild points for every object you slip to someone else."+
"  It uses the skill "+SKILL+" for "+
"determineing the skill bonus.\n\n"+
"Example:\n"+
"> slip frog to khaos\n"+
"You slip a frog to Khaos.\n\n"+
"See also:\n"+
"  palm\n";
}

int teach(object ob) {
  if (this_player()->query_skill(SKILL) < TEACH_LEVEL) {
    return -1;
  }
  if (ob->query_skill(SKILL) < LEARN_LEVEL) {
    return -2;
  }
  ob->add_known_command("slip");
  return 1;
}
