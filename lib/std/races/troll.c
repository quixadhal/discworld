inherit "/std/races/standard";

void setup() {
  set_long("This has been set up as a monster race.\n");
   set_name("troll");
  reset_get();
} /* setup() */

void start_player() {
  previous_object()->adjust_bonus_con(6);
  previous_object()->adjust_bonus_int(-8);
  previous_object()->adjust_bonus_str(8);
  previous_object()->adjust_bonus_wis(-6);
} /* start_player() */

int query_skill_bonus(int lvl, string skill) {
  if (skill == "fighting.points")
    return 30;
  if (skill == "fighting.combat.melee.unarmed")
    return 30;
  return 0;
} /* query_skill_bonus() */

string query_desc(object ob) {
  return "An ugly looking troll.\n";
} /* query_desc() */
