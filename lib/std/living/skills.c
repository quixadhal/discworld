/*
 * sigh, this will be interesting wont it?
 * quick summary of routines
 * skill_bonus(string)         - gives the bonus...
 * skill_lvl(string)           - gives the raw level, with out stat bonus etc
 * modify_skill(string, int)   - modifies the skills level by int.
 * calc_bonus(int, string)     - given the skill lvl, stat bonus str cals bonus
 * calc_lvl(string *)          - calculate number of lvls in the path
 * add_skill_lvl(...)          - horror recursive skill adder. arghh
 * teach_skill(objct *,string) - Used to teach skills to other people.
 * skill_commands()            - all the skill add_actioned commands.
 */
/* alternative method of doing skills...
 */
 
#include "skills.h"
#include "tune.h"

int calc_bonus(int lvl, string skill);
int stat_modify(int bonus, string skill);
mixed recursive_skill_add(mixed skil, string *path, int avr, int lvl, int exp,
                           mixed standard);

mixed *skills;
 
private static int delta;
static mapping bonus_cache,
               level_cache,
               stat_cache,
               teach_offer;

void skill_commands() {
  add_action("do_learn", "le*arn");
} /* skill_command() */
 
mixed *query_skills() { return skills; }
void set_skills(mixed *bing) { skills = bing; }
int calc_level(string *path);

void create() {
  bonus_cache = ([ ]);
  level_cache = ([ ]);
  teach_offer = ([ ]);
  stat_cache = ([ ]);
  skills = ({ });
} /* create() */

mapping query_bonus_cache() { return bonus_cache; }
mapping query_level_cache() { return level_cache; }
mapping query_stat_cache() { return stat_cache; }

void zap_stat_cache(string stat) {
  int i, j;
  mapping bing;
  string *bits;

  for (i=0;i<strlen(stat);i++)
    if ((bing = stat_cache[stat[i]])) {
      bits = keys(bing);
      for (j=0;j<sizeof(bits);j++)
        map_delete(stat_cache, bits[j]);
    }
} /* zap_stat_cache() */

/*
 * This is most probably the code you wish to call when doing skil
 * checks.  It returns the skill + all its bonsues for stats/whatever.
 *  It first checks to see if the skill is in it's cache. 
 */
varargs int query_skill_bonus(string skill) {
  string *path;
  object guild,
         race;
  int tmp, lvl;

  if (!stringp(skill) || !strlen(skill))
    return 0;
  if (!skills)
    skills = ({ });
  if (skill[0] == '.')
    skill = skill[1..strlen(skill)];
  if (bonus_cache[skill])
    return stat_modify(bonus_cache[skill], skill); 
  path = (string *)SKILL_OB->query_skill_path(skill);
  if (!path)
    return 0;
  if(level_cache[skill])
    lvl = level_cache[skill];
  else
    level_cache[skill] = lvl = calc_level(path);
  guild = (object)this_object()->query_guild_ob();
  race = (object)this_object()->query_race_ob();
  if (race)
    tmp = (int)race->query_skill_bonus(lvl, skill);
  if (guild)
    tmp += (int)guild->query_skill_bonus(lvl, skill);
  return calc_bonus(lvl + tmp, skill);
} /* query_skill_bonus() */

/*
 * This returns jus the skill level.  Used a lot to determine if you
 * can use/teach/whatever a skill.
 * This also uses a cache.
 */ 
mixed query_skill(string skill) {
  string *path;

  if (level_cache[skill])
    return level_cache[skill]; 
  if (!skills)
    skills = ({ });
  if (skill[0] == '.')
    skill = skill[1..strlen(skill)];
  path = (string *)SKILL_OB->query_skill_path(skill);
  if (!path)
    return 0;
  return (level_cache[skill] = calc_level(path));
} /* query_skill() */
 
int add_skill_level(string skill, int lvl, int exp) {
  string *path;
 
  if (!skills)
    skills = STD_SKILLS;
  if (!lvl)
    return 0;
  delta = 0;
  path = (string *)SKILL_OB->query_skill_path(skill);
  if (!path)
    return 0;
  bonus_cache = ([ ]);
  level_cache = ([ ]);
  skills = recursive_skill_add(skills, path, 0, lvl, exp,
                                SKILL_OB->query_skills());
  return 1;
} /* add_skill_level() */
 
int calc_level(string *path) {
  mixed cur;
  int i, j;
  int lvl;
 
  lvl = 0;
  cur = skills;
  for (i=0;i<sizeof(path);i++)
    if ((j=member_array(path[i], cur)) == -1)
      return lvl;
    else {
      lvl = cur[j+SKILL_LVL];
      cur = cur[j+SKILL_BIT];
    }
  return lvl;
} /* calc_level() */

