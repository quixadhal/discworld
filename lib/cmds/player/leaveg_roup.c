/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: leaveg_roup.c,v 1.1 1998/01/06 05:29:43 ceres Exp $
 * $Log: leaveg_roup.c,v $
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
/* command, trial out by Turrican for a commands daemon. */

inherit "cmds/base";

#define TEAM_HANDLER "/obj/handlers/team"

int do_leave(string group, object member);

mixed cmd(string str) {
   object member, *members;
   string group;
   int disband;
   
   group = TEAM_HANDLER->query_group(this_player());
   if(!group)
      return notify_fail("You are not a member of any group.\n");

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

   if((object)TEAM_HANDLER->query_owner(group) == this_player())
      disband = 1;
  
   foreach( member in members ) {
      if ( !member || ( member == this_player() ) )
         continue;
      if ( disband ) {
         tell_object(member, this_player()->short()+" leaves the group and "
                     "it disbands, everyone going their separate ways.\n");
         do_leave(group, member);
      } else
         tell_object(member, this_player()->short()+" leaves the group.\n");
   }
   if(disband) {
      TEAM_HANDLER->end_group(group);
      write("You disband "+group+" and everyone goes their separate "
            "ways.\n");
   } else {
      do_leave(group, this_player());
      write("You leave "+group+".\n");
   }
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
