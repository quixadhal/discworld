int teach(object ob) {
  if (this_player()->query_skill(SKILL) < TEACH_LEVEL) {
    return -1;
  }
  if (ob->query_skill(SKILL) < LEARN_LEVEL) {
    return -2;
  }
  ob->add_known_command("palm");
  return 1;
}
