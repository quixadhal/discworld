#include "living.h"
inherit "/std/living/curses";
inherit "/std/living/force";
inherit "/std/living/skills";
inherit "/std/living/money";
inherit "/std/living/spells";
inherit "/std/living/armour";
inherit "/std/living/gender";
inherit "/std/living/stats";
inherit "/std/living/carrying";
inherit "/std/living/health";
inherit "/std/living/effects";
inherit "/std/container";

string msgout,
       msgin,
       mmsgout,
       mmsgin;
int verbose, level;
static mixed *it_them;
static object *followers;
static mixed remember_follow;

void create() {
  set_weap_skill("fighting.combat.melee.unarmed");
  armour::create();
  effects::create();
  curses::create();
  spells::create();
  skills::create();
  health::create();
  enable_commands();
  attack_name = ({ });
  attack_data = ({ });
  followers = ({ });
  max_hp = 1;
  max_gp = 1;
  container::create();
  it_them = ({ this_object() });
} /* create() */

void living_commands() {
  add_action("wield", "wi*eld");
  add_action("wear", "wea*r");
  add_action("hold", "ho*ld");
  add_action("do_equip", "eq*uip");
  add_action("remove", "remove");
  add_action("follow", "fo*llow");
  add_action("lose", "lo*se");
  add_action("unfollow", "unf*ollow");
  add_action("follow_dummy", "FOLLOW_DUMMY");
  combat_commands();
  skill_commands();
} /* living_commands() */

/* move stuff
 */
int test_add(object ob, int flag) { return !flag; }
int test_remove(object ob, int flag) { return !flag; }

static int in_move;

int query_in_move() { return in_move; }

/* thats it... 
 * no need for stupid checking to see if we are attacking something in
 * here stuff.
 */
varargs mixed move_player(string dir, string dest, mixed message,
                         object followee, mixed enter) {
  int i, ret, no_see;
  string arr, leave, ppl, my_short, from;
  object last, *ok_follow, new_env;

  if (!msgout || msgout[0] == '@') {
    msgout = "$N leave$s $T.";
    msgin = "$N arrive$s from $F.";
  }
  if (!mmsgout || mmsgout[0] == '@') {
    mmsgout = "$N disappear$s in a puff of smoke.";
    mmsgin = "$N appear$s out of the ground.";
  }
  last = environment();
  if(!this_object()->query_invis()) {
    my_short = (string)this_object()->short();
  }
  
  if(!my_short || my_short == "" || (stringp(message) && message == "none")) {
    no_see = 1;
  } else if(!dir || dir == "X") { /* we are teleporting */
/*
    leave = implode(explode(mmsgout, "$N"), my_short) + "\n";
    arr = implode(explode(mmsgin, "$N"), my_short) + "\n";
 */
    leave = mmsgout;
    arr = mmsgin;
  } else {
    if (!enter || !pointerp(enter))
      enter = ({ 0, "somewhere" });
    leave = msgout;
    if (pointerp(message))
      leave = message[0];
    leave = replace(leave, "$T", dir);
    arr = msgin;
    switch (enter[0]) {
      case 0 :
        arr = replace(arr, "$F", enter[1]);
        break;
      case 1:
        arr = enter[1];
        break;
    }
  }
/*
  if(!sizeof(followers) && !followee) {
    if (!enter || !pointerp(enter))
      enter = ({ 0, "somewhere" });
    if (pointerp(message))
      message = message[0];
    leave = implode(explode(implode(explode((message?message:msgout), "$N"), 
                    my_short), "$T"), dir)+"\n";
    switch (enter[0]) {
      case 0 :
        arr = implode(explode(implode(explode(msgin, "$N"), my_short), "$F"),
                    enter[1])+"\n";
        break;
      case 1 :
        arr = implode(explode(enter[1], "$N"), my_short)+"\n";
        break;
    }
  }
 */
  if(arr) ret = move(dest, arr, leave);
  else ret = move(dest);

  if(ret) return 0;

  new_env = environment();
  if (interactive(this_object()) && !query_property(UNKNOWN_MOVE_PROP)) {
    if (verbose) {
      this_object()->ignore_from_history("look");
      command("look");
    } else {
      this_object()->ignore_from_history("glance");
      command("glance");
    }
  }
  if(!dir || dir == "X" || (!sizeof(followers) && !followee)) return 1;
  else if(no_see) return ({});

  in_move = 1;
  ok_follow = ({ this_object() });
  for (i=0;i<sizeof(followers);i++) {
    object *tmp;
    if (followers[i] && !followers[i]->query_statue()) {
      if(environment(followers[i])==last && !followers[i]->query_in_move()) {
        tmp = (object *)followers[i]->do_follow_command(dir);
        followers[i]->reset_remember_follow();
        if(tmp && sizeof(tmp)) {
          tell_object(followers[i], "You follow " + my_short + " "+dir+".\n");
          ok_follow += tmp;
        } else {
          tell_object(followers[i], "You fail to follow " +
            my_short + " " +dir+ ".\n");
        }
      }
    } else {
      write("One of the people following you just quit or died a horrible "+
            "death.\n");
      followers = delete(followers, i, 1);
      i--;
    }
  }
  in_move = 0;
  if (followee)
    return ok_follow;
/*
 * Yes we have to do some silly things for multiple exit and enter
 * messages lest they make no sense.
 */
/*
  ppl = capitalize(query_multiple_short(ok_follow));
  if (pointerp(message))
    message = message[1];
  else
    message = "@$N leave $T.";
  leave = implode(explode(implode(explode(message, "$N"), 
                  ppl), "$T"), dir) + "\n";
  if (!enter || !pointerp(enter))
    enter = ({ 0, "somewhere" });
  switch (enter[0]) {
    case 0 :
      arr = "@" + ppl + " arrive from " + enter[1] + ".\n";
/*
      arr = implode(explode(implode(explode("@$N arrive from $F.", "$N"), ppl),
                    "$F"), enter[1])+"\n";
*/                    
/*
      break;
    case 1 :
      arr = implode(explode(enter[1], "$N"), ppl);
      break;
  }
  event(last, "exit", extract(leave, 1), new_env, ok_follow);
  event(new_env, "enter", extract(arr, 1), last, ok_follow);
 */
  if(sizeof(ok_follow) > 1)
    tell_object(this_object(), capitalize(query_multiple_short(ok_follow-
      ({ this_player() })))+" follow" + /* add 's' if only one follower */
      (sizeof(ok_follow) == 2?"s":"") + " you.\n");
  return ok_follow;
} /* move_player() */

