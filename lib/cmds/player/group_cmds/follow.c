#include <group_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;

int do_group_follow( object *who, string group, int unfollow );

void create() {
   ::create();
   add_group_sub_command( "follow", "<indirect:living'member(s)'>",
      (: do_group_follow( $1, $6, 0 ) :) );
   add_group_sub_command( "unfollow", "<indirect:living'member(s)'>",
      (: do_group_follow( $1, $6, 1 ) :) );      
   set_membership_required( "follow", 0, 1 );
   set_membership_required( "unfollow", 0, 1 );
} /* create() */


int do_group_follow( object *who, string group, int unfollow ) {
   
   int size;
   string fault_message;
   object *what_not, *follow, *already_following;

   fault_message = "";   
   who -= ({ 0 });
   
   if( member_array( this_player(), who ) != -1 ) {
      who -= ({ this_player() });
      fault_message += "You cannot follow yourself.  ";
   }
   
   follow = ( who & GROUP->members_of( group ) );
   
   what_not = who - follow;
   
   switch( unfollow ) {
      case 1:
         already_following = filter( follow,
            (: member_array( this_player(),
               $1->query_followers() ) == -1 :) );
         if( sizeof( already_following ) ) {
            fault_message += "You are not following $C$" +
               query_multiple_short( already_following,
               "the" ) + ".  ";
            follow -= already_following;
         }
         break;
      case 0:
         already_following = filter( follow,
            (: member_array( this_player(),
               $1->query_followers() ) != -1 :) );
         if( sizeof( already_following ) ) {
            fault_message += "You are already following $C$" +
               query_multiple_short( already_following,
               "the" ) + ".  ";
            follow -= already_following;
         }
         break;
      default:
         printf( "Barf.\n" );
   }
   
   size = sizeof( what_not );
   
   if( size ) {
      fault_message += "$C$" + query_multiple_short( what_not, "the" ) +
         ( size > 1 ? " are not members of your group" :
         " is not a member of your group" ) + ".  ";
   }
   
   fault_message += "\n";
   
   if( sizeof( follow ) ) {
      GROUP->handle_group_follow( group, this_player(), follow,
         unfollow, 0 );
      return 1;
   } else {
      tell_object( this_player(), fault_message );
      return 1;
   }
   
} /* do_group_follow() */


string query_help_string_for( string sub ) {
   switch( sub ) {
      case "follow":
         return "This command lets you follow your fellow "
            "group members.  Using \"all\" will only target "
            "all group members, not all people, in the room.\n";
      case "unfollow":
         return "This command lets you stop following your fellow "
            "group members.  Using \"all\" will only target "
            "all group members, not all people, in the room.\n";
   }

   return 0;
   
} /* query_help_string_for() */

