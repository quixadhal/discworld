#define SKILL "other.covert.hiding"
#define HIDE_SKILL 150
#define COST 10
#define SHADOW "/std/commands/shadows/hide"
#define TEACH_LEVEL 20
#define LEARN_LEVEL 6

int hide_in_shadows(string str) {
  int sk_bon;
  object ob;

  if ((int)this_player()->query_gp() < COST) {
    notify_fail("Not enough guild points to hide.\n");
    return 0;
  }
  this_player()->adjust_gp(-COST);
  sk_bon = (int)this_player()->query_skill_bonus(SKILL);
  sk_bon += (int)environment(this_player())->query_property("hide bonus");
  if (sk_bon > random(HIDE_SKILL) && !this_player()->query_hide_shadow()) {
/* they succeeded. */
    ob = clone_object(SHADOW);
    ob->setup_shadow(this_player());
/* no such fn. as query_mmsgout()...sheesh
    tell_room(environment(this_player()),
              this_player()->query_cap_name()+" "+
              this_player()->query_mmsgout()+".\n");
*/
  } else
    say(this_player()->query_cap_name()+" skulks about in the shadows.\n");
  write("Ok, you hide in the shadows.\n");
  return 1;
}

string help() {
  return
"Syntax: hide\n\n"+
"Allows you to hide in shadows.  This allows you to have simple "+
"invisibility.  It relies on the skill " + SKILL + ", it costs "+
COST+" guild points regardless of if it succeeds or fails.\n\n"+
"Example:\n"+
"> hide\n"+
"Ok, you hide in shadows.\n\n"+
"See also\n"+
"  sneak.\n";
}

int teach(object ob) {
  if (this_player()->query_skill(SKILL) < TEACH_LEVEL) {
    return -1;
  }
  if (ob->query_skill(SKILL) < LEARN_LEVEL) {
    return -2;
  }
  ob->add_known_command("hide");
  return 1;
}
