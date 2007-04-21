#include <group_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;

#define GROUP_CMD_STATUS  ( GROUP_SUB_CMDS_DIR+"status" )

int do_report( string group );

void create() {
   ::create();
   add_group_sub_command( "report", "", (: do_report( $6 ) :) );
   set_membership_required( "report", 0, 1 );
} /* create() */


int do_report( string group ) {
   string message;
   
   message = this_player()->query_cap_name() + " reports that " +
      this_player()->query_pronoun() + " is ";
   message += GROUP_CMD_STATUS->generate_status_message( this_player() );

   GROUP->notify_group( group, this_player(), ({
      "You report:  " + message,
      message
      }) );

   return 1;

} /* do_report() */


string query_help_string_for( string sub_command ) {
   return "This command lets you report your current "
      "condition on the group's channel.";
} /* query_help_string_for() */

