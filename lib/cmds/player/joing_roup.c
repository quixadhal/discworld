/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: joing_roup.c,v 1.1 1998/01/06 05:29:43 ceres Exp $
 * $Log: joing_roup.c,v $
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
/* command, trial out by Turrican for a commands daemon. */

inherit "cmds/base";

#define TEAM_HANDLER "/obj/handlers/team"

mixed cmd(string str) {
  object owner, person;
  int policy;
  
  if(!str)
    return notify_fail("Syntax: joingroup <group name>\n");
  
  if(TEAM_HANDLER->query_group(this_player()))
    return notify_fail("You are already a member of a group.\n");

  if(!TEAM_HANDLER->test_group(str))
    return notify_fail("Sorry, there is no group by that name.\n");

  owner = TEAM_HANDLER->query_owner(str);
  if(!owner || environment(owner) != environment(this_player()))
    return notify_fail("Sorry, the owner of "+str+" is not here.\n");

  if(TEAM_HANDLER->query_full(str))
    return notify_fail("Sorry, that group is full, you cannot join.\n");

  if(TEAM_HANDLER->query_locked(str))
    return notify_fail("Sorry, that group is locked, you cannot join.\n");
  
  policy = TEAM_HANDLER->query_policy(str);
  
  if(!TEAM_HANDLER->join_group(str, this_player()))
    return notify_fail("Ack, something went wrong.\n");

  foreach(person in TEAM_HANDLER->query_members(str)) {
    if(person && present(person, environment(this_player())) &&
       person != this_player()) {
      this_player()->add_protector(person);
      person->add_protector(this_player());

      if(policy == 1)                 // follow all
  this_player()->add_follower(person);

      if((policy == 1) || (person == owner)) // only follow leader
  person->add_follower(this_player());
    }
  }
  write("You join "+str+" and will now protect and be protected by "
  "them.\n");
  say(this_player()->short()+" joins "+str+" and will protect and be "
      "protected by its members.\n");
  
  write("You are now a member of "+str+"\n");
  return 1;
}
