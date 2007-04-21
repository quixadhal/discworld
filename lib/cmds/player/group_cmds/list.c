#include <group_handler.h>

inherit GROUP_SUBCOMMAND_INHERITABLE;

string *leader_adjectives =
   ({
      "daring",
      "heroic",
      "brave",
      "bold",
      "courageous",
      "dauntless",
      "fearless",
      "intrepid",
      "valiant",
      "awesome",
      "stalwart"
   });

string *follower_messages =
   ({
      " and also includes",
      " and features such prominent figures as",
      ", whose supporters consist of",
      " and features such renowned names as",
   });


int do_list_groups(string person);

void create() {
   ::create();
   add_group_sub_command( "list", "", (: do_list_groups(0) :) );
   add_group_sub_command( "list", "[with] <string'player'>",
                           (: do_list_groups($4[0]) :) );
} /* create() */


int do_list_groups(string person) {

   string group, *groups, prefix, suffix, message, adjective, colour, stats;
   object *members, leader;
   int num_groups, num_members;
   object person_ob;
   
   groups = sort_array( GROUP->query_groups(), 
       (: strcmp( lower_case(GROUP->query_group_short( $1 )), 
            lower_case(GROUP->query_group_short( $2 )) ) :) );

   num_groups = sizeof( groups );

   if (person) {
      person = this_player()->expand_nickname(person);
      person_ob = find_player(person);
      if (!person_ob) {
         add_failed_mess("Sorry, " + person + " is not logged on.\n");
         return 0;
      }
   }
   
   if( !num_groups )
   {
      return notify_fail( "There are currently no active groups.\n" );
   }
         
   prefix = "$I$+8,+0=  + ";
   suffix = "$I$-8,-0=\n";
   
   colour = this_player()->colour_event( GROUP_COLOUR_EVENT, "" );

   message = "";
   num_members = 0;
   num_groups = 0;
   
   foreach( group in groups )
   {
      leader = GROUP->leader_of( group );
      members = GROUP->members_of( group );
      
      if( !leader || !sizeof( members ) )
      {
         continue;
      }
      

      if (!person_ob || person_ob == leader ||
          member_array(person_ob, members) != -1) {

         num_members += sizeof( members );
         message += prefix;
      
         message += colour + GROUP->query_group_short( group ) +
            "%^RESET%^ is led by ";
      
         members -= ({ leader });
      
         adjective = leader_adjectives[ random( sizeof(
                                                       leader_adjectives ) ) ];
      
         message += "the " + adjective + " %^BOLD%^" +
            leader->query_short(1) + "%^RESET%^";
      
               
         if( sizeof( members ) )
         {
//            message += follower_messages[ random( sizeof( follower_messages )
//               ) ] + " %^BOLD%^" + query_multiple_short( members, "the", 1, 1, 1 ) +
//               "%^RESET%^.";
            message += follower_messages[ random( sizeof( follower_messages )
               ) ] + " %^BOLD%^" + query_multiple_short( members->short(1) ) +
               "%^RESET%^.";

         }
         else
         {
            message += " and has no other members.";
         }

         message += suffix;
         num_groups++;
      }
   }

   switch( num_groups )
   {
      case 0:
         if (!person) {
            stats = "There are currently no groups, ";
         } else {
            add_failed_mess("There are no groups with " + person +
                            " in them.\n");
            return 0;
         }
         break;
      case 1:
         stats = "There is currently %^BOLD%^one%^RESET%^ active group, ";
         break;
      default:
         stats = "There are currently %^BOLD%^" + query_num( num_groups ) +
            "%^RESET%^ active groups, ";
         break;
   }
   
   stats += "with a total number of %^BOLD%^" +
      query_num( num_members ) + "%^RESET%^ persons:\n";

   tell_object( this_player(), stats + message );
   
   return 1;

} /* do_list_groups() */


string query_help_string_for( string sub ) {
   return "This command lets you list all currently active "
      "groups and their members and leaders.";
} /* query_help_string_for() */

