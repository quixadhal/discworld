/* the players armour handling routines.
 * wear_armour(object ob)
 * remove_armour(object ob)
 * query_ac(int)
 */
inherit "/std/armour_logic";
inherit "/std/living/combat";
#include "race.h"

static object *armours, held_ob;

create() {
  ac = ({ });
  armours = ({ });
  armour_types = ({ });
  combat::create();
} /* create() */

object *query_armours() { return armours; }

string wear_armour(object armour) {
  int num, i;
  string type;
  object race_ob;

  if (!ac)
    ac = ({ });
  if (!armours)
    armours = ({ });
  type = (string)armour->query_type();
  if(type == "shield" && held_ob)
    return "already holding";
  while (i<sizeof(armours)) {
    if (!armours[i]) /* oopps */ {
      armours = delete(armours,i,1);
      continue;
    }
    if (type == (string)armours[i]->query_type())
      num++;
    i++;
  }
  race_ob = (object)this_object()->query_race_ob();
  if (!race_ob) {
    race_ob = find_object(STD_RACE);
    if (!race_ob)
      call_other(STD_RACE, "??");
    race_ob = find_object(STD_RACE);
  }
  if (num > race_ob->query_number_worn(type))
    return type;
  if (!armour->set_worn_by(this_object()))
    return "Oh dear";
  armours += ({ armour });
  return 0;
} /* wear_armour() */

int remove_armour(object armour) {
  int i;

  if (!armour)
    return 0;
  if (!armours)
    armours = ({ });
  if (!ac)
    ac = ({ });
  if ((i=member_array(armour, armours)) == -1)
    return 1;
  if (!armour->set_worn_by(0))
    return 1;
  armours = delete(armours,i,1);
  return 0;
} /* remove_armour() */

int query_ac(string type, int dam) {
  int val,i;

  if (!ac) {
    ac = ({ });
    armours = ({ });
  }
  val = ::query_ac(type, dam)*(dodging+50)/300;
  for (i=0;i<sizeof(armours);i++)
    val += (int)armours[i]->query_ac(type, dam);
  return val;
} /* query_ac() */

int wear(string str) {
  int i;
  string s, *thing, ret, astr;
  object *obs, *ok, *ufail, *already;
  mapping fail;

  if (!str) {
    notify_fail("Usage: wear <armour>\n");
    return 0;
  }
  obs = find_match(str, this_object());
  if (!sizeof(obs)) {
    notify_fail("You aren't carrying "+str+".\n");
    return 0;
  }
  ok = ({ });
  fail = ([ ]);
  ufail = ({ });
  already = ({ });
  for (i=0;i<sizeof(obs);i++)
    if (obs[i]->query_armour())
      if (member_array(obs[i], armours) != -1)
        already += ({ obs[i] });
      else if ((s=wear_armour(obs[i])))
        if (!fail[s])
          fail[s] = ({ obs[i] });
        else
          fail[s] += ({ obs[i] });
      else {
        ok += ({ obs[i] });
    } else
      ufail += ({ obs[i] });

  ret = "";
  astr = "";
  if (sizeof(ufail))
    ret +=capitalize(query_multiple_short(ufail) + " can't be worn.\n");
  if(sizeof(already))
    ret += "You are already wearing " + query_multiple_short(already) + ".\n";
  thing = m_indices(fail);
  for (i=0;i<sizeof(thing);i++)
    if(thing[i] == "already holding")
      astr += "You can't wear " + query_multiple_short(fail[thing[i]])+
              " as you are holding " + held_ob->short() + ".\n";
    else
      astr += "You can't wear "+query_multiple_short(fail[thing[i]])+
              " as you are already wearing something of type "+thing[i]+".\n";
  if(!sizeof(ok)) {
    notify_fail(ret + astr);
    return 0;
  }
/* changed to work around this_object() rather than this_player() by
   [als] 92.Oct(25) */
  tell_object(this_object(), ret + "You wear "+(ret=query_multiple_short(ok))+".\n" + astr);
  tell_room(environment(), this_object()->query_cap_name()+" wears "+ret+".\n", this_object());
  return 1;
}

