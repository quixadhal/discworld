#include <group_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;

int do_leader( object member, string group );

void create() {
   ::create();
   add_group_sub_command( "leader", "<indirect:any-living'member'>",
      (: do_leader( $1[ 0 ], $6 ) :) );
   set_membership_required( "leader", 0, 1 );
   set_leadership_required( "leader", 0, 1 );
} /* create() */


int do_leader( object member, string group ) {

   if( member == this_player() ) {
      return notify_fail( "Oh, good thinking, sparky.\n" );
   }
   
   if( member->query_group() != group ) {
      return notify_fail( member->the_short(1) + " is not in your "
         "group.\n" );
   }
   
   GROUP->set_leader( group, member, this_player() );
   
   return 1;

} /* do_leader() */


string query_help_string_for( string sub ) {
   return "This command allows you to relinquish command and "
      "appoint a new leader for the group.  It does not remove "
      "your membership of the group, but you lose all special "
      "privileges granted to leaders.";
} /* query_help_string_for() */

