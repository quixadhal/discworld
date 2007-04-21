#include <group_handler.h>
#include <player_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;

int do_create( string name );

void create() {
   ::create();
   add_group_sub_command( "create", "<string'name'>", (: do_create( $4[ 0 ] ) :) );
} /* create() */


int do_create( string name ) {
   
   string short;

   // Remove any colour codes.
   name = strip_colours(name);

   if( this_player()->query_group() ) {
      return notify_fail( "You are already a member of " +
         this_player()->query_group() + ".\n" );
   }
   
   if( GROUP->is_group( name ) ) {
      return notify_fail( "Such a group already exists.\n" );
   }

   short = name;
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
   
   if( !GROUP->create_group( name, this_player() ) ) {
      return notify_fail( "You failed to create the group.\n" );
   }
   
   GROUP->set_group_short( name, short );
   
   this_player()->add_succeeded_mess( this_object(), "$N begin$s "
      "an adventuring group.\n" );
         
   GROUP->add_member( name, this_player() );
   GROUP->set_leader( name, this_player() );

   return 1;
   
} /* do_create() */

string query_help_string_for( string sub_command ) {
   if( sub_command != "create" ) {
      return 0;
   }
   return "This sub-command creates a new group and sets you as the "
      "leader of it.  You create it by typing \"group create <name>\" "
      "with <name> being the name of your group.";
} /* query_help_string() */

