#include "skills.h"
#include "weapon.h"
inherit "/std/guilds/standard";
 
void setup() {
  set_name("fighters");
  set_short("fighters guild");
  set_long("The fighters guild is a guild for fighters, it gives you "+
           "skills that relate to finding out how good a weapon/armour is "+
           "and a sound background in beating people up.\n"+
           "Only join this guild if you wish to beat people into a pulp "+
           "they are not amazingly good at much else.  They do get lower "+
           "skill costs for armour and weapon evaluation skills.\n");
  reset_get();
/*
  add_command("skills", "/std/commands/skills", "skill_list");
*/
  add_command("judge", 0);
  add_command("fix", 0);
  add_command("skills", 1);
  set_start_pos("/d/am/fighters/arena");
}
 
void set_gp(object ob) {
  int gp;
 
  gp = (int)ob->query_skill_bonus("magic.points");
  ob->set_max_gp(gp+50);
}
 
/* this figures out how much the given skill will cost to advance in
 * It returns a multipler.  10 is normal.
 */
int query_skill_cost(string skill) {
  string *bits;
 
  bits = explode(skill, ".");
  if (bits[0] == "fighting")
    return 5;
  if (bits[0] == "magic")
    return 15;
  if (bits[0] == "faith")
    return 15;
  return 10;
}
 
/* returns the maximun level that you are allowed to advance in the
 * given skill.
 */
int query_skill_max_level(string skill) {
  string *bits;
 
  bits = explode(skill, ".");
  if (bits[0] == "fighting")
    return 300; /* basicly infinite */
  if (bits[0] == "magic" || bits[0] == "faith")
    return 5;
  if (sizeof(bits) < 3)
    return 10;
  if (bits[0] == "other" &&
      bits[1] == "evaluating" &&
      (bits[2] == "weapon" || bits[2] == "armour"))
    return 100;
  return 10;
}
 
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
  return (int)ob->query_skill("fighting");
}

string query_title(object player) {
  int lvl;
 
  lvl = query_level(player);
  if (lvl < 5) return "apprentice fighter";
  if (lvl < 10) return "novice fighter";
  if (lvl < 20) return "experienced fighter";
  if (lvl < 30) return "the proficent swordsman";
  if (lvl < 40) return "the excellent swordsman";
  if (lvl < 50) return "the expert swordsman";
  if (lvl < 60) return "the master swordsman";
  return "the weapon master";
}
 
/* cant think of anything I want to do here at the moment...
 */

void set_new_level(mixed *arr) {
  object ob;
  int lvl;

  ob = arr[0];
  lvl = arr[1];
  ob->add_skill_level("fighting", lvl);
  ob->set_no_check(1);
  ob->adjust_str(17);
  ob->adjust_con(15);
  ob->adjust_int(10);
  ob->adjust_wis(10);
  ob->adjust_dex(13);
/*
 * fighters... huh magic? whats that?
 */
  ob->reset_all();
  ob->set_hp(10000);
  ob->set_gp(10000);
}
