#include "money.h"
#define MIN_LEVEL 100
#define SP_COST 200
#define AMOUNT_NEEDED 10000
 
mixed extra_life(string str, int skill, int cast);

string help() {
  return
"This spells gives the target an extra life.  It costs "+SP_COST+" spell "+
"points to cast, needs to be cast in a place of power and requires "+
MONEY_HAND->money_value_string(AMOUNT_NEEDED)+" to cast.  This is one "+
"one of the more powerful priest spells, you need to be "+MIN_LEVEL+
"faith.spells.curing.target to cast this spell.\n\n"+
"See also:\n"+
"  raise, resurect.\n";
}
 
int cast_extra_life(string str) {
  int   skill;
  mixed ret;
 
  if (this_player()->query_property("killed harry")) {
   write("Your god refuses to grant you the power to cast this spell.\n");
   return 1;
  }
  str = (string)this_player()->expand_nickname(str);
  skill = (int)this_player()->query_skill("faith.spells.curing.target");
  if (skill < MIN_LEVEL) {
    notify_fail("You are not powerful enough to give people extra lives.\n");
    return 0;
  }
  skill = (int)this_player()->query_skill_bonus("faith.spells.curing.target");
  ret = extra_life(str, skill, 1);
  if (stringp(ret)) {
    notify_fail(ret);
    return 0;
    }
  return 1;
  }
 
mixed extra_life(string str, int skill, int cast) {     
  mixed ob;
 
  if (!str || str == "")
    return "You completely unsucessfully manage to give nothing an "+
           "extra life.\n";
  if (!environment(this_player())->query_property("place of power"))
    return "This is not a place of power!  You cannot cast extra life "+
           "from here.\n";
  if (this_player()->query_value() < AMOUNT_NEEDED)
    return "You do not have enough money to draw power from to cast the "+
           "spell.\n";
  ob = find_match(str, environment(this_player()));
  if (sizeof(ob))
    ob = ob[0];
  else
    return "Who is " + capitalize(str) + "?\n";
  if (!living(ob))
    return (string)ob->query_cap_name() + " ignores you.\n";
  if (ob->query_property("noregen"))
    return "Death tells you: HOLD ON, I'M NOT FINISHED WITH THEM YET.\n";
  if (cast && (int)this_player()->adjust_gp(-SP_COST) < 0)
    return "Too low on power.\n";
  this_player()->pay_money(MONEY_HAND->create_money_array(AMOUNT_NEEDED));
  write("You call upon the gods to give an extra life to  " +
        (string)ob->query_cap_name() + ".  You hear the heavens roar and "+
        "a bolt of pure puce strikes down from the sky hitting "+
        (string)ob->query_cap_name() + " on the head.\n");
  tell_object(ob, this_player()->query_cap_name()+
        " call upon the gods to give an extra life to  " +
        "you.  You hear the heavens roar and "+
        "a bolt of pure puce strikes down from the sky hitting "+
        "you on the head.\n");
  ob->remove_ghost();
  say(this_player()->query_cap_name()+" call upon the gods to give an "+
        "extra life to  " +
        (string)ob->query_cap_name() + ".  You hear the heavens roar and "+
        "a bolt of pure puce strikes down from the sky hitting "+
        (string)ob->query_cap_name() + " on the head.\n");
  ob->adjust_max_deaths(1);
  this_player()->adjust_tmp_con(-10);
  return 1;
  }
