#define MIN_LVL 1
#define AMBUSH_COST 50
#define STEALTH "other.covert.stealth"
#define HIDDEN_BONUS 50
#define TEACH_LEVEL 40
#define LEARN_LEVEL 10


/*
 * inherit "/std/weapon_logic";
 */

int ambush(string str) {
  int skill, mul, i;
  object weap, ambusher, *obs;
  string sname, mess;

  ambusher = this_player();

/*  Looks pretty finished to me...
  if (!ambusher->query_creator()) {
    notify_fail("Ambush not finished.\n");
    return 0;
  }
 */
  if(!str) {
    notify_fail("Usage: ambush <person>\n");
    return 0;
  }
  skill = (int)ambusher->query_skill_bonus(STEALTH);
  if(ambusher->query_hide_shadow()) skill += HIDDEN_BONUS;

  if (skill < MIN_LVL) {
    write("You are not stealthy enough even to try.\n");
    return 1;
  }
  weap = (object)ambusher->query_weapon();
  if(!weap) weap = ambusher;
  skill += (int)weap->query_property("ambush bonus");
  sname = (string)weap->query_weap_skill();
  switch(sname) {
    case "fighting.combat.melee.sharp":
      mess = "backstab";
      break;
    case "fighting.combat.melee.blunt":
      mess = "cosh";
      break;
    case "fighting.combat.melee.unarmed":
      mess = "Spock pinch";
      break;
    default:
      write("You cannot ambush with that type of weapon.\n");
      return 1;
  }
  obs = find_match(str, environment(ambusher));
  if (!sizeof(obs)) {
    write("Ambush whom?\n");
    return 1;
  }
  if(sizeof((object *)ambusher->query_attacker_list())) {
    write("You can't sneak up behind people in the heat of battle!\n");
    return 1;
  }
  if((int)ambusher->adjust_gp(AMBUSH_COST) == -1) {
    write("You lack sufficient guild points to Ambush at the moment.\n");
    return 1;
  }
  if(userp(obs[0]) && userp(ambusher)) {
    write("You may not ambush "+obs[0]->query_name()+".\n");
    return 1;
  }
  write("You " + mess + " "+obs[0]->query_name()+".\n");
  mess = pluralize(mess);
  say(ambusher->query_cap_name()+" " + mess + " "+
      obs[0]->query_name()+".\n", obs[0]);
  tell_object(obs[0], ambusher->query_cap_name()+
              " " + mess + " you.\n");
  obs[0]->attack_by(ambusher);
  ambusher->attack_by(obs[0]);
  ambusher->modify_skill(sname, skill);
  weap->weapon_attack(obs[0], ambusher);
  ambusher->modify_skill(sname, -skill);
  return 1;
}

string help(string str) {
  return "Syntax: ambush <person>\n\n"+
"The ambush action gives a bonus to your first attack equal to your skill in " +
STEALTH + ". This bonus can only be applied to melee weapons (or unarmed " +
"combat) " +
"To ambush you must not be in combat yourself (though your target may be). " +
"Also, being hidden when you ambush gives a +" + HIDDEN_BONUS +
"bonus to this action.\n" +
"Example:\n" +
"> wield club\n" +
"> ambush thief\n" +
"You cosh thief.\n\n" +
"See also\n"+
"  Hmmm?\n";
}

int teach(object ob) {
  if (this_player()->query_skill(STEALTH) < TEACH_LEVEL)
    return -2;
  if (ob->query_skill(STEALTH) < LEARN_LEVEL)
    return -1;
  ob->add_known_command("ambush");
  return 1;
}
