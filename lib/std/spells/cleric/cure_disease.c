/*
 * This will stop the person from fighting, unless it is an agressive
 * monster in which case...  It will keep beating him up.
 */
#define SP_COST 10
#define SKILL_CUTOFF 30
#define MIN_LVL 18
 
mixed cure_lt_wounds(string str, int skill, int cast);

string help() {
  return
"The cure disease spell does what it sounds like it does.  Cures any "+
"horrible and nasty diseases the character may have picked up.  The "+
"spell costs "+SP_COST+" to cast and uses the faith.spells.curing.target "+
"and faith.spells.curing.self depending on who you are cureing.\n\n"+
"See also:\n"+
"  Remove curse.\n";
}

mixed cure_disease(string str, int skill, int cast);

int cast_cure_disease(string str) {
  int skill;
  mixed ret;
 
  str = (string)this_player()->expand_nickname(str);
  skill = (int)this_player()->query_skill("faith.spells.curing.target");
  if (skill<MIN_LVL) {
    notify_fail("You are not skillful enough.\n");
    return 0;
  }
  skill = (int)this_player()->query_skill_bonus("faith.spells.defensive.target");
  ret = cure_disease(str, skill, 1);
  if (stringp(ret)) {
    notify_fail(ret);
    return 0;
  }
  write("You cast Cure disease.\n");
  return 1;
}
 
mixed cure_disease(string str, int skill, int cast) {
  mixed ob;
 
  if ((int)this_player()->query_spell_effect("spell")) 
    return "You are already casting a spell.\n";
  ob = filter_array(find_match(str, environment(this_player())), "check_liv",
                    this_object());
  if (!sizeof(ob))
    return "You must cast calm at someone.\n";
  this_player()->add_spell_effect(1, "spell", "Cure Disease",
    this_object(), "hb_cure_disease", ({ skill, ob, cast }));
  return 1;
}

int check_liv(object ob) {
  return living(ob);
}
 
int hb_cure_disease(object attacker, mixed *params) {
  int hp;
  mixed *succ, *to_low, *alist;
  mapping curses;
  string *bits;
  int i, j, skill, ep;
  string nam;

  to_low = succ = ({ });
  nam = (string)previous_object()->query_cap_name();
  skill = params[0];
  for (i=0;i<sizeof(params[1]);i++) {
    if (params[2] && (int)previous_object()->adjust_gp(-SP_COST) < 0) {
      to_low += params[1][i..i];
      continue;
    }
    succ += params[1][i..i];
    /* cure all diseases */
    if (m_sizeof((curses = (mapping)params[1]->query_diseases()))) {
      bits = m_indices(curses);
      for (i=0; i<sizeof(bits); i+=2)
        if (curses[bits[i]]->query_remove(skill)) {
          params[1]->remove_disease(bits[i]);
          tell_object(params[1], "The " + bits[i] + " disease is cured.\n");
          ep += 100;
        }
    }
    tell_object(params[1], nam+" cures all your diseases.\n");
  }
  if (sizeof(succ)) {
    string s;
    tell_object(previous_object(), "You cure "+
      (s=query_multiple_short(succ))+" disease\n");
    say(nam+" waves his arms around, calls upon the power of the gods "+
        "to cure "+s+"'s diseases.\n");
  }
  if (sizeof(to_low))
    tell_object(previous_object(), "You are too low on power to cure any of "+
                query_multiple_short(to_low)+" diseases.\n");
  return ep;
}
