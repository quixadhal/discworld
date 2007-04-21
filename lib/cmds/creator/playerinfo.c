/**
 * Print info - prints playerinfo database entries for a given player.
 * If the reason argument is given, print only entries referring to that
 * event.
 *
 * @author Fiona
 */
#include <creator.h>
#include <playerinfo.h>
#include <player_handler.h>

#define LOGGING

inherit "/cmds/base";

int do_report( string player, string event, int lastn, int interleaved ) {
   string who = this_player()->expand_nickname( lower_case(player) );
   string ret;

   if( !PLAYER_HANDLER->test_user(who) ) {
      tell_object( this_player(), "%^RED%^%^BOLD%^NOTE: %^RESET%^"
        "This is a player that either "
        "never existed or no longer exists.  Some deleted players' "
        "dossiers remain, while others do not.\n" );
   }

#ifdef LOGGING
   log_file( "/d/admin/log/PLAYERINFO.log", "%s: %s read dossier of %s.\n",
     ctime( time() ), this_player()->query_name(), who );
#endif

   if( interleaved ) {
     ret = PLAYERINFO_HANDLER->query_interleaved(this_player(), who, 
                                                 event);
   } else if( !event ) {
     ret = PLAYERINFO_HANDLER->query_dossier( this_player(), who, lastn );
   } else {
     ret = PLAYERINFO_HANDLER->query_event( this_player(), who,
                                            lower_case(event) );
   }
   ret = this_player()->fix_string(ret, this_player()->query_cols());
   return this_player()->more_string(ret, "Playerinfo", 1);
} /* do_report() */

int list_events() {
   string * list;
   string event, myself, result;

   list = sort_array( PLAYERINFO_HANDLER->query_events(), 1 );
   myself = base_name(this_object());

   result = "";
   foreach( event in list )
     result += ( event == list[0] ) ? event : ( ", " + event );
   write( "The following events exist: " + result + ". \n" );

   list = filter( list,
     (: PLAYERINFO_HANDLER->query_source_ok( $1, $(myself) ) :) );

   write( "You can manually add the following events: "
     + query_multiple_short(list) + ". \n" );
   return 1;
} /* list_events() */

// Update the database with the data the user has entered
void update_database( string comment, string * data ) {
   if( !comment )
      return;
   if( PLAYERINFO_HANDLER->add_entry( this_player(), data[0], data[1],
                                      comment + "\n" ) ) {
      write( capitalize( data[0] ) + "'s dossier updated.\n" );
   } else {
      write( "Cannot update " + capitalize( data[0] ) + "'s dossier.\n" );
   }
} /* update_database() */

int do_acknowledge( string player, string event, int idx, string update ) {
   if( !(PLAYERINFO_HANDLER->is_alert( player, idx ) ) ) {
      add_failed_mess( "That event is not an alert, so cannot be "
        "acknowledged.\n" );
      return -1;
   }

   if( !PLAYERINFO_HANDLER->acknowledge_alert( this_player(), player,
     event, idx, update, 0 ) ) {
      add_failed_mess( "You do not have the permission to acknowledge that alert, "
        "or the event type is invalid.\n" );
      return -1;
   }
   add_succeeded_mess( ({ "Event acknowledged.\n", "" }) );
   return 1;
} /* do_acknowledge() */

int list_alerts( int foo ) {
   string * names;

   if( foo )
      names = PLAYERINFO_HANDLER->query_online_alerts();
   else
      names = keys( PLAYERINFO_HANDLER->query_alerts() );

   if( !sizeof( names ) )
      write( "There are no players with unacknowledged alerts.\n" );
   else
      write( "The following players have unacknowledged alerts: "
        + query_multiple_short( names ) + ". (" + sizeof( names ) +
        ")\n" );

   return 1;
} /* list_alerts() */

int add_event( string player, string event, string comment ) {
   string *events;
   string myself;

   player = this_player()->expand_nickname( lower_case(player) );
   event = lower_case(event);

   if( !PLAYER_HANDLER->test_user(player) ) {
      add_failed_mess( "No such player: " + player + ".\n" );
      return 0;
   }

   myself = base_name( this_object() );

   events = filter( PLAYERINFO_HANDLER->query_events(),
     (: PLAYERINFO_HANDLER->query_source_ok( $1, $(myself) ) :) );
   if( member_array( event, events ) == -1 ) {
      add_failed_mess( "Incorrect event type '" + event
        + "': it must be one of " + query_multiple_short(events) + ".\n" );
      return 0;
   }

   if( comment ) {
      update_database( comment, ({ player, event }) );
      return 1;
   }
   write( "Please enter your comments:\n" );
   this_player()->do_edit( 0, "update_database", this_object(), 0,
     ({ player, event }) );
   return 1;
} /* add_event() */

int add_alts( string s1, string s2, int deleting ) {
   string who, result;
   string * alts;

   who = this_player()->expand_nickname( lower_case(s1) );
   alts = explode( lower_case(s2), " " );
   alts = explode( implode( alts, "," ), "," ) - ({ "" }) - ({ 0 });

   if( deleting ) {
      result = PLAYERINFO_HANDLER->delete_alts( this_player(), who, alts );
   } else {
      result = PLAYERINFO_HANDLER->add_alts( this_player(), who, alts );
   }

   add_succeeded_mess( ({ result, "" }) );
   return 1;
} /* add_alts() */

// Return the command pattern list.
mixed *query_patterns() {
   return ({
     "list",
       (: list_events() :),
     "list alerts",
       (: list_alerts(0) :),
     "list alerts online",
       (: list_alerts(1) :),
     "<word'playername'>",
       (: do_report( $4[0], 0, 10, 0 ) :),
     "<word'playername'> verbose",
       (: do_report( $4[0], 0, 0, 0 ) :),
     "<word'playername'> interleaved",
       (: do_report( $4[0], 0, 0, 1 ) :),
     "<word'playername'> <word'event type'> interleaved",
       (: do_report( $4[0], $4[1], 0, 1 ) :),
     "<word'playername'> <word'event type'>",
       (: do_report( $4[0], $4[1], 0, 0 ) :),
     "<word'playername'> acknowledge <number'event number'> "
       "<string'comment'>",
       (: do_acknowledge( $4[0], "misc", $4[1], $4[2] ) :),
     "<word'playername'> acknowledge <number'event number'> "
       "as <word'event type'> <string'comment'>",
       (: do_acknowledge( $4[0], $4[2], $4[1], $4[3] ) :),
     "<word'playername'> add [event] <word'event type'>",
       (: add_event( $4[0], $4[1], 0 ) :),
     "<word'playername'> add [event] <word'event type'> <string'comment'>",
       (: add_event( $4[0], $4[1], $4[2] ) :),
     "<word'playername'> addalt <string'alt-name alt-name ...'>",
       (: add_alts( $4[0], $4[1], 0 ) :),
     "<word'playername'> delalt <string'alt-name alt-name ...'>",
       (: add_alts( $4[0], $4[1], 1 ) :),
   });
} /* query_patterns() */
