inherit "/std/races/standard";

void setup() {
  set_long("The Elves are a tall noble race.\n");
  set_name("elf");
  reset_get();
}

void start_player() {
  previous_object()->adjust_bonus_int(3);
  previous_object()->adjust_bonus_dex(3);
  previous_object()->adjust_bonus_str(-3);
  previous_object()->adjust_bonus_wis(-3);
}

int query_skill_bonus(int lvl, string skill) {
  if (skill == "magic.spells")
    return 4;
  if (skill == "other.covert.stealth")
    return 5;
  return 0;
}

string query_desc(object ob) {
  return "A tall noble elf.\n";
}
