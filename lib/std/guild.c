inherit "/std/room";

#include "skills.h"
#include "tune.h"
#include "money.h"

string our_guild;
mixed *spells,
      *commands;
object teaching_person;

void check_spells_commands(string *sk, int lvl);

void create() {
  spells = ({ });
  commands = ({ });
  ::create();
} /* create() */

void init() {
  ::init();
  add_action("do_advance", "ad*vance");
  add_action("do_join", "join");
  add_action("do_info", "inf*o");
  add_action("do_cost", "co*st");
} /* init() */

void set_guild(string str) { our_guild = str; }

int do_advance(string str) {
  string skill_name, sk, g_o;
  string *bits;
  int cost, i, lvl, tmp, to, by, max_lvl, total_xp, total_cost, p_val;

  if ((g_o=(string)this_player()->query_guild_ob()) != our_guild && g_o) {
    notify_fail("You cannot advance here! Go to your own guild.\n");
    return 0;
  }
  if (!str) {
    notify_fail(sprintf("Syntax: %-*#s\n", this_player()->query_cols(),
        "advance <skill>\nadvance <skill> to <level>\n" +
        "advance <skill> by <level>\n"));
    return 0;
  }
  if ((sscanf(str, "%s to %d", sk, to) != 2) &&
      (sscanf(str, "%s by %d", sk, by) != 2)) {
    sk = str;
    by = 1;
  }
  bits = explode(implode(explode(sk, " "), "."), ".");
  if (!(skill_name = (string)SKILL_OB->query_skill(bits))) {
    notify_fail("I am sorry but the skill " + sk + " does not exist.\n");
    return 0;
  }
  lvl = (int)this_player()->query_skill(skill_name);
  if(!by && to) by = to - lvl;
  if(by <= 0) {
    if(by == 0) notify_fail("You are already at level " + lvl + " at " +
        skill_name + ".\n");
    else notify_fail("You cannot regress below your current skill level!\n");
    return 0;
  }
  if ((sizeof(explode(skill_name, "."))-1)*5 > lvl) {
    notify_fail("Not a high enough level in the outer skills to advance "+
                "that yet.\n");
    return 0;
  }
  if (!g_o)
    max_lvl = 5;
  else
    max_lvl = (int)our_guild->query_skill_max_level(skill_name);
  if (lvl >= max_lvl) {
    notify_fail("You cannot advance in that skill any further here.\n");
    return 0;
  }
  if (!g_o)
    cost = DEFAULT_COST; /* The guild trains cheaper.  Better thingys */
  else
    cost = (int)our_guild->query_skill_cost(skill_name);
  cost *= (int)SKILL_OB->query_skill_cost(skill_name);
  cost *= STD_COST/5;
  p_val = (int)this_player()->query_value();
  for (i=0;i<by;i++) {
    tmp = cost * ( ((lvl + i)/LEVEL_DIV) + 1);
    if(this_player()->query_xp() < tmp+total_xp) {
      if(!i) {
        notify_fail("Your lack of experience prevents you improving " +
          sk + ".\n");
        return 0;
      } else {
        write("Your lack of experience prevents you taking all the " +
        "advancement you requested.\n");
      }
      break;
    }
    if (lvl + i >= max_lvl) {
      write("Your guild hasn't the resources to train you as far as "+
            "you requested.\n");
      break;
    }
    if (((tmp/COST_DIV)+total_cost) > p_val) {
      if(!i) {
        notify_fail("Due to your sad lack of money the guild cannot train "+
                    "you in "+sk+", it would have cost you "+
                    MONEY_HAND->money_value_string(tmp/COST_DIV)+".\n");
        return 0;
      } else {
        write("Due to your sad lack of money the guild cannot train "+
             "you in "+sk+" to "+(i+lvl)+", it would have cost you "+
             MONEY_HAND->money_value_string(tmp/COST_DIV)+".\n");
      }
      break;
    }
    total_xp += tmp;
    total_cost += tmp/COST_DIV;
  }
  this_player()->adjust_xp(-total_xp);
  this_player()->pay_money(MONEY_HAND->create_money_array(total_cost));
  this_player()->add_skill_level(skill_name, i);
  this_player()->reset_all();
  event(this_object(), "guild_advance", bits, lvl, lvl+i);
  check_spells_commands(bits, lvl+i);
  write("You advance your skill in " + sk + " from " + lvl + " to " +
        (lvl + i) + " for " + total_xp + " xp and " +
        MONEY_HAND->money_value_string(total_cost) + ".\n");
  say(this_player()->query_cap_name()+" advances "+
      this_player()->query_possessive()+" skills.\n");
  return 1;
} /* do_advance() */

int do_join(string str) {
  if((string)this_player()->query_guild_ob() == our_guild) {
    notify_fail("You're already in this guild.\n");
    return 0;
  }
  if(this_player()->query_guild_ob()) {
    notify_fail("You cannot join a guild while a member of another.\n");
    return 0;
  }
  write("You will only ever get to join one guild.  Are you sure? ");
  input_to("join2");
  return 1;
} /* do_join() */


