#define MIN_LEVEL 70
#define SP_COST 100
 
mixed raise(string str, int skill, int cast);

string help() {
  return
"This spell ressurects someone after they have died.  It does not remove "+
"one of your lives, unlike "+
"raise, and turns you back from being a ghost.\n\n"+
"See also:\n"+
"  raise, extra life.\n";
}

mixed ressurect(string str, int skill, int cast);
 
int cast_ressurect(string str) {
  int   skill;
  mixed ret;
 
  if (this_player()->query_property("killed harry")) {
    write("Your god is still smarting because you killed his high priest "+
          "he has a long memory.\n");
    return 1;
  }
  str = (string)this_player()->expand_nickname(str);
  skill = (int)this_player()->query_skill("faith.spells.curing.target");
  if (skill < MIN_LEVEL) {
    notify_fail("You are not powerful enough to ressurect the dead.\n");
    return 0;
  }
  skill = (int)this_player()->query_skill_bonus("faith.spells.curing.target");
  ret = ressurect(str, skill, 1);
  if (stringp(ret)) {
    notify_fail(ret);
    return 0;
  }
  return 1;
}
 
mixed ressurect(string str, int skill, int cast) {     
  mixed ob;

  if (!str || str == "")
    return "You try to ressurect nothing and fail miserably.\n";
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
  write("You call upon the gods to ressurect "+(string)ob->query_cap_name()+
    " from " + (string)ob->query_possessive() + " immaterial state by "+
    "hitting "+ob->query_pronoun()+" with a lightning bolt..\n");
  tell_object(ob, (string)this_player()->query_cap_name() +
    " summons the gods for you, and incidently hits you with a lightning "+
    "bolt.  It doesn't seem to hurt.\n");
  say((string)this_player()->query_cap_name() + " stares intently into space, "+
    "you hear a vage rumbling in the distance, suddenly a lightning bolt "+
    "sears through the sky and hits "+ob->query_cap_name()+" ressurecting "+
    " them from the dead.\n", ob);
  ob->remove_ghost();
  ob->adjust_max_deaths(1);
  return 1;
}
