#include "skills.h"
#include "weapon.h"
 
inherit "std/guilds/standard";
 
int query_level(object ob) {
  return (int)ob->query_skill("faith");
} /* query_level() */
 
#if 0
int query_skill_bonus(int lvl, string skill) {
  string *bits;
 
  bits = explode(skill, ".");
  if ((string)previous_object()->query_guild_data() == skill)
    return 30 + ::query_skill_bonus(lvl, skill);
  return ::query_skill_bonus(lvl, skill);
} /* query_skill_bonus() */
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
} /* query_skill_cost() */
 
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
} /* query_skill_max_level() */
 
string query_title(object player) {
  int lvl, fem;
                                     
  fem = (int) player->query_gender() == 2;
/* [als] added female names on 20/10/92.  Some of them are strange
   but the masculine version just wasn't appropriate.  Bing on. */
  lvl = query_level(player);
  if (lvl > 50) return (fem) ? "the High Priestess" : "the High Priest";
  if (lvl > 40) return (fem) ? "the Matriarch" : "the Patriarch";
  if (lvl > 30) return "the Lama";
  if (lvl > 25) return (fem) ? "the Canoness" : "the Canon";
/*
  if (lvl > 20) return "the";
*/
  if (lvl > 15) return "the Curate";
  if (lvl > 10) return (fem) ? "the Priestess" : "the Priest";
  if (lvl > 5) return "the Adept";
  return "the Acolyte";
} /* query_title() */
 
void setup() {
  set_name("priest");
  set_short("priests guild");
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
  add_spell("cure light wounds", "std/spells/cleric/cure_lt_wounds",
    "cast_cure_lt_wounds", 0);
  add_spell("cure serious wounds", "std/spells/cleric/cure_sr_wounds",
    "cast_cure_sr_wounds", 0);
  add_spell("cure critical wounds", "std/spells/cleric/cure_cr_wounds",
    "cast_cure_cr_wounds", 0);
  add_spell("heal", "std/spells/cleric/heal", "cast_heal", 0);
  add_spell("calm", "std/spells/cleric/calm", "cast_calm", 0);
  add_spell("cure disease", "std/spells/cleric/cure_disease", "cast_cure_disease", 0);
  add_spell("remove curse", "std/spells/cleric/remove_curse", "cast_remove_curse", 0);
  add_spell("restore", "std/spells/cleric/restore", "cast_restore", 0);
  add_spell("ressurect", "std/spells/cleric/ressurect", "cast_ressurect", 0);
  add_spell("raise", "std/spells/cleric/raise", "cast_raise", 0);
  add_spell("extra life", "/std/spells/cleric/extra_life", "cast_extra_life", 0);
/*
  add_command("skills", "std/commands/skills", "skill_list");
*/
  set_start_pos("d/am/priest/templeofsg");
} /* setup() */
 
/*
 *  Called if the player leaves this guild
 */
void leaving_guild() {
} /* leaving_guild() */
 
void set_new_level(mixed *arr) {
  object ob;
  int level;
 
  ob = arr[0];
  level = arr[1];
  ob->add_skill_level("faith", level);
  ob->add_skill_level("fighting", level/2);
  ob->adjust_wis(17);
  ob->adjust_int(10);
  ob->adjust_str(13);
  ob->adjust_dex(13);
  ob->adjust_con(13);
  ob->add_triggered_action("bing", "spell_effects", "/std/guilds/priest",
                           "check_spell");
  ob->reset_all();
} /* set_new_level() */
 
void set_gp(object ob) {
  int gp;
 
  gp = (int) ob->query_skill_bonus("faith.points");
  ob->set_max_gp(gp + 50);
} /* set_gp() */

int check_spell(object ob) {
  if (random(3))
    call_out("do_check_spell", 0, ob);
} /* check_spell() */

void do_check_spell(object ob) {
  int lvl;
  int diff, cur_hp;

  lvl = (int)this_player()->query_my_att_level();
  diff = (int)this_player()->query_max_hp() -
         (cur_hp = (int)this_player()->query_hp());
  if (diff < 100) /* almost not damaged! */
    return ;
  if (lvl > 100) {
    if (cur_hp < 500)
      cast_spell("heal", this_player()->query_name());
    return ;
  }
  if (lvl > 20) {
    if (diff > 500)
      cast_spell("cure critical wounds", this_player()->query_name());
    return ;
  }
  if (lvl > 10) {
    if (diff > 150)
      cast_spell("cure serious wounds", this_player()->query_name());
    return ;
  }
  cast_spell("cure light wounds", this_player()->query_name());
} /* do_check_spell() */