mixed add_to_all(mixed skil, int lvl) {
  int i;

  if (!sizeof(skil))
    return ({ });
  for (i=0;i<sizeof(skil);i+=SKILL_ARR_SIZE) {
    skil[i+SKILL_LVL] += lvl;
    skil[i+SKILL_BIT] = add_to_all(skil[i+SKILL_BIT], lvl);
  }
  return skil;
} /* add_to_all() */
 
mixed recursive_skill_add(mixed skil, string *path, int avr, int lvl, int exp,
                          mixed standard) {
  int i, j, tmp;
  mixed bit;

  if ((j=member_array(path[0], skil)) == -1) {
    j = sizeof(skil);
    skil += ({ path[0], avr, 0, ({ }) });
    for (i=0;i<sizeof(standard);i+=SKILL_ARR_SIZE)
      if (member_array(standard[i], skil) == -1)
        skil += ({ standard[i], avr, 0, ({ }) });
  }
  if ((i=member_array(path[0], standard)) == -1) {
    standard = ({ path[0], 0, 0, ({ }) });
    i = 0;
  }
  path = path[1..sizeof(path)];
  if (!sizeof(path)) {
    skil[j+SKILL_LVL] += lvl;
    skil[j+SKILL_BIT] = add_to_all(skil[j+SKILL_BIT], lvl);
    skil[j+SKILL_EXP] += exp;
    return skil;
  }
  avr = skil[j+SKILL_LVL];
  skil[j+SKILL_BIT] = recursive_skill_add(skil[j+SKILL_BIT], path, avr, lvl,
                               exp, standard[i+SKILL_BIT]);
  bit = skil[j+SKILL_BIT];
  if (sizeof(bit)) {
    for (i=0;i<sizeof(bit);i+=SKILL_ARR_SIZE)
      tmp += bit[i+SKILL_LVL];
    skil[j+SKILL_LVL] = (tmp*SKILL_ARR_SIZE)/sizeof(bit);
    if (tmp%sizeof(bit))
      skil[j+SKILL_LVL] += 1;
  }
  return skil;
} /* recursive_skill_add() */
 
int stat_modify(int lvl, string skill) {
  string stat_bonus;
  int i, stat, bonus;

  if (!stat_cache[skill]) {
    stat_bonus = (string)SKILL_OB->query_skill_stat(skill);
    for (i = strlen(stat_bonus); i--; ) {
      switch(stat_bonus[i]) {
        case 'C' : stat = (int)this_object()->query_con();
                   break;
        case 'S' : stat = (int)this_object()->query_str();
                   break;
        case 'D' : stat = (int)this_object()->query_dex();
                   break;
        case 'W' : stat = (int)this_object()->query_wis();
                   break;
        case 'I' : stat = (int)this_object()->query_int();
                   break;
      }
      bonus += (stat - 13) * 3;
      if (!stat_cache[stat_bonus[i]])
        stat_cache[stat_bonus[i]] = ([ stat_bonus : 1 ]);
      else
        stat_cache[stat_bonus[i]] += ([ stat_bonus : 1 ]);
    }
    stat_cache[stat_bonus] = ({ bonus, stat_bonus });
  } else {
    bonus = stat_cache[skill][0];
    stat_bonus = stat_cache[skill][1];
  }
  if (strlen(stat_bonus))
    return lvl+( (lvl*bonus) / (strlen(stat_bonus) * 60) );
  return lvl;
} /* stat_modify() */

int calc_bonus(int lvl, string skill) {
  int bonus, stat, i;

  if (lvl > 60)
    lvl = 170 + ((lvl-60) >> 1);
  else if (lvl > 40)
    lvl = 150 + (lvl-40);
  else if (lvl > 20)
    lvl = 100 + ( ((lvl-20)*5) >> 1);
  else
    lvl = lvl * 5;
  bonus_cache[skill] = lvl;
  return stat_modify(lvl, skill);
} /* calc_bonus() */

int query_skill_successful(string str, int mod) {
  return (query_skill_bonus(str, 0) + mod >= random(200));
} /* query_skill_successful */

