/*
 *  Heal: - cure all but 10-40 of a characters hitpoints;
 *        - return all negative temporary stats to zero;
 *        - cure all curses;
 *        - cure all diseases.
 */
 
#define MIN_LVL 100
#define SP_COST 100

string help() {
  return
"This is the most powerful healing spell.  It heals all but a few of the "+
"players hit points and returns all tempory stats to 0.  This curing spell "+
"uses the same skills as all the other curing spells, so I wont put it in "+
"here as well.\n\n"+
"See also:\n"+
"  cure disease, remove curse, cure light wounds, restore.\n";
}
 
mixed heal(string str, int skill, int cast);
 
int cast_heal(string str) {
  int skill;
  mixed ret;
 
  if (this_player()->query_property("killed harry")) {
    write("Your god refuses to grant you the power since you killed his "+
         "high priest.\n");
   return 1;
  }
  str = (string)this_player()->expand_nickname(str);
  if (str == (string)this_player()->query_name())
    skill = (int)this_player()->query_skill("faith.spells.curing.self");
  else
    skill = (int)this_player()->query_skill("faith.spells.curing.target");
  if (skill < MIN_LVL) {
    notify_fail("You hear a faint laughter.\n");
    return 0;
  }
  if (str == (string)this_player()->query_name())
    skill = (int)this_player()->query_skill_bonus("faith.spells.curing.self");
  else
    skill = (int)this_player()->query_skill_bonus("faith.spells.curing.target");
  ret = heal(str, skill, 1);
  if (stringp(ret)) {
    notify_fail(ret);
    return 0;
    }
  return 1;
  }
 
mixed heal(string str, int skill, int cast) {
  mixed ob;
 
  if (this_player()->query_spell_effect("spell")) 
    return "You are already casting a spell.\n";
  ob = find_match(str, environment(this_player()));
  if (sizeof(ob))
    ob = ob[0];
  else
    ob = 0;
  this_player()->add_spell_effect(1, "spell", "Heal",
    this_object(), "hb_heal", ({ skill,ob,cast }));
  return 1;
  }
 
int hb_heal(object attacker, mixed *params) {
  int   hp_deficit, i, skill;
  mixed *curses;

  skill =  params[0];
  if (!params[1]) {
    tell_object(previous_object(),
      "Your spell fails to find its intended target.\n");
    return 0;
    }
  if (!living(params[1])) {
    tell_object(previous_object(),
      (string)params[1]->query_cap_name() +
      " is happy with the way it is now and doesnt want any help" +
      " from you.\n");
    return 0;
    }
  if (params[2] && (int)previous_object()->adjust_gp(-SP_COST) < 0) {
    tell_object(previous_object(),
      "You feel too drained.\n");
    return 0;
    }
  if (params[1] == previous_object()) {
    tell_object(previous_object(),
      "You cast a healing spell on yourself and feel a warm" +
      " sensation.  God that felt good!\n");
    say(
      (string)previous_object()->query_cap_name() +
      " mumbles something then acquires an idiotic grin and a" +
      " brief octarine glow.\n", params[1]);
    }
  else {
    tell_object(previous_object(),
      "You cast a healing spell on " + (string)params[1]->query_cap_name() +
      " who then acquires an idiotic grin and a brief octarine glow.\n");
    say(
      (string)previous_object()->query_cap_name() +
      " mumbles something then points at " +
      (string)params[1]->query_cap_name() +
      " who acquires an idiotic grin and a brief octarine glow.\n",
      params[1]);
    tell_object(params[1],
      (string)previous_object()->query_cap_name() +
      " mumbles something then points at you.  You feel a warm" +
      " sensation.  God that felt good!\n");
    }
 
  /* fix up hit points */
  hp_deficit = (int)params[1]->query_max_hp() - (int)params[1]->query_hp();
  if (hp_deficit >= 40) {
    params[1]->adjust_hp(hp_deficit - 40 + 10*random(4));
    tell_object(params[1], "You feel much healthier.\n");
    }
 
  /* fix up temporary stats */
/*
  if ((int)params[1]->query_tmp_int() < 0) {
    params[1]->adjust_tmp_int(-((int)params[1]->query_tmp_int()));
    tell_object(params[1], "Your intelligence returns to normal.\n");
    }
  if ((int)params[1]->query_tmp_dex() < 0) {
    params[1]->adjust_tmp_dex(-((int)params[1]->query_tmp_dex()));
    tell_object(params[1], "Your dexterity returns to normal.\n");
    }
  if ((int)params[1]->query_tmp_wis() < 0) {
    params[1]->adjust_tmp_wis(-((int)params[1]->query_tmp_wis()));
    tell_object(params[1], "Your wisdom returns to normal.\n");
    }
  if ((int)params[1]->query_tmp_str() < 0) {
    params[1]->adjust_tmp_str(-((int)params[1]->query_tmp_str()));
    tell_object(params[1], "Your strength returns to normal.\n");
    }
  if ((int)params[1]->query_tmp_con() < 0) {
    params[1]->adjust_tmp_con(-((int)params[1]->query_tmp_con()));
    tell_object(params[1], "Your constitution returns to normal.\n");
    }
  params[1]->reset_all();
*/
 
  /* cure all curses */
/*
  if (sizeof((curses = (mixed *)params[1]->query_curses())))
    for (i=0; i<sizeof(curses); i+=2)
      if (params[1]->query_remove(curses[i])->query_remove(skill)) {
        params[1]->remove_curse(curses[i]);
        tell_object(params[1], "The curse of " + curses[i] + " is lifted.\n");
      }
*/
 
  /* cure all diseases */
/*
  if (sizeof((curses = (mixed *)params[1]->query_diseases())))
    for (i=0; i<sizeof(curses); i+=2)
      if (params[1]->query_remove(curses[i])->query_remove(skill)) {
        params[1]->remove_disease(curses[i]);
        tell_object(params[1], "The " + curses[i] + " disease is cured.\n");
      }
 */

  return 300;
  }