int join2(string str) {
  mixed skills;
  int i;

  str = lower_case(str);
  if (str[0] != 'n' && str[0] != 'y') {
    write("I do not understand.  Yes or no? ");
    input_to("join2");
    return 1;
  }
  if (str[0] == 'n') {
    write("Ok, not joining the "+ our_guild->query_name()+" guild.\n");
    return 1;
  }
  this_player()->set_guild_ob(our_guild);
  this_player()->race_guild_commands();
  write("You are now a member of the "+our_guild->query_name()+" guild.\n");
  say(this_player()->query_cap_name()+" joins the "+our_guild->query_name()+
          "guild.\n");
  event(this_object(), "guild_join");
  event(users(), "inform", this_player()->query_cap_name()+" is now a member "+
            "of "+our_guild->query_name(), "guild");
  skills = (mixed *)this_player()->query_skills();
  for (i=0;i<sizeof(skills);i+=SKILL_ARR_SIZE) {
/* Only do the top level */
    check_spells_commands(skills[i+SKILL_NAM..i+SKILL_NAM],
                          skills[i+SKILL_LVL]);
  }
  return 1;
} /* join2() */

int do_leave(string str) {
  if(str != "guild") {
    notify_fail("Syntax : leave guild\n");
    return 0;
  }
  if ((string)this_player()->query_guild_ob() != our_guild) {
    notify_fail("How can you leave a guild if you aren't in it.\n");
    return 0;
  }
  if ((int)this_player()->query_level() > 5) {
    notify_fail("You are too high a level to leave this guild.\n");
    return 0;
  }
  write("Are you sure you want to leave the guild (Y/N) : ");
  input_to("confirm_leave");
  return 1;
} /* do_leave() */

void confirm_leave(string str) {
  str = lower_case(str);
  if(str != "y") {
    write("You are still in the guild.\n");
    return;
  }
  write("Goodbye.\n");
  call_other(our_guild, "leaving_guild");
  this_player()->set_start_pos("d/am/am/mendeddrum");
  this_player()->set_guild_ob(0);
  this_player()->set_guild_data(0);
  this_player()->guild_commands();
  this_player()->save_me();
} /* confirm_leave() */

string rec_cost_old(mixed *arr, string path, int depth) {
  int i;
  int cost;
  string str, lpath;

  str = "";
  for (i=0;i<sizeof(arr);i+=SKILL_ARR_SIZE) {
    lpath = path + "." + arr[i];
    cost = (int)SKILL_OB->query_skill_cost((lpath)[1..500]);
    cost *= (int)our_guild->query_skill_cost((lpath)[1..500]);
    str += sprintf("%*'| 's%-*'.'s costs %d xp.\n", (depth-1)*2, "",
             20-(depth-1)*2, arr[i],
             cost*STD_COST*(((int)this_player()->query_skill(lpath)
                  /LEVEL_DIV)+1));
    if (this_player()->query_skill(lpath) >= depth*5)
      str += rec_cost_old(arr[i+SKILL_BIT], path+"."+arr[i], depth+1);
  }
  return str;
} /* rec_cost_old() */

string rec_cost(mixed *arr, string path, int depth, string g_o)
{ /* use lots of variables...so we only work things out minimum times */
  int i, depth_gap, ndots;
  int cost, lvl, max_lvl;
  string str, lpath;

  str = "";
  depth_gap = (depth - 1) * 2;
  ndots = 18 - depth_gap;
  
  for (i=0;i<sizeof(arr);i+=SKILL_ARR_SIZE) {
    if (path != "")
      lpath = path + "." + arr[i];
    else
      lpath = arr[i];
    lvl = (int)this_player()->query_skill(lpath);
    if (!g_o)
      max_lvl = 5;
    else
      max_lvl = (int)g_o->query_skill_max_level(lpath);
    if(lvl >= max_lvl) {
      str += sprintf("%*'| 's%-*'.'s %3d/%3d  mastered\n", depth_gap, "",
                     ndots, arr[i], lvl, max_lvl);
    } else {                     
      if (!g_o)
        cost = DEFAULT_COST;
      else
        cost = (int)g_o->query_skill_cost(lpath);
      cost *=  (int)SKILL_OB->query_skill_cost(lpath);
      cost *= (STD_COST/5)*((lvl/LEVEL_DIV)+1);
      str += sprintf("%*'| 's%-*'.'s %3d/%3d %6d xp\n", depth_gap, "",
               ndots, arr[i], lvl, max_lvl, cost);
    }
    if (lvl >= depth*5)
      str += rec_cost(arr[i+SKILL_BIT], lpath, depth+1, g_o);
  }
  return str;
} /* rec_cost() */