object *do_follow_command(string dir)
{
  remember_follow = ({ previous_object(), dir });
  this_object()->ignore_from_history("FOLLOW_DUMMY");
  command("FOLLOW_DUMMY");
  return remember_follow;
}

void reset_remember_follow()
{
  remember_follow = 0;
}

int follow_dummy()
{
  remember_follow = (object *)environment()->do_exit_command(0,
    remember_follow[1], this_object(), remember_follow[0]);
  if(remember_follow && sizeof(remember_follow)) return 1;
  else return 0;
}

int cannot_get_stuff() { return 1; }

mixed *stats() {
  return container::stats() + ({
           ({ "max_hp", max_hp }),
           ({ "hp",  hp }),
           ({ "max_gp", max_gp }),
           ({ "gp", gp }),
           ({ "weapon", (!weapon?"(none)":(string)weapon->short()) }),
           ({ "Str", query_str() }),
           ({ "Int", query_int() }),
           ({ "Dex", query_dex() }),
           ({ "Con", query_con() }),
           ({ "Wis", query_wis() }),
           ({ "tmp Str", query_tmp_str() }),
           ({ "tmp dex", query_tmp_dex() }),
           ({ "tmp Con", query_tmp_con() }),
            ({ "tmp Int", query_tmp_int() }),
           ({ "tmp Wis", query_tmp_wis() }),
           ({ "bonus Str", query_bonus_str() }),
           ({ "bonus Int", query_bonus_int() }),
           ({ "bonus Dex", query_bonus_dex() }),
           ({ "bonus Con", query_bonus_con() }),
           ({ "Gender", query_gender_string() }),
           ({ "bonus Wis", query_bonus_wis() }),
           ({ "total money", query_value() }),
           ({ "Exp", query_xp() }),
           ({ "Total xp", query_total_xp() }),
        }) + armour::stats();
}

void dest_me() {
  object *ob;
  int i;

  ob = deep_inventory(this_object());
  for (i=0;i<sizeof(ob);i++)
    ob[i]->dest_me();
  container::dest_me();
}

