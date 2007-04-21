#include <command.h>
#include <group_handler.h>

#define CMD_SHIELDS ( DIR_PLAYER_CMDS "/shields" )

inherit GROUP_SUBCOMMAND_INHERITABLE;

int do_shields( object *members, string group );

void create()
{
   ::create();
   add_group_sub_command( "shields", "", (: do_shields( 0, $6 ) :) );
   add_group_sub_command( "shields", "<indirect:any-living'member'>",
      (: do_shields( $1, $6 ) :) );
   set_membership_required( "shields", 0, 1 );
} /* create() */


int do_shields( object *suggested_members, string group )
{
   object cmd_shields, member, *members, *check_for;
   string shields, mess, colour;

   cmd_shields = load_object( CMD_SHIELDS );
   
   if( !cmd_shields )
   {
      return notify_fail( "Cannot find the \"shield\" command.  "
         "Please bug report this group command.\n" );
   }
   
   check_for = members = GROUP->members_of( group );
   
   if( sizeof( suggested_members ) )
   {
      check_for = ( suggested_members & members );
   }
   
   if( !sizeof( check_for ) )
   {
      return notify_fail( "$C$" + query_multiple_short( suggested_members ) +
         ( sizeof( suggested_members ) > 1 ? " are not members " : " is not "
         "a member " ) + "of your group.\n" );
   }
   
   mess = "";
   colour = this_player()->colour_event( GROUP_COLOUR_EVENT, "" );
   
   foreach( member in check_for )
   {
		 if(!member)
			 continue;
		 
		 shields = cmd_shields->construct_shield_message( member, 1 );
		 
		 if( sizeof( shields ) ) {
         mess += "Arcane protection for " + colour +
					 member->query_cap_name() + "%^RESET%^:-\n";
         mess += shields + "\n";
		 } else {
			 mess += colour + member->query_cap_name() +
				 "%^RESET%^ has no arcane protection.\n\n";
		 }
   }
   
   if( sizeof( mess ) > 2 && mess[ sizeof( mess ) - 1 ] == '\n' )
   {
      mess = mess[ 0..( sizeof( mess ) - 2 ) ];
   }
   
   tell_object( this_player(), mess );
   
   return 1;
   
} /* do_shields() */


string query_help_string_for( string what )
{
   return "The \"group shields\" command allows you to view "
      "the arcane protection of all or some of your group's "
      "members.  You can either specify no arguments, or "
      "you can include the names of only those members "
      "whose protection you wish to check.\n";
} /* query_help_string_for() */

