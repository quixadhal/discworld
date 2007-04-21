inherit "/std/effect_shadow";

int query_skill_bonus(string skill, int true) {
  if ((skill != "other.movement.swimming") || true)
    return player->query_skill_bonus(skill);
  return player->query_skill_bonus(skill)+50;
}