int teach_skill(object *obs, string str) {
  int num, lvl, my_lvl;
  object *ok, *cannot, *too_little, *too_low;
  string skill;
  mixed *bits;
  int i, j, cost, total;

  if (sscanf(str, "%d levels of %s", num, skill) != 2)
    if (sscanf(str, "%d level of %s", num, skill) != 2)
      if (sscanf(str, "%d %s", num, skill) != 2) {
        num = 1;
        skill = str;
      }
  if (num > 50) {
    write("You can only teach a maximun of 50 levels at one time.\n");
    return 1;
  }
/* Make sure its a valid skill */
  bits = explode(implode(explode(skill, " "), "."), ".");
  if (!(skill = (string)SKILL_OB->query_skill(bits)))
    return 0;
/*
 * We don't do the teaching here.  Figure out how much xp it will cost
 * to go up the levels, and inform the person we are teaching...
 */
  my_lvl = query_skill(skill);
  too_low = ok = cannot = too_little = ({ });
  for (i=0;i<sizeof(obs);i++) {
    lvl = (int)obs[i]->query_skill(skill);
    if (lvl+j >= my_lvl && obs[i] != this_object()) {
      cannot += ({ obs[i] });
      continue;
    }
    if ((sizeof(explode(skill, "."))-1)*5 > lvl) {
      too_low += ({ obs[i] });
      continue;
    }
    cost = DEFAULT_COST;
    cost *= (int)SKILL_OB->query_skill_cost(skill);
    cost *= STD_COST/5;
    total = 0;
    for (j=0;j<num;j++) {
      int k;

      if (lvl+j <= my_lvl/2)
        total += cost*(((lvl+j)/LEVEL_DIV)+1);
      else
        total += cost*(((lvl+j)/LEVEL_DIV)*(k=((2*(lvl+j))/(my_lvl))))*k;
    }
    if (total > (int)obs[i]->query_xp()) {
      write("Would have cost "+total+" xp to teach "+num+" levels of "+
            skill+" to "+obs[i]->query_cap_name()+".\n");
      too_little += ({ obs[i] });
      continue;
    }
    if (obs[i] != this_object()) {
      tell_object(obs[i], this_object()->query_cap_name()+" offers to teach "+
                          "you "+num+" level"+(num>1?"s":"")+" of "+skill+
                          " for "+total+" xp.\nUse 'learn' to learn the "+
                          "skill.\n");
      ok += ({ obs[i] });
    } else
      write("You can teach yourself "+num+" level"+(num>1?"s":"")+" of "+
            skill+" for "+total+" xp.\nUse 'learn' to leanr the skill.\n");
    obs[i]->add_teach_offer(this_object(), skill, num, lvl, total);
  }
  if (sizeof(cannot))
    write("You are too low a level to teach "+query_multiple_short(cannot)+
          " "+num+" levels of "+skill+".\n");
  if (sizeof(too_low))
    write(capitalize(query_multiple_short(too_low))+" is not high enough "+
          "level in the outer skills to learn "+num+" levels of "+skill+".\n");
/*
  if (sizeof(too_little))
    write(capitalize(query_multiple_short(too_little))+" does not have "+
          "enough xp to learn "+num+" levels of "+skill+".\n");
 */
  if (sizeof(ok))
    write("You offer teach "+query_multiple_short(ok)+" "+num+" levels of "+
          skill+".\n");
  return 1;
} /* teach_skill() */

int add_teach_offer(object ob, string skill, int num, int lvl, int xp) {
  teach_offer[ob] = ({ skill, num, lvl, xp });
} /* add_teach_offer() */

mapping query_teach_offer() { return teach_offer + ([ ]); }

