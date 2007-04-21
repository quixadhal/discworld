#include <group_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;

int do_group_emote( string message, string group );

void create() {
   ::create();
   add_group_sub_command( "emote", "<string'message'>",
      (: do_group_emote( $4[ 0 ], $6 ) :) );
   set_membership_required( "emote", 0, 1 );
} /* create() */

int do_group_emote( string message, string group ) {

   message = replace( message, ({ "%^", "", }) );
      
   GROUP->notify_group( group, this_player(), ({
      "You emote: " + this_player()->query_cap_name() + " " + message,
      this_player()->query_cap_name() + " " + message
      }) );
   
   return 1;
   
} /* emote_to_group() */


string query_help_string_for( string sub ) {
   return "This command allows you to emote on the group "
      "channel.";
} /* query_help_string_for() */

