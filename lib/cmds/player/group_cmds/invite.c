#include <group_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;

int do_invite( object person, string group );

void create() {
   ::create();
   add_group_sub_command( "invite", "<indirect:living'person'>", (:
      do_invite( $1, $6 ) :) );
   set_membership_required( "invite", 0, 1 );
   set_leadership_required( "invite", 0, 1 );
} /* create() */


int do_invite( object *persons, string group ) {
   
   object person;
   
   persons -= ({ this_player() });
   
   if( !sizeof( persons ) ) {
      return notify_fail( "I would come up with "
         "something witty, but I'm not going to "
         "bother.\n" );
   }
   
   if( sizeof( persons ) > 1 ) {
      return notify_fail( "You can only invite one person "
         "at a time.\n" );
   }
   
   person = persons[ 0 ];
   
   if( sizeof( GROUP->invitations_of( group ) ) >=
      MAX_INVITATIONS_PER_PARTY_AT_ONCE ) {
      return notify_fail( "You can only have a total of " +
         MAX_INVITATIONS_PER_PARTY_AT_ONCE + " invitations pending "
         "at one time.\n" );
   }

   if( GROUP->is_invited( group, person ) ) {
      return notify_fail( person->the_short() + " has "
         "already been invited to your group.\n" );
   }
   
   if( GROUP->is_member( group, person ) ) {
      return notify_fail( person->the_short() + " is "
         "already a member of your group.\n" );
   }
   
   if( !userp( person ) ) {
      return notify_fail( "You cannot invite " + person->the_short() +
         " to your group.\n" );
   }
   
   GROUP->add_invite( group, person );
   
   GROUP->notify_group( group, this_player(), person->query_cap_name() +
      " has been invited by " + this_player()->query_cap_name() + ".  " );
      
   tell_object( person, "You have been invited by " +
      this_player()->a_short() + " to join " +
      this_player()->query_possessive() + " group.  "
      "You have " + INVITE_TIMEOUT + " seconds to join "
      "the group before the invitation is withdrawn.  Use "
      "\"group join " + this_player()->query_name() + "\" "
      "to join.\n" );
   
   if( person->query_group() ) {
      tell_object( person, "%^BOLD%^Note that accepting the invitation "
         "will force you to leave your current group.%^RESET%^\n" );
   }
   
   return 1;
   
} /* do_invite() */


string query_help_string_for( string sub ) {
   return "With this command the leader of a group can invite new "
      "members to the group.  A person who has been invited must then "
      "use the \"join\" command to actually become a member.";
} /* query_help_string_for() */

