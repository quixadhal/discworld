#include <group_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;


int do_join( object member );


void create() {
   ::create();
   add_group_sub_command( "join", "<indirect:living'existing member'>",
      (: do_join( $1[ 0 ] ) :) );
} /* create() */


int do_join( object member ) {

   string his_group, my_group;
   
   his_group = member->query_group();
   
   if( this_player() == member ) {
      return notify_fail( "I could come up with something witty, "
         "but I'm not going to bother.\n" );
   }
   
   if( !his_group ) {
      return notify_fail( member->the_short() + " is not a member "
         "of any group.\n" );
   }
   
   if( !GROUP->is_invited( his_group, this_player() ) ) {
      return notify_fail( "You have not been invited to that "
         "group.\n" );
   }
   
   if( sizeof( GROUP->members_of( his_group ) ) >= MAX_PEOPLE_PER_PARTY ) {
      return notify_fail( "The group you are trying to join is "
         "full.\n" );
   }

   my_group = this_player()->query_group();
   
   if( my_group && ( my_group == his_group ) ) {
      return notify_fail( "You are already a member of "
         "that group.\n" );
   }
   
   if( my_group ) {
      if( !GROUP->remove_member( my_group, this_player() ) ) {
         return notify_fail( "You fail to leave your previous "
            "group.. for some reason.\n" );
      }
      tell_object( this_player(), "%^BOLD%^You leave your current "
         "group behind to join a new one.%^RESET%^\n" );
   }

   if( !GROUP->add_member( his_group, this_player() ) ) {
      return notify_fail( "You failed to join the group "
         "for some weird reason.\n" );
   }

   return 1;
} /* do_join() */


string query_help_string_for( string sub_command ) {
   switch( sub_command ) {
      case "join":
         return "This command lets you join an existing group.  To join, "
            "type \"group join <existing member>\" where "
            "<existing member> is someone who is already a member of the "
            "group you wish to be a part of.  You must be invited to a "
            "group by its leader before you can join.";
      default:
         return 0;
   }
} /* query_help_string_for() */

            
            
