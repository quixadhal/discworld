inherit "/std/races/standard";

void setup() {
  set_long("Half-orcs are strong but have all charisma of their orc " +
           "relatives.\n");
  set_name("half-orc");
  reset_get();
}

void start_player() {
  previous_object()->adjust_bonus_int(-4);
  previous_object()->adjust_bonus_str(4);
  previous_object()->adjust_bonus_wis(-2);
  previous_object()->adjust_bonus_con(2);
}

int query_skill_bonus(int lvl, string skill) {
  if (skill == "magic.spells")
    return -10;
  if (skill == "fighting.melee")
    return 20;
  if (skill == "fighting.melee.unarmed")
    return 20;
  return 0;
}

string query_desc(object ob) {
  return "A scruffy-looking half-orc varlet.\n";
}
