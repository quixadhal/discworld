#include <group_handler.h>
#include <player_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;

int do_rename_group( object member, string group );

void create() {
   ::create();
   add_group_sub_command( "rename", "<string'new name'>",
      (: do_rename_group( $4[ 0 ], $6 ) :) );
   set_membership_required( "rename", 0, 1 );
   set_leadership_required( "rename", 0, 1 );
} /* create() */


int do_rename_group( string new_group, string group ) {
   string short;
   string name;

   if( GROUP->is_group( new_group ) ) {
      return notify_fail( new_group + " already exists.\n");
   }

   short = new_group;
   name = GROUP->short_to_name( short );

   if(!sizeof( name ) || !PLAYER_HANDLER->test_valid(name)) {
      return notify_fail( "That is an invalid name for a group.\n" );
   }

   if( sizeof( name ) > MAX_GROUP_NAME_SIZE ) {
      return notify_fail( "That is too long a name for a "
         "group.\n" );
   }

   if( sscanf( name, "%*s%%^^%*s" ) == 2 )
   {
      return notify_fail( "Colour tags are not allowed in group "
         "names.\n" );
   }
   
   if (!GROUP->rename_group( group, name)) {
      add_failed_mess("Not allowed to rename the group.\n");
      return 0;
   }
   
   return 1;

} /* do_kick_member() */


string query_help_string_for( string sub ) {
   return "This command lets you rename a group to something more "
          "exciting.";
} /* query_help_string_for() */

