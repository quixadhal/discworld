object my_player;

int test_add() { return 0; }
void attack() {
}

void adjust_hp() {
}

void set_hp() {
}
 
void do_death() {
/* He als`ready dead...  sheeze.  */
}
 
void setup_shadow(object ob) {
  shadow(ob,1);
  my_player = ob;
}

attack_by(object ob) {
  tell_object(ob, "This person is a ghost... You cannot attack them.\n");
  ob->stop_fight(my_player);
  return 1;
}

attack_ob(object ob) {
  write("Your hand passes right through it! You are just a ghost!\n");
  return 0;
}

string short() {
  if (!my_player)
    return "Death shadow";
  return "Ghost of "+my_player->short();
}

void dest_death_shadow() {
  destruct(this_object());
}

int cast() {
  write("Casting spells when you are dead?\n");
  return 1;
}

int do_shout() {
  write("You are dead, that is not possible.\n");
  return 1;
}

int do_echo() {
  write("You are dead...\n");
  return 1;
}

int do_guild_command() {
  write("You are dead!\n");
  return 1;
}

int do_race_command() {
  write("You are dead!\n");
  return 1;
}

int score()
{
  write("You are just a disembodied spirit. What use has a wispy thing like " +
    "you have for a collection of meaningless numbers?\n");
  return 1;
}
 
void second_life() {
} /* second_life() */
