#define MIN_LVL 1
#define BACKSTAB_COST -50

mixed do_backstab(string str, int skill, int cast, object wpn);

int backstab(string str) {
  int skill, s1;
  mixed ret;
  object weap;

  skill = (s1 = (int)this_player()->query_skill_bonus("other.covert.stealth"))+
          (int)this_player()->query_skill_bonus("fighting.combat.melee.sharp");
  if (s1 < MIN_LVL) {
    notify_fail("You are not skillfull enough.\n");
    return 0;
  }
  weap = (object)this_player()->query_weapon();
  if(!weap) {
    notify_fail("Going to rip their heart out with your bare hands?\n");
    return 0;
  }
  if((string)weap->query_weap_skill() != "fighting.combat.melee.sharp") {
    notify_fail("You cannot stab with that weapon.\n");
    return 0;
  }
  ret = do_backstab(str, skill, 1, weap);
  if (stringp(ret)) {
    notify_fail(ret);
    return 0;
  }
  write("You prepare for your backstab.\n");
  return 1;
}

mixed do_backstab(string str, int skill, int cast, object wpn) {
  mixed ob;
/* now skill bonus... will range from about 1-250 or so...
 * make max damage 25 hp... ok skill/10.
 * no problems.
 * remember it will be random(dam) anyway ...
 * garanteeded of doing SOME damage.. ;)
 */
  if ((int)this_player()->query_spell_effect("attack")) 
/* used to add spells in the attack procedure. */
    return "You are already casting a spell.\n";
  ob = find_match(str, environment(this_player()));
  if (sizeof(ob))
    ob = ob[0];
   else
    return "Backstab whom?\n";
  this_player()->add_spell_effect(1, "attack", "Backstab", this_object(),
                         "heart_beat_backstab", ({ skill/10, ob, cast, wpn }));
  return 1;
}

int heart_beat_backstab(object attacker, mixed *params) {
  int stabber_lvl, dam;
  object wpn;

  if (params[2]) /* were we cast? */
     previous_object()->adjust_gp(BACKSTAB_COST);
  if(attacker) {
    write("You can't sneak up behind people in the heat of battle!\n");
    return 0;
  }
  attacker = params[1];
  if (!attacker) {
    write("It is beyond your backstab affecting it!\n");
    return 0;
  }
  wpn = (object)previous_object()->query_weapon();
  if(wpn != params[3]) {
    tell_object(previous_object(), "You are not using the same weapon!\n");
    return 0;
  }
  say(previous_object()->query_cap_name()+" backstabs "+
      attacker->query_name()+"\n", attacker);
  tell_object(attacker, previous_object()->query_cap_name()+
              " backstabs you.\n");
  tell_object(previous_object(), "You backstab "+attacker->query_name()+
                                 ".\n");
  attacker->attack_by(previous_object());
  dam = (int)wpn->weapon_attack(attacker, previous_object());
  stabber_lvl = (int)previous_object()->query_level();
  if(stabber_lvl)  dam /= stabber_lvl;
  return (int)attacker->query_level() * dam;
}

string help(string str) {
  return
"Syntax: backstab <person>\n\n"+
"The backstab action uses the skills other.covert.stealth and "+
"fighting.combat.melee.sharp for calcuating the bonus.  To backstab you "+
"need to actually have a sharp weapon, as backstabbing with your bare "+
"hands or a blunt weapon can be difficult.\n\n"+
"Example\n"+
"> backstab thief\n"+
"You prepare your backstab.\n"+
"You hit thief.\n"+
"You backstab thief.\n\n"+
"See also\n"+
"  Hmmm?\n";
}