int do_learn(string str) {
  object *obs, *diff_lvl, *no_xp, *not_offer, *ok;
  int i, lvl;
  string skill, *bits;
  mixed *bing;

  notify_fail("Syntax: learn <skill> from <person>\nIf they have to have "+
              "offered to teach you to use this.\n");
  if (!str)
    return 0;
  if (sscanf(str, "%s from %s", skill, str) != 2)
    return 0;
  obs = find_match(str, environment());
  if (!sizeof(obs)) {
    notify_fail("I am sorry "+str+" is not here.\n");
    return 0;
  }
  bits = explode(implode(explode(skill, " "), "."), ".");
  if (!(skill = (string)SKILL_OB->query_skill(bits))) {
    notify_fail("The skill '"+implode(bits, ".")+"' is invalid.\n");
    return 0;
  }
  ok = not_offer = no_xp = diff_lvl = ({ });
  for (i=0;i<sizeof(obs);i++) {
    if (!(bing = teach_offer[obs[i]])) {
      not_offer += ({ obs[i] });
      continue;
    }
    if (skill != bing[O_SKILL]) {
      not_offer += ({ obs[i] });
      continue;
    }
    if ((int)this_object()->query_xp() < bing[O_XP]) {
      no_xp += ({ obs[i] });
      continue;
    }
    if (query_skill(bing[O_SKILL]) != bing[O_LVL]) {
      diff_lvl += ({ obs[i] });
      continue;
    }
    if (sizeof(ok)) continue;
    if (obs[i] != this_object()) {
      write(obs[i]->query_cap_name()+" starts to teach you "+bing[O_NUM]+
            " levels of "+bing[O_SKILL]+" for "+bing[O_XP]+" xp.\n");
      tell_object(obs[i], "You start to teach "+this_player()->query_cap_name()+
                  " "+bing[O_NUM]+" levels of "+bing[O_SKILL]+" for "+
                  bing[O_XP]+".\n");
      say(obs[i]->query_cap_name()+" starts to teach "+
            this_player()->query_cap_name()+" some skills.\n",
          ({ this_player(), obs[i] }));
      ok += ({ obs[i] });
    } else {
      write("You start to teach yourself "+bing[O_NUM]+" levels in "+
            bing[O_SKILL]+" for "+bing[O_XP]+".\n");
      say(this_player()->query_cap_name()+" starts to teach themselves some "+
          " skills.\n", ({ this_object() }));
    }
/*
 * Ok...  We now change the time on both people and wait...
 * After they have both finished everything is hunky womble.  If one
 * types stop, we dont teach them anything...
 */
/*
    this_object()->adjust_xp(-bing[O_XP]);
    add_skill_level(bing[O_SKILL], bing[O_NUM], bing[O_XP]);
    teach_offer = m_delete(teach_offer, obs[i]);
 */
    this_object()->adjust_time_left(-(bing[O_XP]/TIME_DIV));
    this_object()->set_interupt_command("stop_teaching_skills",
                                        this_object(), bing + ({ obs[i] }));
    if (this_object() != obs[i]) {
      obs[i]->adjust_time_left(-(bing[O_XP]/TIME_DIV));
      obs[i]->set_interupt_command("stop_teaching_skills",
                                   this_object(), bing + ({ obs[i] }));
    }
  }
  if (sizeof(not_offer))
    write(capitalize(query_multiple_short(not_offer))+" is not offering to "+
          "teach you "+skill+" at your current level in it.\n");
  if (sizeof(no_xp))
    write("You do not have enough xp to learn "+skill+" from "+
          query_multiple_short(no_xp)+".\n");
  if (sizeof(diff_lvl))
    write("You are a different level in "+skill+" when "+
          query_multiple_short(diff_lvl)+" offered to teach you.\n");
  if (sizeof(ok))
    say(this_player()->query_cap_name()+" learns some skills from "+
        query_multiple_short(ok)+".\n", ok);
  return 1;
} /* do_learn() */

void stop_teaching_skills(int left, mixed bing) {
  object ob;

  if (left > 0) {
/* Someone did a stop!  Naughty frogs! */
    if (bing[O_OTHER_PER] == this_object()) {
      say(this_object()->query_cap_name()+" stops teaching themselves some "+
          "skills.\n");
      return ;
    }
    if (previous_object() == this_object()) {
      ob = bing[O_OTHER_PER];
      tell_object(ob, this_object()->query_cap_name()+" interupts your "+
                      "training.\n");
    } else {
      ob = this_object();
      tell_object(ob, bing[O_OTHER_PER]->query_cap_name()+" interupts your "+
                      "training.\n");
    }
    say(bing[O_OTHER_PER]->query_cap_name()+" stops teaching some skills to "+
        this_object()->query_cap_name()+".\n",
        ({ this_object(), bing[O_OTHER_PER] }));
    if (ob->query_time_left() < 0) {
/* Give them a 0 time.  and zap the interupt command. */
      ob->adjust_time_left(-((int)ob->query_time_left()));
      ob->set_interupt_command(0);
    }
    return ;
  }
  if (previous_object() != this_object()) {
/* First make sure we dont get the level twice... */
    return ;
  }
/* Ok...  We did it!  Finished! */
  if (this_object() != bing[O_OTHER_PER])
    bing[O_OTHER_PER]->adjust_xp(bing[O_XP]/10);
  this_object()->adjust_xp(-bing[O_XP]);
  add_skill_level(bing[O_SKILL], bing[O_NUM], bing[O_XP]);
  if (this_object() != bing[O_OTHER_PER]) {
    tell_object(this_object(), "You finish learning "+bing[O_NUM]+" levels of "
                +bing[O_SKILL]+" from "+bing[O_OTHER_PER]->query_cap_name()+
                ".\n");
    tell_object(bing[O_OTHER_PER], this_object()->query_cap_name()+" finishes "+
                "learning "+bing[O_NUM]+" levels of "
                +bing[O_SKILL]+" from you.\n");
    say(this_object()->query_cap_name()+" finishes learning some skills "+
        " from "+bing[O_OTHER_PER]->query_cap_name()+".\n",
        ({ this_object(), bing[O_OTHER_PER] }));
  } else {
    tell_object(this_object(), "You finish teaching yourself "+bing[O_NUM]+
                " levels of "+bing[O_SKILL]+".\n");
    say(this_object()->query_cap_name()+" finishes learning some skills "+
        " from themselves.\n", ({ this_object(), bing[O_OTHER_PER] }));
  }
} /* stop_teaching_skills() */
