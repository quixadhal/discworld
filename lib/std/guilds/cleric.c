#include "skills.h"
#include "weapon.h"
 
inherit "std/guilds/standard";
 
int query_level(object ob) {
  return (int)ob->query_skill("faith");
  }
 
#if 0
int query_skill_bonus(int lvl, string skill) {
  string *bits;
 
  bits = explode(skill, ".");
  if ((string)previous_object()->query_guild_data() == skill)
    return 30 + ::query_skill_bonus(lvl, skill);
  return ::query_skill_bonus(lvl, skill);
  }
#endif
 
/* 
 *  This figures out how much the given skill will cost to advance in
 *  It returns a multipler.  10 is normal.
 */
int query_skill_cost(string skill) {
  string *bits;
 
  bits = explode(skill, ".");
  if (bits[0] == "fighting") return 15;
  if (bits[0] == "magic") return 15;
  if (bits[0] == "faith") return 3;
  return 10;
}
 
/* 
 *  Returns the maximum level that you are allowed to advance in the
 *  given skill.
 */
int query_skill_max_level(string skill) {
  string *bits;
 
  bits = explode(skill, ".");
  if (bits[0] == "faith") return 300; /* basicly infinite */
  if (skill == "fighting.points") return 20;
  if (bits[0] == "magic" || bits[0] == "fighting") return 5;
  return 10;
}
 
string query_title(object player) {
  int lvl;
                                     
  lvl = query_level(player);
  if (lvl > 50) return "the High Priest";
  if (lvl > 40) return "the Patriach";
  if (lvl > 30) return "the Lama";
  if (lvl > 25) return "the Canon";
/*
  if (lvl > 20) return "the";
*/
  if (lvl > 15) return "the Curate";
  if (lvl > 10) return "the Priest";
  if (lvl > 5) return "the Adept";
  return "the Acolyte";
}
 
void setup() {
  set_name("clerics");
  set_short("clerics guild");
  set_long(
    "The priests guild of the temple of small gods is a old and "+
    "powerfull temple.  Residing in the middle of Ankh-morpork as it always "+
    "has gives an austerity and awe few other temples can match.  Joining "+
    "this guild gives you " +
    "skills that relate to healing and turning the undead minions.  " +
    "You will also get a certain amount of help in defending yourself " +
    "in combat as priests are a bunch very susceptible to pain.\n"+
    "They also get at high levels the abilty to give people extra lives "+
    "and at much lower levels refresh the character anew.\n");
  reset_get();
/*
  add_spell("cure light wounds", "std/spells/cleric/cure_lt_wounds",
    "cast_cure_lt_wounds");
  add_spell("cure serious wounds", "std/spells/cleric/cure_sr_wounds",
    "cast_cure_sr_wounds");
  add_spell("cure critical wounds", "std/spells/cleric/cure_cr_wounds",
    "cast_cure_cr_wounds");
  add_spell("heal", "std/spells/cleric/heal", "cast_heal");
  add_spell("raise", "std/spells/cleric/raise", "cast_raise");
  add_spell("extra life", "/std/spells/cleric/extra_life", "cast_extra_life");
  add_spell("restore", "/std/spells/cleric/restore", "cast_restore");
  add_spell("ressurect", "/std/spells/cleric/ressurect", "cast_ressurect");
  add_spell("cure disease", "/std/spells/cleric/cure_disease",
    "cast_cure_disease");
  add_spell("remove curse", "/std/spells/cleric/remove_curse",
    "cast_remove_curse");
 */
/*
  add_command("skills", "std/commands/skills", "skill_list");
*/
  set_start_pos("/d/am/cleric/templeofsg");
}
 
/*
 *  Called if the player leaves this guild
 */
void leaving_guild() {
  remove_spell("raise");
  remove_spell("cure light wounds");
  remove_spell("cure serious wounds");
  remove_spell("cure critical wounds");
  remove_spell("heal");
}
 
/*
 *  Move the guild member to the guild room when they log in.
 */
void start_player(object pl) {
  previous_object()->set_guild_ob("/std/guilds/priest");
  ::start_player(pl);
}
 
void set_new_level(mixed *arr) {
  object ob;
  int level;
 
  ob = arr[0];
  level = arr[1];
  ob->add_skill_lvl("faith", level);
  ob->add_skill_lvl("fighting", level/2);
  ob->adjust_wis(17);
  ob->adjust_int(10);
  ob->adjust_str(13);
  ob->adjust_dex(13);
  ob->adjust_con(13);
  ob->set_hp(10000);
  ob->set_gp(10000);
}
 
void set_gp(object ob) {
  int gp;
 
  gp = (int) ob->query_skill_bonus("faith.points");
  ob->set_max_gp(gp*5 + 50);
}
