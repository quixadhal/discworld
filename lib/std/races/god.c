inherit "/std/races/standard";

string *inedible;

void setup() {
  set_long("The gods are an ancient and powerful race.\n");
  set_name("god");
  reset_get();
}

void start_player() {
  previous_object()->adjust_bonus_int(97);
  previous_object()->adjust_bonus_dex(97);
  previous_object()->adjust_bonus_str(97);
  previous_object()->adjust_bonus_wis(97);
  previous_object()->adjust_bonus_con(97);
  inedible = ({ "skull", "teeth" });
}

int query_skill_bonus(int lvl, string skill) {
  if (skill == "magic.spells")
    return 250;
  if (skill == "faith")
    return 500;
  if (skill == "magic.points")
    return 10000;
  if (skill == "fighting.points")
    return 1000;
  if (skill == "fighting.combat.melee")
    return 1000;
  if (skill == "fighting.combat.melee.unarmed")
    return 250;
  return 200;
}

string query_desc(object ob) {
  if ((int)ob->query_gender() == 1)
    return "You stand in awe at the sight of a god.\n";
  return "You are stunned by the unearthly beauty of a goddess.\n";
}

int query_eat(string type) {
  return (member_array(type, inedible) == -1); 
}
