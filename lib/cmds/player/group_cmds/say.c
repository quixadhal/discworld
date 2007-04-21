#include <group_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;

int do_group_say( string message, string group );


void create() {
   ::create();
   add_group_sub_command( "say", "<string'message'>",
      (: do_group_say( $4[ 0 ], $6 ) :) );
   set_membership_required( "say", 0, 1 );
} /* create() */


int do_group_say( string message, string group ) {

   message = replace( message, ({ "%^", "" }) );
   
   GROUP->notify_group( group, this_player(),
      this_player()->query_cap_name() + ": " + message );
      
   return 1;
   
} /* say_to_group() */


string query_help_string_for( string sub ) {
   return "This command allows you to say something on "
      "the group's channel.";
} /* query_help_string_for() */

