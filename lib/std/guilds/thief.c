#include "skills.h"
#include "weapon.h"
inherit "/std/guilds/standard";
 
void setup() {
  set_name("thieves");
  set_short("thieves guild");
  set_long("The thieves guild is a guild for thieves,  it gives you "+
           "skills suitable for relieving other people of things they no "+
           "longer require.\n");
  reset_get();
/*
  add_command("backstab", "/std/commands/backstab", "backstab");
  add_command("assess", "/std/commands/assess", "assess");
 */
  add_command("sneak", 0);
  add_command("hide", 0);
  add_command("palm", 0);
  add_command("slip", 0);
  add_command("ambush", 0);

  set_start_pos("d/am/thief/entry");
} /* setup() */
 
void set_gp(object ob) {
  int gp;
 
  gp = (int)ob->query_skill_bonus("magic.points");
  ob->set_max_gp(gp+50);
} /* set_gp() */
 
/* this figures out how much the given skill will cost to advance in
 * It returns a multipler.  10 is normal.
 */
int query_skill_cost(string skill) {
  string *bits;
 
  switch(skill) {
    case "fighting.combat.melee.sharp" :
    case "fighting.combat.range.thrown" :
      return 6;
    case "dodging.melee" :
    case "fighting.points" :
      return 8;
  }
  bits = explode(skill, ".");
  switch(bits[0]) {
    case "other":
      if(sizeof(bits) == 1)
        return 6;

      switch(bits[1]) {
        case "covert":
          return 3;
        case "evaluating":
          return 5;
        default:
          return 6;
      }
    case "fighting":
      return 8;
    case "magic" :
      return 15;
    case "occult" :
      return 20;
    case "faith" :
      return 15;
    default:
      return 10;
  }
} /* query_skill_cost() */
 
/*
 * returns the maximun level that you are allowed to advance in the
 * given skill.
 */
int query_skill_max_level(string skill) {
  string *bits;
 
  switch(skill) {
    case "fighting.combat":
      return 25;
    case "fighting.combat.melee.sharp" :
    case "fighting.combat.range.thrown" :
    case "fighting.combat.dodging.melee" :
      return 50;
    case "fighting.points" :
      return 50;
/* Who the heck gave thieves good magic ability?
    case "magic.points" :
      return 15;
 */
    default:
  }
  bits = explode(skill, ".");
  switch(bits[0]) {
    case "other":
      if(sizeof(bits) <=1)
        return 30;
      switch(bits[1]) {
        case "covert":
          return 300;
        case "valueing":
          if(sizeof(bits) >=3 && (bits[2] == "gems" || bits[2] == "jewelery"))
            return 300;
          else
            return 25;
        default:
          return 25;
      }
    case "fighting" :
      return 25;
/* Thieves good at magic?  No way ho say.
    case "magic" :
      return 15;
    case "occult":
      return 30;
 */
    case "magic" :
    case "faith" :
    case "occult" :
      return 5;
    default:
      return 10;
  }
} /* query_max_level() */
 
#if 0
int query_skill_bonus(int lvl, string skill) {
  string *bits;
 
  bits = explode(skill, ".");
  if ((string)previous_object()->query_guild_data() == skill)
    return 30+::query_skill_bonus(lvl, skill);
  return ::query_skill_bonus(lvl, skill);
}
#endif
 
int query_level(object ob) {
  int lvl;

  lvl = (int)ob->query_skill("fighting.combat.melee");
/*
  lvl += (int)ob->query_skill("fighting.combat.range");
 */
  lvl += (int)ob->query_skill("other.valueing");
  lvl += (int)ob->query_skill("other.covert");
  lvl /= 3;
  return lvl;
} /* query_level() */

string query_title(object player) {
  int level;

  level = query_level(player);
  if(level > 100) return "the Grand Master Exalted High Thief";
  if(level > 95) return "the Grand Master High Thief";
  if(level > 90) return "the Exalted Grand Master Thief";
  if(level > 85) return "the Grand Master Thief";
  if(level > 80) return "the Exalted High Master Thief";
  if(level > 75) return "the High Master Thief";
  if(level > 70) return "the Exalted High Thief";
  if(level > 65) return "the Expert Catburglar";
  if(level > 60) return "the Good Catburglar";
  if(level > 55) return "the Catburglar";
  if(level > 50) return "the Apprentice Catburglar";
  if(level > 45) return "the Master Thief";
  if(level > 40) return "the Expert Thief";
  if(level > 35) return "the Thief";
  if(level > 30) return "The Tough Mugger";
  if(level > 25) return "The Mugger";
  if(level > 20) return "The Cutthroat";
  if(level > 15) return "The Experienced Pickpocket";
  if(level > 10) return "The Practiced Pickpocket";
  if(level > 5) return "The Pickpocket";
  return "the Novice Pickpocket";
} /* query_title() */
 
void set_new_level(mixed *arr) {
  object ob;
  int lvl;
  string class;

  ob = arr[0];
  lvl = arr[1];
  class = arr[2];
  ob->add_skill_level("other", lvl);
  ob->add_skill_level("fighting", lvl*2/3);
  ob->add_skill_level("thief", lvl*2/3);
  ob->adjust_dex(16);
  ob->adjust_str(15);
  ob->adjust_con(10);
  ob->adjust_int(12);
  ob->adjust_wis(12);
/*
 * fighters... huh magic? whats that?
 */
  ob->set_hp(100000);
  ob->set_gp(100000);
  ob->reset_all();
} /* set_new_level() */

void check_stab(object ob) {
  if (ob && random(4))
    call_out("do_it", 0, ob);
} /* check_stab() */

void do_it(object ob) {
  if (ob)
    do_command("backstab", ob->query_name());
} /* do_it() */
