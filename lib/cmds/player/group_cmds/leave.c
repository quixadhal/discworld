#include <group_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;

int do_leave( string group );

void create() {
   ::create();
   add_group_sub_command( "leave", "", (: do_leave( $6 ) :) );
   set_membership_required( "leave", 0, 1 );
} /* create() */


int do_leave( string group ) {
   
   if( !GROUP->remove_member( this_player()->query_group(),
      this_player() ) ) {
      return notify_fail( "You failed to leave the group "
         "for some odd-ball reason.\n" );
   }
   
   return 1;
} /* do_leave() */


string query_help_string_for( string sub ) {
   return "This command allows you to leave your current "
      "group.  If you're the leader, a new one will be "
      "chosen at random from the remaining members.";
} /* query_help_string_for() */

