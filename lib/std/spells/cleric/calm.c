/*
 * This will stop the person from fighting, unless it is an agressive
 * monster in which case...  It will keep beating him up.
 */
#define SP_COST 10
#define AGGRESSIVE_CUTOFF 150
#define SKILL_CUTOFF 30
#define MIN_LVL 5
 
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

mixed calm(string str, int skill, int cast);

int cast_calm(string str) {
  int skill;
  mixed ret;
 
  str = (string)this_player()->expand_nickname(str);
  skill = (int)this_player()->query_skill("faith.spells.defensive.target");
  if (skill<MIN_LVL) {
    notify_fail("You are not skillful enough.\n");
    return 0;
  }
  skill = (int)this_player()->query_skill_bonus("faith.spells.defensive.target");
  ret = calm(str, skill, 1);
  if (stringp(ret)) {
    notify_fail(ret);
    return 0;
  }
  write("You cast Calm.\n");
  return 1;
}
 
mixed calm(string str, int skill, int cast) {
  mixed ob;
 
  if ((int)this_player()->query_spell_effect("spell")) 
    return "You are already casting a spell.\n";
  ob = filter_array(find_match(str, environment(this_player())), "check_liv",
                    this_object());
  if (!sizeof(ob))
    return "You must cast calm at someone.\n";
  this_player()->add_spell_effect(1, "spell", "Cure Light Wounds",
    this_object(), "hb_calm", ({ skill, ob, cast }));
  return 1;
}

int check_liv(object ob) {
  return living(ob);
}
 
int hb_calm(object attacker, mixed *params) {
  int hp;
  mixed *succ, *to_low, *alist, *not_attacking;
  int i, j, skill;
  string nam;

  not_attacking = to_low = succ = ({ });
  nam = (string)previous_object()->query_cap_name();
  skill = params[0];
  for (i=0;i<sizeof(params[1]);i++) {
    if (!sizeof(alist = (object *)params[1][i]->query_attacker_list())) {
      not_attacking += params[1][i..i];
      continue;
    }
    if (params[2] && (int)previous_object()->adjust_gp(-SP_COST) < 0) {
      to_low += params[1][i..i];
      continue;
    }
    succ += params[1][i..i];
    if (params[1][i]->query_aggressive() && random(skill) < AGGRESSIVE_CUTOFF)
      continue;
    for (j=0;j<sizeof(alist);j++)
      if (random(skill) > SKILL_CUTOFF)
        params[1][i]->stop_fight(alist[j]);
    tell_object(params[1], nam+" calms you down.\n");
  }
  if (sizeof(succ)) {
    string s;
    tell_object(previous_object(), "You calm "+
      (s=query_multiple_short(succ))+" down.\n");
    say(nam+" calms "+s+" down.\n");
  }
  if (sizeof(to_low))
    tell_object(previous_object(), "You are too low on power to calm "+
                query_multiple_short(to_low)+" down.\n");
  if (sizeof(not_attacking))
    tell_object(previous_object(), query_multiple_short(not_attacking)+
                (sizeof(not_attacking)==1?" is ":" are ")+
                "not attacking anyone.\n");
  return sizeof(succ)*20;
}
