#define MIN_LEVEL 0
#define SP_COST 10
 
mixed raise(string str, int skill, int cast);

string help() {
  return
"This spell raises you from the dead.  It removes one of your lives, unlike "+
"resurect, and turns you back from being a ghost.\n\n"+
"See also:\n"+
"  ressurect, extra life.\n";
}
 
int cast_raise(string str) {
  int   skill;
  mixed ret;
 
  str = (string)this_player()->expand_nickname(str);
  skill = (int)this_player()->query_skill("faith.spells.curing.target");
  if (skill < MIN_LEVEL) {
    notify_fail("You are not powerful enough to raise the dead.\n");
    return 0;
  }
  skill = (int)this_player()->query_skill_bonus("faith.spells.curing.target");
  ret = raise(str, skill, 1);
  if (stringp(ret)) {
    notify_fail(ret);
    return 0;
  }
  return 1;
}
 
mixed raise(string str, int skill, int cast) {     
  mixed ob;

  if (!str || str == "")
    return "You try to raise nothing and fail miserably.\n";
  ob = find_match(str, environment(this_player()));
  if (sizeof(ob))
    ob = ob[0];
  else
    return "Who is " + capitalize(str) + "?\n";
  if (!living(ob))
    return (string)ob->query_cap_name() + " refuses to be brought to life.\n";
  if (!ob->query_property("dead"))
    return (string)ob->query_cap_name() + " is not dead ... yet.\n";
  if (ob->query_property("noregen"))
    return "Death tells you: HOLD ON, I'M NOT FINISHED WITH THEM YET.\n";
  if (cast && (int)this_player()->adjust_gp(-SP_COST) < 0)
    return "Too low on power.\n";
  write("You call upon the gods to restore " + (string)ob->query_cap_name() +
    " from " + (string)ob->query_possessive() + " immaterial state.\n");
  tell_object(ob, (string)this_player()->query_cap_name() +
    " summons the gods for you.\n");
  say((string)this_player()->query_cap_name() + " summons the gods to raise " +
    (string)ob->query_cap_name() + " from the dead.\n", ob);
  ob->remove_ghost();
  return 1;
}
