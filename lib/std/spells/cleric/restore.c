/*
 *  Restore: return all negative temporary stats to zero;
 */
 
#define MIN_LVL 40
#define SP_COST 33
#define EXP 50

string help() {
  return
"This spell returns all of your negative tempory stats to zero.  Nullifying "+
"the effects of death and poison potions of various sorts.  It costs "+
SP_COST+" guild points to cast.\n\n"+
"See also:\n"+
"  cure disease, remove curse, cure light wounds, heal.\n";
}
 
mixed restore(string str, mixed skill2, int cast);
 
int cast_restore(string str) {
  int skill1, skill2;
  mixed ret;
 
  if (this_player()->query_property("killed harry")) {
    write("Your god is not happy about the death of his high priest "+
          "he does not grant your request.\n");
    return 1;
  }
  str = (string)this_player()->expand_nickname(str);
  skill1 = (int)this_player()->query_skill("faith.spells.curing.self");
  skill2 = (int)this_player()->query_skill("faith.spells.curing.target");
  if (skill1 < MIN_LVL && skill2 < MIN_LVL) {
    notify_fail("You are not yet skillful enough to cast this spell.\n");
    return 0;
  }
  skill1 = (int)this_player()->query_skill_bonus("faith.spells.curing.self");
  skill2 = (int)this_player()->query_skill_bonus("faith.spells.curing.target");
  ret = restore(str, ({ skill1, skill2 }), 1);
  if (stringp(ret)) {
    notify_fail(ret);
    return 0;
  }
  return 1;
}
 
mixed restore(string str, mixed skill, int cast) {
  mixed ob;
  int i;
  object *peoples;
 
  if (this_player()->query_spell_effect("spell")) 
    return "You are already casting a spell.\n";
  ob = find_match(str, environment(this_player()));
   peoples = ({ });
   for (i = 0; i < sizeof(ob); i++)
      if (living(ob[i])) peoples += ({ ob[i] });
   ob = peoples;

  if (!sizeof(ob))
    return "You must cast restore at someone.\n";
  this_player()->add_spell_effect(1, "spell", "Restore",
    this_object(), "hb_restore", ({ skill, ob, cast }));
  return 1;
}

int hb_restore(object attacker, mixed *params) {
  int i, j, skill1, skill2, exp;
  object *dont, *to_low, *succ;

  succ = ({ });

  if (pointerp(params[0])) {
    skill1 =  params[0][0];
    skill2 =  params[0][1];
  } else
    skill1 = skill2 = params[0];
  dont = to_low = ({ });
  for (i=0;i<sizeof(params[1]);i++) {
    if (!living(params[1][i]))
      dont += params[1][i..i];
    if (params[2] && (int)previous_object()->adjust_gp(-SP_COST) < 0) {
      to_low += params[1][i..i];
      continue;
    }
    if (params[1][i] == previous_object()) {
      tell_object(previous_object(),
        "You cast a restore spell on yourself.  Amazing what a restore can "+
        "do.\n");
      say(
        (string)previous_object()->query_cap_name() +
        " waves " + (string)previous_object()->query_possessive() + " arms "+
        "around madly casting a spell on " +
        (string)previous_object()->query_objective() + "self.  " +
         capitalize((string)previous_object()->query_pronoun()) +
        " suddenly stops, leaps 3 metres in the air, " +
        (string)previous_object()->query_possessive() + " hair crackling " +
        "insanely.\n",
         params[1]);
    } else {
      succ += params[1][i..i];
      tell_object(params[1][i],
        (string)previous_object()->query_cap_name() +
        " mumbles something then points at you.  You cackle madly a bit "+
        "and your hair stands on end.  Wheee, what fun.\n");
    }
    /* fix up temporary stats */
    if ((int)params[1][i]->query_tmp_int() < 0) {
      params[1][i]->adjust_tmp_int(-((int)params[1][i]->query_tmp_int()));
      tell_object(params[1][i], "Your intelligence returns to normal.\n");
      exp += EXP;
    }
    if ((int)params[1][i]->query_tmp_dex() < 0) {
      params[1][i]->adjust_tmp_dex(-((int)params[1][i]->query_tmp_dex()));
      tell_object(params[1][i], "Your dexterity returns to normal.\n");
      exp += EXP;
    }
    if ((int)params[1][i]->query_tmp_wis() < 0) {
      params[1][i]->adjust_tmp_wis(-((int)params[1][i]->query_tmp_wis()));
      tell_object(params[1][i], "Your wisdom returns to normal.\n");
      exp += EXP;
    }
    if ((int)params[1][i]->query_tmp_str() < 0) {
      params[1][i]->adjust_tmp_str(-((int)params[1][i]->query_tmp_str()));
      tell_object(params[1][i], "Your strength returns to normal.\n");
      exp += EXP;
    }
    if ((int)params[1][i]->query_tmp_con() < 0) {
      params[1][i]->adjust_tmp_con(-((int)params[1][i]->query_tmp_con()));
      tell_object(params[1][i], "Your constitution returns to normal.\n");
      exp += EXP;
    }
    params[1][i]->reset_all();
  }
  if (succ) {
    string s;

    tell_object(previous_object(),
                "You successfully cast restore on "+
                (s=query_multiple_short(succ))+", wheeee!  That was fun.\n");
    say(previous_object()->query_cap_name()+" eyes light up, he makes slow "+
        "circling movements with his fingers then suddenly leaps in the "+
        "air whilst capering around "+s+".\n", succ);
  } else
    say(previous_object()->query_cap_name()+" eyes light up, he makes slow "+
        "circling movements with his fingers then suddenly leaps in the "+
        "air whilst capering around errr nothing.  They look very silly.\n");
  if (sizeof(to_low))
    tell_object(previous_object(),
      "You were too low in energy to cast restore on "+
      query_multiple_short(to_low)+".\n");
  if (sizeof(dont))
    tell_object(previous_object(),
      query_multiple_short(dont)+" do not appear to need any help.\n");
  return exp;
}
