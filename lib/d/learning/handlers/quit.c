#include <learning.h>

#define SAVE_FILE SAVEPATH +"search_quit"

inherit "/std/room/basic_room";

mapping locations;

void setup() {
   set_short( "quit handler of the search rooms" );
   set_light( 50 );
   set_long( "You are in the quit_handler of the search rooms of the learning "
            "domain.  In normal use, people only end up in the quit_handler "
            "if something goes wrong, since the event_enter() function will "
            "send them on to the correct place immediately.\n" );
   add_exit( "back", ENTRANCE, "path" );
   locations = ([ ]);
   if ( file_size( SAVE_FILE +".o" ) )
      unguarded( (: restore_object, SAVE_FILE :) );
   call_out( "check_names", 2 );
} /* setup() */

/* This one saves the locations of players handled by it.
 */
void save_this() { unguarded( (: save_object, SAVE_FILE :) ); }

/* This one will run through the saved people to check if they are still 
 * playing here
 */
void check_names() {
   string word;
   foreach ( word in keys( locations ) )
      if ( !"/secure/login"->test_user( word ) )
         locations = m_delete( locations, word );
   save_this();
} /* check_names() */

/* This is called by the quit command, with the quitting player and 
 * the place quitted in as parameters
 */
void player_quitting( object player, object place ) {
   if ( player->query_property( "guest" ) )
      return;
   locations[ (string)player->query_name() ] =
         (string *)place->query_keywords();
   /* if query_keywords are undefined, it's in the function search
    */
   if ( !locations[ (string)player->query_name() ] ) {   
      locations[ (string)player->query_name() ] =
          (string)place->query_marker();   
   }
   save_this();
} /* player_quitting() */

/* This is called by an event_exit in the rooms handled
 */
void remove_from_list( object player ) {
   string word;
   
   word = (string)player->query_name();
   if ( locations[ word ] )
      locations = m_delete( locations, word );
   save_this();
} /* remove_from_list() */

/* This is the room people are put in when logging on after
 * logging out from a room handled by this handler, so normally
 * it's made to dump the players their default starting position
 * if they can't be put in the right room, but since I'm such a 
 * nice guy I'll let them look around a bit.
 */
void event_enter( object player, string words, object from ) {
   string location, word;
   
   if( from ) {
      /*  Player entered from another room, to it's ok to stay
       */
      return;
   }
   word = (string)player->query_name();
   if ( !locations[ word ] ) {
      player->move( (string)player->query_start_pos(),
            "$N enter$s the game." );
      return;
   }
   if ( pointerp( locations[ word ] ) ) { /* it's one of the search rooms */
      location = (string)SEARCH->find_room( locations[ word ] );
   } else { /* it's one of the function rooms */
      location = (string)SEARCH->find_function_room( locations[ word ] );
   }
   player->move( location, "$N enter$s the game." );
} /* event_enter() */
