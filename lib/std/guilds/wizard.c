#include "skills.h"
inherit "/std/guilds/standard";
 
void setup() {
  set_name("wizards");
  set_short("Wizards' guild");
  set_long(
    "The wizards' guild is for the more subtle people of the world, "+
    "we achieve dominance (without fail) by the use of quickness of "+
    "thought and the judicious use of the miriad of spells at our "+
    "disposal.  And, unlike the uncouth fighters, if the situation "+
    "demands violence, we don't endanger ourselves in barbaric melee "+
    "combat, we blow the bastards up.\n"
  );
  reset_get();
/*
  add_command("skills", "/std/commands/skills", "skill_list");
  add_command("sk", "/std/commands/skills.new", "skill_list");
*/
  add_command("skills", 1);
  add_spell("missile", "/std/spells/missile", "cast_missile", 0);
  set_start_pos("/d/am/uu/uni/guild_room");
}
 

int query_level(object ob) {
  return (int)ob->query_skill("magic");
}

void set_gp(object ob) {
  int gp;
 
  gp = (int)ob->query_skill_bonus("magic.points");
  ob->set_max_gp(gp+50);
}
 
/*
 * This figures out how much the given skill will cost to advance in
 * It returns a multipler.  10 is normal.
 */
int query_skill_cost(string skill) {
  string *bits;
 
  bits = explode(skill, ".");
  if (bits[0] == "fighting")
    return 15;
  if (bits[0] == "magic")
    return 3;
  if (bits[0] == "faith")
    return 15;
  return 10;
}
 
/*
 * Returns the maximun level that you are allowed to advance in the
 * given skill.
 */
int query_skill_max_level(string skill) {
  string *bits;
 
  bits = explode(skill, ".");
  if (bits[0] == "magic")
    return 300; /* basically infinite */
  if (bits[0] == "faith" || bits[0] == "occult")
    return 5;
  if (bits[0] == "fighting")
    return 25;
  return 10;
}
 
#if 0
int query_skill_bonus(int lvl, string skill) {
  string *bits;
 
  bits = explode(skill, ".");
  if ((string)this_player()->query_guild_data() == skill)
    return 30+::query_skill_bonus(lvl, skill);
  return ::query_skill_bonus(lvl, skill);
}
#endif

#define UNI "the Unseen University "
string query_title(object player) {
  int lvl;
  string depart;
 
  lvl = query_level(player);
  depart = (string)player->query_property("UU department");
  if(!depart) depart = "Practical Necromancy";
  switch(lvl) {
    case 0:      return UNI + "Applicant";
    case 1..5:   return UNI + "Freshman";
    case 6..10:  return UNI + "Sophomore";
    case 11..15: return UNI + "Junior";
    case 16..20: return UNI + "Senior";
    case 21..25: return UNI + "Graduate";
    case 26..30: return "the Post-Graduate in" + depart;
    case 31..35: return "the Lecturer in " + depart;
    case 36..40: return "the Senior Lecturer in " + depart;
    case 41..45: return "the Chair of " + depart;
    case 46..300: return "the Dean of " + depart;
    case 301..10000: return "the Archchancellor";
    default: return "the Wizard";
  }
}
 
void set_new_level(mixed *arr) {
  object ob;
  int lvl;

  ob = arr[0];
  lvl = arr[1];
  ob->add_skill_level("fighting", lvl);
  ob->add_skill_level("magic", lvl*2);
  ob->adjust_int(21);
  ob->adjust_dex(17);
  ob->set_con(15);
  ob->set_wis(13);
  ob->set_str(5);
  ob->set_hp(10000);
  ob->set_gp(10000);
}

int query_see_octarine()
{
  return 1;
}
