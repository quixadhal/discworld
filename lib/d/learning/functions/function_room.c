#include "path.h"
#include <map.h>

inherit "/std/room/basic_room";

string function_chosen;

void setup() {
   set_short( "uninitialised search room for functions" );
   set_long( "You are in an uninitialized search room.  "
            "This is an example of a room that gets cloned and configured "
            "afterwards.  In this room, it's the function set_marker that's " 
            "used to define its look, and most if the exits as well.  And "
            "the function set_destination that is used to find, possibly "
            "clone and configure, the rooms that fit the keywords you search "
            "for.  The handler called SEARCH in the code, is \""+
            SEARCH +"\" and the one called QUIT_HANDLER, is \""+ 
            QUIT_HANDLER +"\".\n" );
   add_property( "no map", 1 );
   /* set_light sets the amount of light the room has.  See help light
    * for a list of what the number means:)
    */
   set_light( 50 );

   add_exit( "exit", PATH +"functions", "path" );
   /* add_property is used to add a value to an object, that can later be
    * queried for with query_property.
    * The "commented functions" and "keywords" properties are specific for 
    * rooms in the learning domain and is used to make it possible to search
    * for subjects. 
    * See /d/learning/handlers/search.c for the way this is done.
    */
   add_property( "commented functions", 
                ({ "set_destination", "query_quit_handler", 
                   "query_cloning_info" }) );
   add_property( "keywords", ({ "room", "virtual" }) );
} /* setup() */

string query_marker() {
   return function_chosen;
} /* query_marker() */ 

void set_marker( string word ) {
   string description, exit_letter, exit_room, exit;
   
   set_short( "\""+ word +"\" search room" );
   add_property( "determinate", "the " );
   function_chosen = word;
   description = "You have chosen to see the rooms describing the use of the "
         "function "+ function_chosen +".";
   description += "$I$5=Take one of the lettered exits to go to that room.";
   exit_letter = "a";
   foreach( exit_room in (string *)SEARCH->query_rooms_with_function( word ) ) {
      exit = sprintf( "%s: %s", 
                     exit_letter, 
                     (string)MAP->query_short( exit_room ) );
      add_exit( exit_letter, exit_room, "path" );
      description += "\n$C$"+ exit +".";
      exit_letter[ 0 ]++;
   }
   description += "$I$0=\n";
   set_long( description );
} /* set_marker() */ 

/* This is for cleanup, it's called 10 seconds after something left, and if
 * nothing is in the room it's removed.
 */
void check_empty() {
   if ( sizeof( all_inventory( this_object() ) ) )
      return;
   dest_me();
} /* check_empty() */

/* query_quit_handler() is a function called by the player to get the filename
 * of the quit_handler. 
 * When quitting, the function player_quitting() is called in the quit_handler
 * which will save the
 */
string query_quit_handler() { return QUIT_HANDLER; }

/* query_cloning_info() is called by things that need to be able to remare
 * this room.
 * It it an array of three or four elements, the first of which is the
 * object, the second is a function called in it, with the third and if it's
 * there, the fourth element as arguments.
 */
mixed *query_cloning_info() {
   return ({ SEARCH, "find_function_room", function_chosen });
} /* query_cloning_info() */

/* This is for cleaning up the information saved by the quit_handler, to
 * make sure that the players name is removed from the lists.
 * event_exit is generally called whenever someone leaves the room.
 */
void event_exit( object thing, string mess, object to ) {
   string handler;
   
   if ( find_call_out( "check_empty" ) == -1 )
      call_out( "check_empty", 10 );
   if ( file_name( to ) == "/room/departures" )
      return;
   handler = query_quit_handler();
   if ( (string)to->query_quit_handler() != handler )
      handler->remove_from_list( thing );
} /* event_exit() */