int do_cost(string str) {
  string *bits;
  mixed *arr;
  int i, cost, lvl, max_lvl, tmp, by, to;
  string skill_name, sk, g_o;

  if(!str) {
    notify_fail(sprintf("Syntax: %-*#s\n", this_player()->query_cols(),
      "cost all\ncost <skill>\ncost <skill> to <level>\n" +
      "cost <skill> by <level>\n"));
    return 0;
  }
  if ((g_o = (string)this_player()->query_guild_ob()) != our_guild && g_o) {
    notify_fail("You are not a member of this guild.\n");
    return 0;
  }
  arr = (mixed *)SKILL_OB->query_skills();
  if (!arr) {
    notify_fail("Hmmm, we appear to have an error here.\n");
    return 0;
  }
  if (str == "old") {
    printf("%#*-s\n", this_player()->query_cols(), rec_cost_old(arr, "", 1));
  } else if ( str == "all") {
    printf("%-*'='s\n", this_player()->query_cols(),
        "======SKILLS=======Cur/Max==For Next");
    printf("%#*-s\n", this_player()->query_cols(), rec_cost(arr, "", 1, g_o));
    printf("%*'='|s\n", this_player()->query_cols(), "> You have " +
                        this_player()->query_xp() + " points to spend <");
  } else {
    if((sscanf(str, "%s to %d", sk, to) !=2) &&
       (sscanf(str, "%s by %d", sk, by) !=2)) {
        sk = str;
        by = 1;
    }
    bits = explode(sk, ".");
    if (!(skill_name = (string)SKILL_OB->query_skill(bits))) {
      notify_fail("There is no such skill as " + sk + ".\n");
      return 0;
    }
    lvl = (int)this_player()->query_skill(skill_name);
    if(!by && to) by = to - lvl;
    if(by <= 0) {
      if(by == 0)
        notify_fail("It won't cost you anything to stay at the same level!\n");
      else  
        notify_fail("We can't give refund xp for regressing skills!\n");
      return 0;
    }
    if ((sizeof(explode(skill_name, "."))-1)*5 > lvl) {
      notify_fail("Not a high enough level in the outer skills to advance " +
        "that yet.\n");
      return 0;
    }
    if (!g_o)
      max_lvl = 5;
    else
      max_lvl = (int)our_guild->query_skill_max_level(skill_name);
    if(lvl >= max_lvl) {
      notify_fail("Your guild prevents you even considering advancing " + sk +
        " above level " + max_lvl + ".\n");
      return 0;
    }
    if (!g_o)
      cost = DEFAULT_COST;
    else
      cost = (int)our_guild->query_skill_cost(skill_name);
    cost *= (int)SKILL_OB->query_skill_cost(skill_name);
    cost *= STD_COST/5;
    for (i=0;i<by;i++) {
      if (lvl + i >= max_lvl) {
        write("You guild prevents you considering all the advancement you " +
          "requested.\n");
        break;
      }
      tmp += cost * ( ((lvl + i)/LEVEL_DIV) + 1);
    }
    printf("It would cost you %d xp to raise %s from level %d to %d.\n",
        tmp, skill_name, lvl, (lvl + i));
  }
  return 1;
} /* do_cost() */

int do_info() {
  mixed *arr;

  write(our_guild->long()+"\nSkill costs if you join this guild.\n");
  arr = (mixed *)this_player()->query_skills();
  printf("%-*'='s\n", this_player()->query_cols(),
      "======SKILLS=======Cur/Max==For Next");
  printf("%#*-s\n", this_player()->query_cols(), rec_cost(arr, "", 1,
                    our_guild));
  printf("%*'='|s\n", this_player()->query_cols(), "> You have " +
                      this_player()->query_xp() + " points to spend <");
  return 1;
} /* do_info() */

/*
 * From here on are the changes to make level gaining hopefully more
 * realistic...  (Note the more :)
 */
void add_spell(string name, string *path, int lvl) {
  spells += ({ name, path, lvl });
} /* add_spell() */

void add_command(string name, string *path, int lvl) {
  commands += ({ name, path, lvl });
} /* add_command() */

void set_teaching_person(object ob) { teaching_person = ob; }
object query_teaching_person() { return teaching_person; }

void check_spells_commands(string *path, int lvl) {
  int i, j;

  if (!teaching_person || member_array(this_player(), 
      (object *)teaching_person->query_attacker_list()) != -1)
    return ;

  for (i=0;i<sizeof(spells);i+=3)
    for (j=0;j<sizeof(spells[i+1]);j++)
      if (j >= sizeof(path)) {  /* got through the skill path ok */
        if (spells[i+2] <= lvl)
           teaching_person->init_command("teach "+spells[i]+" to "+
                                         this_player()->query_name());
      } else
        if (spells[i+1][j] != path[j])
          break;

  for (i=0;i<sizeof(commands);i+=3)
    for (j=0;j<sizeof(commands[i+1]);j++)
      if (j >= sizeof(path)) {  /* got through the skill path ok */
        if (commands[i+2] <= lvl)
           teaching_person->init_command("teach "+commands[i]+" to "+
                                         this_player()->query_name());
      } else
        if (commands[i+1][j] != path[j])
          break;
} /* check_spell_commands() */

query_spells() { return spells + ({ }); }
query_commands() { return commands + ({ }); }