int remove(string str) {
  int i;
  string s, ret;
  object *obs, *fail, *ok;

  if (!str) {
    notify_fail("Usage: remove <armour>\n");
    return 0;
  }
  obs = find_match(str, this_object());
  if (!sizeof(obs)) {
    notify_fail("Cannot find "+str+".\n");
    return 0;
  }
  ok = ({ });
  fail = ({ });
  for (i=0;i<sizeof(obs);i++)
    if (obs[i]->query_armour())
      if (remove_armour(obs[i]))
        fail += ({ obs[i] });
      else
        ok += ({ obs[i] });
    else
      fail += ({ obs[i] });
  if (!sizeof(ok)) {
    notify_fail("You are not wearing "+query_multiple_short(fail)+".\n");
    return 0;
  }
  if (sizeof(fail))
    write("You are not wearing "+query_multiple_short(fail)+".\n");
  write("You remove "+(ret=query_multiple_short(ok))+".\n");
  say(this_player()->query_cap_name()+" removes "+ret+".\n");
  return 1;
}

mixed *stats() {
  return armour_logic::stats() +
         combat::stats() +
         ( held_ob?({ ({ "Holding", held_ob->short(0) }) }):({ }) );
}

int hold(string str) {
  int i;
  object *obs;

  if (!str) {
    notify_fail("Usage: hold <torch>\n");
    return 0;
  }
  if(str == "hands" || str == "nothing") {
    if(held_ob)
      write("You lower your " + (string)held_ob->short() + ".\n");
    else {
      notify_fail("You are not holding anything.\n");
      return 0;
    }
    held_ob->set_holder(0);
    held_ob = 0;
    return 1;
  }
  obs = find_match(str, this_object());
  if (!sizeof(obs)) {
    notify_fail("Cannot find "+str+".\n");
    return 0;
  }
  for(i=0; i<sizeof(armours); i++)
    if((string)armours[i]->query_type() == "shield") {
      notify_fail("You can't hold anything while wearing a shield.\n");
      return 0;
    }
  for (i=0;i<sizeof(obs);i++)
    if (obs[i]->query_holdable()) {
      if (held_ob && !held_ob->set_holder(0))
        continue;
      if (!obs[i]->set_holder(this_object()))
        continue;
      held_ob = obs[i];
      if(this_object() != this_player()) return 1;      
      say(this_object()->query_cap_name()+" holds "+held_ob->short()+
          ".\n");
      if (this_player() == this_object()) write("You hold "+
          held_ob->short()+".\n");
      return 1;
    }
  notify_fail(str+" may not be held.\n");
  return 0;
}

int unhold_ob(object ob) {
  if (ob == held_ob) {
    held_ob = 0;
    return 1;
  }
  return 0;
}

object query_held_ob() { return held_ob; }
void set_held_ob(object h) { held_ob = h; }

int do_equip()
{
  int i, wielded, held, shield;
  object *obs, *equipped;
  string sh;
  mixed tmp;
  
  equipped = ({ });

  if(query_weapon()) wielded = 1;
  if(held_ob) held = 1;
  if(!held_ob)
    for(i=0; i<sizeof(armours); i++)
      if((string)armours[i]->query_type() == "shield") {
        shield = 1;
        break;
      }
  obs = all_inventory(this_object());
  for(i=0;i<sizeof(obs);i++)
    if(obs[i]->query_armour()) {
      if(!wear_armour(obs[i]))
        equipped += ({ obs[i] });
    }
    else if(!wielded && obs[i]->query_weapon()) {
      if ((tmp = obs[i]->set_wield(this_object())) && !stringp(tmp)) {
        set_weapon(obs[i]);
        equipped += ({ obs[i] });
        wielded = 1;
      }
    }
    else if(!held && !shield && obs[i]->query_holdable()) {
      obs[i]->set_holder(this_object());
      held_ob = obs[i];
      equipped += ({ obs[i] });
      held = 1;
    }
  if(this_object() != this_player()) return 1;
  if(!sizeof(equipped)) {
    if(wielded || held || sizeof(armours))
      notify_fail("You are already using all the items you can.\n");
    else
      notify_fail("You have no equipable items.\n");
    return 0;
  } else {
    write("You equip " + (sh = query_multiple_short(equipped)) + ".\n");
    say(this_player()->query_cap_name() + " equips " + sh + ".\n");
    return 1;
  }
} /* do_equip() */