mixed *query_it_them() { return it_them; }
mixed *set_it_them(mixed *i) { return (it_them = i); }

int add_follower(object ob) {
  if (ob == this_object())
    return 0;
  if (member_array(ob, attacker_list) != -1)
    return 0;
  if (member_array(ob, followers) == -1)
    followers += ({ ob });
  return 1;
}

int remove_follower(object ob) {
  int i;

  if ((i=member_array(ob, followers)) != -1) {
    followers = delete(followers, i, 1);
    return 1;
  }
  return 0;
}

int attack_by(object ob) {
  if (member_array(ob, followers) != -1)
    remove_follower(ob);
  armour::attack_by(ob);
} /* attack_by() */

int follow(string str) {
  int i;
  object *obs, *ok;
  string s;

  if (!str) {
    notify_fail("Syntax: follow <person>\n");
    return 0;
  }
  obs = find_match(str, environment());
  ok = ({ });
  if (!sizeof(obs)) {
    notify_fail("Could not find "+str+".\n");
    return 0;
  }
  for (i=0;i<sizeof(obs);i++)
    if (obs[i]->add_follower(this_object()))
      ok += obs[i..i];
  if (!sizeof(ok)) {
    notify_fail("You can only follow living things.\n");
    return 1;
  }
  write("You follow "+(s=query_multiple_short(ok))+".\n");
  say(this_player()->query_cap_name()+" follows "+s+".\n", ok);
  ok += ({ "you" });
  for (i=0;i<sizeof(ok)-1;i++)
    tell_object(ok[i], this_player()->query_cap_name()+
                " follows "+query_multiple_short(ok - ({ ok[i] }))+".\n");
  return 1;
}

int unfollow(string str) {
  int i;
  object *ok, *obs;
  string s;

  if (!str) {
    notify_fail("Syntax: unfollow <person>\n");
    return 0;
  }
  obs = find_match(str, environment());
  if (!sizeof(obs)) {
    notify_fail("I cannot find "+str+" to unfollow.\n");
    return 0;
  }
  ok = ({ });
  for (i=0;i<sizeof(obs);i++)
    if (obs[i]->remove_follower(this_player()))
      ok += obs[i..i];
  if (!sizeof(ok)) {
    notify_fail("You are not following "+query_multiple_short(obs)+".\n");
    return 0;
  }
  write("You stop following "+(s=query_multiple_short(ok))+".\n");
  say(this_player()->query_cap_name()+" stops following "+s+".\n", ok);
  ok += ({ "you" });
  for (i=0;i<sizeof(ok)-1;i++)
    tell_object(ok[i], this_player()->query_cap_name()+
              " stops following "+query_multiple_short(ok - ({ ok[i] }))+".\n");
  return 1;
}

int lose(string str) {
  int i;
  object *ok, *obs;
  string s;

  if (!str) {
    notify_fail("Syntax: lose <person|everyone>\n");
    return 0;
  }
  if (str == "everyone")
    obs = followers;
  else
    obs = find_match(str, environment());
  if (!sizeof(obs)) {
    notify_fail("I cannot find "+str+" to lose.\n");
    return 0;
  }
  ok = ({ });
  for (i=0;i<sizeof(obs);i++)
    if (remove_follower(obs[i]))
      ok += obs[i..i];
  if (!sizeof(ok)) {
    notify_fail("You are not being followed by "+
                query_multiple_short(obs)+".\n");
    return 0;
  }
  write("You lose "+(s=query_multiple_short(ok))+".\n");
  say(this_player()->query_cap_name()+" loses "+s+".\n", ok);
  ok += ({ "you" });
  for (i=0;i<sizeof(ok)-1;i++)
    tell_object(ok[i], this_player()->query_cap_name()+
                " loses "+query_multiple_short(ok - ({ ok[i] }))+".\n");
  return 1;
}
/* not needed is it?
void attack_by(object ob) {
  armour::attack_by(ob);
}
*/
object query_current_room() { return environment(); }

mixed *query_followers() { return followers; }

varargs int adjust_money(mixed amt, string type) {
  return money::adjust_money(amt, type);
} /* adjust_money() */

mixed *query_money_array() { 
  return money::query_money_array();
} /* query_money_array() */

int query_money(string type) { 
  return money::query_money(type);
} /* query_money() */

int query_value() { return money::query_value(); }
