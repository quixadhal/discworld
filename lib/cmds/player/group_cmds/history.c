#include <group_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;

int do_channel_history();

void create() {
   ::create();
   add_group_sub_command( "history", "", (: do_channel_history() :) );
   set_membership_required( "history", 0, 1 );
} /* create() */


int do_channel_history() {

   this_player()->display_group_history();

   return 1;

} /* do_channel_history() */


string query_help_string_for( string sub ) {
   return "This command lets you recap what has been said on your "
      "group's channel.";
} /* query_help_string_for() */

