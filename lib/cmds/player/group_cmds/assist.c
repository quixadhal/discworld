#include <group_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;

int do_assist( object member, string group );
int do_unassist( object member, string group );
int do_assist_list(string group);

void create() {

   ::create();

   add_group_sub_command( "assist", "<indirect:living'member'>",
      (: do_assist( $1[ 0 ], $6 ) :) );
   add_group_sub_command( "unassist", "<indirect:distant-living'member'>",
      (: do_unassist( $1[ 0 ], $6 ) :) );
   add_group_sub_command( "assist", "list",
      (: do_assist_list( $6 ) :) );

   set_membership_required( "assist", 0, 1 );

} /* create() */


int do_assist_list( string group ) {
   object *assisting;
   
   assisting = this_player()->query_assisting();

   if( sizeof( assisting ) )
   {
      tell_object( this_player(), "You are currently "
         "assisting " + query_multiple_short( assisting ) +
         ".\n" );
      return 1;
   }
   add_failed_mess("You are not assisting anyone.\n");
   return 0;
} /* do_assist_list() */

int do_assist( object member, string group ) {
   object *assisting;
   
   assisting = this_player()->query_assisting();

   if( member == this_player() ) {
      return notify_fail( "Don't be stupid.\n" );
   }
   
   if( !GROUP->is_member( group, member ) ) {
      add_failed_mess( member->the_short() + " is not a "
         "member of your group.\n" );
      return 0;
   }
   
   if( member->query_assisters() &&
      member_array( this_player(), member->query_assisters() ) != -1 ) {
      add_failed_mess( "You are already assisting " + member->the_short() +
         ".\n" );
      return 0;
   }
   
   if( sizeof( assisting ) >= MAX_PEOPLE_YOU_CAN_ASSIST ) {
      add_failed_mess( "You can not assist any more people.\n" );
      return 0;
   }

   add_succeeded_mess( "$N begin$s keeping a close eye on $I.\n",
                       ({ member }) );
   
   member->add_assister( this_player() );
   
   return 1;
   
} /* do_assist() */

int do_unassist( object member, string group ) {
   if( member == this_player() ) {
      return notify_fail( "Don't be stupid.\n" );
   }
   
   if( member->query_assisters() &&
      member_array( this_player(), member->query_assisters() ) != -1 ) {
      member->remove_assister( this_player() );
      add_succeeded_mess( ({ "You stop assisting " + member->the_short() +
         ".\n", "" }) );
      return 1;
   }

   add_failed_mess("You are not assisting $I.\n", ({ member }));
   
   return 0;
} /* do_unassist() */


string query_help_string_for( string sub ) {
   return "With this command you can guard a group member.  If the "
      "person you're guarding gets attacked, you will automatically "
      "join the fight after a delay.  The length of the delay depends "
      "on how many people you're guarding at the moment.  Use "
      "\"list\" to see who you are currently assisting.  To stop "
      "assisting someone, use \"group assist <someone>\" again "
      "on that person.\n";
} /* query_help_string_for() */

