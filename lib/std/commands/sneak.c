#define SKILL "other.covert.stealth"
#define SNEAK_SKILL 150
/* make it cost 10 guild points. */
#define COST 10
#define TEACH_LEVEL 30
#define LEARN_LEVEL 10

int sneak(string str) {
  int i;
  int sk_bon;
  object ob;
  mixed *dest_dir;

  if (!str) {
    notify_fail("Syntax: sneak <dir>\n");
    return 0;
  }
  if ((int)this_player()->query_gp() < COST) {
    notify_fail("Not enough guild points to sneak.\n");
    return 0;
  }
/* well actually you can 8)*
  if (sizeof((mixed *)this_player()->query_followers())) {
    notify_fail("You cannot sneak with people following you.\n");
    return 0;
  }
*/
  dest_dir = (mixed *)environment(this_player())->query_dest_dir();
  if (!pointerp(dest_dir)) {
    notify_fail("Please contact someone at once.  This room is WRONG!\n");
    return 0;
  }
  if ((i = member_array(str, dest_dir)) == -1 || i%2) {
    notify_fail("Helps if you sneak in a direction that exists.\n");
    return 0;
  }
  this_player()->adjust_gp(-COST);
  sk_bon = (int)this_player()->query_skill_bonus(SKILL);
  notify_fail("You failed to sneak "+dest_dir[i]+".\n");
  if (sk_bon > random(SNEAK_SKILL))
/* they succeeded. */
    i = (int)environment(this_player())->do_exit_command(0,
        ({ dest_dir[i], "none"}) );
  else
    i = (int)environment(this_player())->do_exit_command(0, dest_dir[i]);
  if (!i)
    return 0;
  write("Ok, you sneak "+str+".\n");
  return 1;
}

string help() {
  return 
"Syntax: sneak <dir>\n\n"+
"Lets you sneak in a direction.  This means that you get no exit or enter "+
"messages.  This relies on the skill other.covert.stealth.  Bing!  It will "+
"not inform you of success or failure and costs "+COST+" guild points "+
"regardless of if you succeed or not.\n\n"+
"Example : \n"+
"> sneak west\n"+
"Ok, you sneak west\n"+
"[Description of random room eaten by the hairy room descrtipion eater]\n"+
"\nSee also\n"+
"  hide.\n";
}

int teach(object ob) {
  if (this_player()->query_skill(SKILL) < TEACH_LEVEL) {
    return -1;
  }
  if (ob->query_skill(SKILL) < LEARN_LEVEL) {
    return -2;
  }
  ob->add_known_command("sneak");
  return 1;
}
