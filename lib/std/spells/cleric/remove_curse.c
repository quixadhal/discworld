/*
 * This will stop the person from fighting, unless it is an agressive
 * monster in which case...  It will keep beating him up.
 */
#define SP_COST 50
#define SKILL_CUTOFF 34
#define MIN_LVL 25
 
mixed cure_lt_wounds(string str, int skill, int cast);

string help() {
  return
"The calm spell allows you to calm monsters down.  Calming monsters down "+
"means that they stop fighting you.  The spell costs 10 spell points per "+
"target cast upon. The success of this spell working "+
"is based upon your skill in faith.spells.defensive.target.\n\n"+
"See also:\n"+
"  None.\n";
}

mixed remove_curse(string str, int skill, int cast);

int cast_remove_curse(string str) {
  int skill;
  mixed ret;
 
  str = (string)this_player()->expand_nickname(str);
  skill = (int)this_player()->query_skill("faith.spells.curing.target");
  if (skill<MIN_LVL) {
    notify_fail("You are not skillful enough.\n");
    return 0;
  }
  skill = (int)this_player()->query_skill_bonus("faith.spells.curing.target");
  ret = remove_curse(str, skill, 1);
  if (stringp(ret)) {
    notify_fail(ret);
    return 0;
  }
  write("You cast Remove curse.\n");
  return 1;
}
 
mixed remove_curse(string str, int skill, int cast) {
  mixed ob;
 
  if ((int)this_player()->query_spell_effect("spell")) 
    return "You are already casting a spell.\n";
  ob = filter_array(find_match(str, environment(this_player())), "check_liv",
                    this_object());
  if (!sizeof(ob))
    return "You must cast calm at someone.\n";
  this_player()->add_spell_effect(1, "spell", "Remove Curse",
    this_object(), "hb_remove_curse", ({ skill, ob, cast }));
  return 1;
}

int check_liv(object ob) {
  return living(ob);
}
 
int hb_remove_curse(object attacker, mixed *params) {
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
    /* cure all curses */
    if (m_sizeof((curses = (mapping)params[1][i]->query_curse()))) {
      bits = m_indices(curses);
      for (i=0; i<sizeof(bits); i+=2)
        if (curses[bits[i]]->query_remove(skill)) {
          params[1]->remove_curse(bits[i]);
          tell_object(params[1], "The " + bits[i] + " curse is removed.\n");
          ep += 100;
        }
    }
    tell_object(params[1], nam+" removes all your curses.\n");
  }
  if (sizeof(succ)) {
    string s;
    tell_object(previous_object(), "You remove "+
      (s=query_multiple_short(succ))+"'s curses.\n");
    say(nam+" waves his arms around, calls upon the power of the gods "+
        "to remove "+s+"'s curses.\n");
  }
  if (sizeof(to_low))
    tell_object(previous_object(), "You are too low on power to remove any of "+
                query_multiple_short(to_low)+" curses.\n");
  return ep;
}
