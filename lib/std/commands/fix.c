#include "money.h"
#define TEACH_LEVEL 10
#define LEARN_LEVEL 2
#define SKILL "other"

int fix(string str) {
  object *obs, *too_poor, *ok, *fail;
  int w_skill, a_skill, low_c, max_c, high_c, total_cost, per, i, diff, val;

  if (!str) {
    notify_fail("Syntax: fix <armour(s)/weapon(s)>\n"+
                "PS you need to be in a smithy.\n");
    return 0;
  } 
  if (!environment(this_player())->query_property("smithy")) {
    notify_fail("You are not in a smithy so you cannot fix anything.\n");
    return 0;
  }
  obs = find_match(str, this_player()); /* can only fix things your
                                         * holding */
  obs = filter_array(obs, "check_arm_or_weap", this_object());
  if (!sizeof(obs)) {
    notify_fail("You need to actually fix something.\n");
    return 0;
  }
  a_skill = w_skill = (int)this_player()->query_skill_bonus("other.smithing")*10;
  ok = too_poor = fail = ({ });
  per = 100 - (int)environment(this_player())->query_discount();
  val = (int)this_player()->query_value();
  if (per < 0)
    per = 0;
  for (i=0;i<sizeof(obs);i++) {
    low_c = (int)obs[i]->query_lowest_cond();
    max_c = (int)obs[i]->query_max_cond();
    diff = (int)obs[i]->query_cond();
    if (obs[i]->query_weapon()) {
      diff = low_c + w_skill - diff;
      if (diff < 0)
        diff = 0;
    } else {
      diff = low_c + a_skill - diff;
      if (diff < 0)
        diff = 0;
    }
    if (!diff) {
      fail += ({ obs[i] });
      continue;
    }
    if (val < (diff*10*per)/100 + total_cost) {
      too_poor += ({ obs[i] });
      continue;
    }
    ok += ({ obs[i] });
    obs[i]->adjust_cond(diff);
    total_cost += (diff*10*per)/100;
  }
  if (total_cost)
    this_player()->pay_money((mixed *)MONEY_HAND->create_money_array(total_cost));
  if (sizeof(fail))
    write("You failed to fix "+query_multiple_short(fail)+".\n");
  if (sizeof(too_poor))
    write("You are too poor to afford the materials to fix "+
          query_multiple_short(too_poor)+".\n");
  if (sizeof(ok)) {
    write("You fix "+query_multiple_short(ok)+" and it costs you "+
          MONEY_HAND->money_value_string(total_cost)+".\n");
    say(this_player()->query_cap_name()+" fixes up "+
        query_multiple_short(ok));
  }
  return 1;
}

int check_arm_or_weap(object ob) {
  return (int)ob->query_weapon() || (int)ob->query_armour();
}

string help() {
  return 
"Syntax: fix <objects>\n\n"+
"This command allows you to fix up damaged weapons and armours.  Using "+
"weapons and armours in combat deteriorates their condition.  Using this "+
"command you can fix up the armour or weapon's condition so that it gives "+
"you more protection or does more damage again.  It costs you 10 copper "+
"pieces per point of condition you fix and you need to do it in a "+
"smithy.  You can only fix a weapon or armour so far above its minimun "+
"condition it has ever achieved.\n\n"+
"Examples\n"+
"> fix sword\n"+
"You fix up a long sword and it costs you 6 silver pieces.\n\n"+
"See also\n  Nothing as yet.\n";
}

int teach(object ob) {
  if (this_player()->query_skill(SKILL) < TEACH_LEVEL) {
    return -1;
  }
  if (ob->query_skill(SKILL) < LEARN_LEVEL) {
    return -2;
  }
  ob->add_known_command("fix");
  return 1;
}
