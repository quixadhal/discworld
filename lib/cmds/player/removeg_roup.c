/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: removeg_roup.c,v 1.1 2000/06/17 06:48:19 pinkfish Exp $
 * 
 */
inherit "cmds/base";

#define TEAM_HANDLER "/obj/handlers/team"

int do_leave(string group, object member);

mixed cmd(string str) {
  string group;
  object player, *members, member;
  
  notify_fail("Syntax: removegroup <member>\n");

  player = find_player(str);

  if(!(group = TEAM_HANDLER->query_group(this_player())))
    return notify_fail("You are not a member of a group.\n");

  if(TEAM_HANDLER->query_owner(group) != player)
    return notify_fail("Sorry, you are not the leader of " + group + ".\n");
  
  if(member_array(player, TEAM_HANDLER->query_members(group)) == -1)
    return notify_fail("Sorry, " + str + " is not a member of " + group +
                       ".\n");

  members = TEAM_HANDLER->query_members( group );
  if ( !members) {
    TEAM_HANDLER->leave_group(group, this_player());
    TEAM_HANDLER->end_group( group );
    write( "Somehow your group has no members.  "
           "Your group has been ended.  "
           "I hope this is okay.\n" );
    return 1;
  } else if(!(TEAM_HANDLER->query_owner(group))) {
    TEAM_HANDLER->leave_group( group, member );
    TEAM_HANDLER->end_group( group );
    write( "Somehow your group has no leader.  "
           "Your group has been ended.  "
           "I hope this is okay.\n" );
    return 1;
  }
  
  foreach( member in members ) {
    if ( !member )
      continue;
    tell_object(member, player->short()+" is removed from the "
                "group.\n");
  }
  
  do_leave(group, player);
  return 1;
}

int do_leave(string group, object member) {
   object person;
   
   if(!TEAM_HANDLER->leave_group(group, member))
      return notify_fail("Ack, something went wrong.\n");

   if ( !sizeof( (object *)TEAM_HANDLER->query_members( group ) ) )
      return 0;
  
   foreach(person in TEAM_HANDLER->query_members(group)) {
      if(person) {
         member->remove_protector(person);
         person->remove_protector(member);
         member->remove_follower(person);
         person->remove_follower(member);
      }
   }
   return 1;
}
