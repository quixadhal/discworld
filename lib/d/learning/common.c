#include <config.h>
#include "path.h"

/* inherit "/std/dom/cmn_mas"; 
 * being a common room, this should inherit the previous, but since that's 
 * basically a piece of shit, I won't have it contaminate my nice room:)
 */
inherit "/std/room/basic_room";

#define HANDLER (LEARNING +"handlers/request")

object board;
object *chairs; /* people sitting */
int do_claim(int);
int do_remove(int);
int do_request(string, string);
int do_sit( string command, object *indir, string dir_match, 
           string indir_match, mixed *args, string pattern );

void setup() {
   set_light( 100 );
   set_short( "somewhat boring commonroom of the learning domain" );
   add_property( "determinate", "the " );
   set_long( "This is the common room of the domain of learning, the place "
            "where creators come to learn.  It is a somewhat boring room "
            "except for some chairs strewn around the place which happen to "
            "be an example of having commands defined in add_items, and some "
            "sort of list hanging on the wall which is an example of using "
            "add_command in rooms.\n" );
   add_item( "chair", ({ 
      "long", "This chair looks comfortable, perhaps you can sit in it.",
      "sit", ({ (: do_sit :), "[in] <direct:object>" })
   }), 1 );
   add_item( "chairs", ({ 
      "long", "These chairs look comfortable, "
              "perhaps you can sit in one of them.",
      "sit", ({ (: do_sit :),  "[in] <direct:object>" })
   }), 1 );
   add_item( "list", 
            "This list has several actions associated with it:\n"
            "\"show list\" will show the current requests.\n" 
            "\"request documentation\" will ask for "
            "documentation on something.\n" 
            "\"request example\" will ask for an example of "
            "something to be coded.\n" 
            "\"claim <number>\" is use to claim a request, thereby attempting "
            "to make sure that only one creator is writing the documentation/"
            "coding the examples." );
   add_exit( "entrance", PATH +"main", "path" );
   add_exit( "drum", CONFIG_START_LOCATION, "path" );

   /* make all chairs empty */
   chairs = ({ });
   /* calling this result in the function extra_look() being called whenever
    * people look at the room */
   add_extra_look( this_object() );
   /* the normal add_property() calls for the learning domain */
   add_property( "commented functions", ({ "add_extra_look", "extra_look",
      "remove_extra_look", "add_command" }) );
   add_property( "keywords", ({ "room", "add_item", "action" }) );
   /*make a copy of the domain's board */
   board = clone_object( "/obj/misc/board" );
   board->set_datafile( DOMAIN );
   board->move( this_object() );
} /* setup() */

void init() {
   ::init();
   add_command( "show", "list" );
   add_command( "claim", "<number>", (:do_claim( $4[0] ):) );
   add_command( "request", "{documentation|example} <string'subject'>",
                (:do_request( $4[0], $4[1] ):));
   add_command( "remove", "<number>", (:do_remove( $4[0] ):) );
} /* init() */

int do_show() {
   write( HANDLER->query_list() );
   return 1;
} /* do_show() */

int do_claim( int i ) {
  if ( HANDLER->claim_request( this_player()->query_name(), i ) ) {
    add_succeeded_mess("$N $V a request.\n", ({}));
  } else {
    return notify_fail( "Illegal request number.\n" );
  }
  return 1;
} /* do_claim() */

int do_remove( int i ) {
  if ( HANDLER->remove_request( this_player()->query_name(), i ) ) {
    add_succeeded_mess("$N $V a request.\n", ({}));
  } else {
    return notify_fail( "You can't remove that request.\n" );
  }
  return 1;
} /* do_remove() */

int do_request( string option, string words ) {
  if ( option == "documentation" ){
      HANDLER->add_request( this_player()->query_name(),
                           "documentation for "+ words );
      write( "You request some documentation.\n");
      say( this_player()->the_short() +" requests some documentation.\n");
      return 1;
   }
  if ( option == "example") {
      HANDLER->add_request( this_player()->query_name(),
                           "examples of "+ words );
      write( "You request some example.\n");
      say( this_player()->the_short() +" requests some example.\n");
      return 1;
   }
   return 0;   
} /* do_request() */

int do_sit( string command, object *indir, string dir_match, 
           string indir_match, mixed *args, string pattern ) {
   /* Note!  I use add_succeeded_mess even though the command failed, 
    * because it fails for another reason than being the wrong object.
    * This make it work even though there is a "sit" soul.
    */
   if ( dir_match == "chairs" ) {
      this_player()->add_succeeded_mess( previous_object(), 
             "How can you sit on more than one chair at the same time?\n", 
             ({ }) );
      return 1;
   }
   if ( member_array( this_player(), chairs ) != -1 ) {
      this_player()->add_succeeded_mess( previous_object(), 
             "You are already sitting in a chair.\n", ({ }) );
      return 1;
   }
   /* add the player to the list of people in the chairs */
   chairs += ({ this_player() });
   /* calling add_extra_look() result in the function extra_look being     
    * called whenever people look at the person */
   this_player()->add_extra_look( this_object() );
   this_player()->add_succeeded_mess( previous_object(), 
           "$N $V in a "+ dir_match +".\n", ({ }) );
   return 1;
} /* do_sit() */

string extra_look( object thing ) {
   if ( thing == this_object() ) { /* we're looking at the room */
      /* remove people who have logged out */
      chairs = filter_array( chairs, (: $1 != 0 :) );
      /* and those who have left */
      chairs = filter_array( chairs, 
                            (: environment( $1 ) == this_object() :) );
      if ( !sizeof( chairs ) ) { /* none left sitting */
         return "";
      } else {
         if ( sizeof( chairs ) == 1 ) {           /* only one left */
            if ( chairs[ 0 ] == this_player() ) { /* and it's you */
               return "You are sitting in one of the chairs.\n";
            } else {                              /* and it's someone else */
               return (string)chairs[ 0 ]->the_short() 
                      +" is sitting in one of the chairs.\n";
            }
         } else {
            return query_multiple_short( chairs, "the" ) 
              +" are sitting in the chairs.\n";
         }
      }
   } else if ( member_array( thing, chairs ) != -1 ) {
      /* we're looking at someone who might be sitting */
      if ( environment( thing ) == this_object() ) { /* sitting and here */
         if ( thing == this_player() ) { /*looking at yourself */
            return "You are sitting in a chair looking lazy.\n";
         } else {
            return thing->query_pronoun() +" is lazing around in a chair.\n";
         }
      }
   }
   /* calling remove_extra_look() stop extra_look() being     
    * called whenever people look at the thing */
   thing->remove_extra_look( this_object() );
   /* show nothing */
   return "";
} /* extra_look() */
