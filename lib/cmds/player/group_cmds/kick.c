#include <group_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;

int do_kick_member( object member, string group );

void create() {
   ::create();
   add_group_sub_command( "kick", "<indirect:any-living'member'>",
      (: do_kick_member( $1[ 0 ], $6 ) :) );
   set_membership_required( "kick", 0, 1 );
   set_leadership_required( "kick", 0, 1 );
} /* create() */


int do_kick_member( object member, string group ) {

   if( this_player() == member ) {
      this_player()->new_parser(
         "skick " + this_player()->query_name() );
      return 1;
   }
      
   if( !GROUP->is_member( group, member ) ) {
      return notify_fail( member->the_short() + " is not a member "
         "of your group.\n" );
   }
   
   GROUP->notify_group( group, this_player(), ({
      "You kick " + member->query_cap_name() + " from your ranks.",
      this_player()->query_cap_name() + " has kicked " +
      member->the_short() + " from the group." }) );
      
   GROUP->remove_member( group, member );
   
   return 1;

} /* do_kick_member() */


string query_help_string_for( string sub ) {
   return "This command lets you boot out offending members in "
      "your group.";
} /* query_help_string_for() */

