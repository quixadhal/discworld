inherit "/std/races/standard";

void setup() {
  set_long("The Half Elves are a tall noble race.\n");
  set_name("half elf");
  reset_get();
} /* setup() */

void start_player() {
  previous_object()->adjust_bonus_int(2);
  previous_object()->adjust_bonus_dex(2);
  previous_object()->adjust_bonus_str(-2);
  previous_object()->adjust_bonus_wis(-2);
} /* start_player() */

int query_skill_bonus(int lvl, string skill) {
  if (skill == "magic.spells")
    return 4;
  if (skill == "other.covert.stealth")
    return 5;
  return 0;
} /* query_skill_bonus() */

string query_desc(object ob) {
  return "A tall not quite so noble half elf.\n";
} /* query_desc() */
