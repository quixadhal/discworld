#define MIN_LVL 0
#define DIVISOR 20
#define SP_COST 10
 
mixed cure_lt_wounds(string str, int skill, int cast);
 
string help() {
  return
"Cure light wounds can be cast on a person or on yourself.  It gives you "+
"back a small number of hit points and is the easiest of the curing "+
"spells.  It costs "+SP_COST+" spell points to cast, the amount of "+
"cureing done "+
"is dependant on your skill in faith.spells.curing.target and "+
"faith.spells.curing.self.\n\n"+
"See also:\n"+
"  cure serious wounds, cure critical wounds.\n";
}
 
int cast_cure_lt_wounds(string str) {
  int skill;
  mixed ret;
 
  str = (string)this_player()->expand_nickname(str);
  if (str==(string)this_player()->query_name())
    skill = (int)this_player()->query_skill("faith.spells.curing.self");
  else
    skill = (int)this_player()->query_skill("faith.spells.curing.target");
  if (skill<MIN_LVL) {
    notify_fail("You are not skillful enough.\n");
    return 0;
    }
  if (str==(string)this_player()->query_name())
    skill = (int)this_player()->query_skill_bonus("faith.spells.curing.self");
  else
    skill = (int)this_player()->query_skill_bonus("faith.spells.curing.target");
  skill += 20;
  ret = cure_lt_wounds(str, skill, 1);
  if (stringp(ret)) {
    notify_fail(ret);
    return 0;
    }
  write("You cast Cure Light Wounds.\n");
  return 1;
  }
 
mixed cure_lt_wounds(string str, int skill, int cast) {
  mixed ob;
 
  if ((int)this_player()->query_spell_effect("spell")) 
    return "You are already casting a  spell.\n";
  ob = find_match(str, environment(this_player()));
  if (sizeof(ob)>0)
    ob = ob[0];
  else
    ob = 0;
  this_player()->add_spell_effect(1, "spell", "Cure Light Wounds",
    this_object(), "hb_cure_lt_wounds", ({ skill/DIVISOR,ob,cast }));
  return 1;
  }
 
int hb_cure_lt_wounds(object attacker, mixed *params) {
  int hp;

  if (!params[1]) {
    tell_object(previous_object(),
      "Your spell failed ... there is no one of that name here.\n");
    return 0;
    }
  if (params[2] && (int)previous_object()->adjust_gp(-SP_COST)<0) {
    tell_object(previous_object(), "Too low on power.\n");
    return 0;
    }
  say((string)previous_object()->query_cap_name() +
    " cures some of " + (string)params[1]->query_cap_name() +
    "'s light wounds.\n", params[1]);
  tell_object(params[1], (string)previous_object()->query_cap_name() +
    " cures some of your light wounds.\n");
  tell_object(previous_object(), "You cure some of " +
    (string)params[1]->query_cap_name() + "'s light wounds.\n");
  hp = (int)params[1]->query_hp();
  if (hp == (int)params[1]->query_max_hp()) return 0;
  return ((int)params[1]->adjust_hp(params[0]) - hp)/5 + 10;
  }
