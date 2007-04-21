/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: start_positions.c,v 1.2 2001/06/20 15:27:05 shrike Exp $
 *
 *
 */

#include <login_handler.h>

#define SAVE_FILE "/save/start_position_handler"
#define CHECKED_PROP "checked start positions"

private int _last_added;
private string * _buggy_starts;

/** @ignore */
void load_file() {
   if ( file_size( SAVE_FILE + ".o" ) > 0 ||
        file_size( SAVE_FILE + ".o.gz" ) > 0 )
   {
      unguarded( (: restore_object, SAVE_FILE :) );
   }
} /* load_file() */

/** @ignore */
void save_file() {
   unguarded( (: save_object, SAVE_FILE :) );
} /* save_file() */

/** @ignore */
void create() {
   _last_added = 0;
   _buggy_starts = ({ });

   load_file();

   if ( !_buggy_starts ) {
      _buggy_starts = ({ });
   }
} /* create() */

/**
 * Add a filename to the array of buggy start positions to be removed.
 * Returns 1 if successful, -1 if the start position was already in the
 * list, or 0 for an error.
 * @param fname The filename of the room.  '.c' will be stripped.
 * @return The integer success code.
 */
int add_buggy_start( string fname ) {
   if ( !fname ) {
      return 0;
   }
   fname = lower_case( fname );

   if ( fname[ <2 .. ] == ".c" ) {
      fname = fname[ 0 .. <3 ];
   }

   if ( member_array( fname, _buggy_starts ) != -1 ) {
      return -1;
   }

   _buggy_starts += ({ fname });
   _last_added = time();
   save_file();
   return 1;
} /* add_buggy_start() */

/**
 * Remove a filename from the array of buggy start positions.
 * Returns 1 if successful, -1 if the start position was not in the list,
 * or 0 for an error.
 * @param fname The filename of the room.  '.c' will be stripped.
 * @return The integer success code.
 */
int delete_buggy_start( string fname ) {
   if ( !fname ) {
      return 0;
   }
   fname = lower_case( fname );

   if ( fname[ <2 .. ] == ".c" ) {
      fname = fname[ 0 .. <3 ];
   }

   if ( member_array( fname, _buggy_starts ) == -1 ) {
      return -1;
   }

   _buggy_starts -= ({ fname });
   save_file();
   return 1;
} /* delete_buggy_start() */

/**
 * Get a list of all the currently registered buggy start positions.
 * @return The list of filenames.
 */
string * list_buggy_starts() {
   return copy( _buggy_starts );
} /* list_buggy_starts() */

/**
 * Process a player and remove start positions as required.
 * @param player The player object to check.
 */
void remove_buggy_starts( object player ) {
   string * starts;

   if ( !player ) {
      return;
   }

   starts = copy( player->query_starts() );
   for( int i = 0; i < sizeof( starts ); i += 2 ) {
      if ( member_array( starts[i], _buggy_starts ) != -1 ) {
         player->remove_start( starts[i] );
      }
   }
   player->add_property( CHECKED_PROP, time() );
} /* remove_buggy_starts() */

/**
 * This is the callback function registered with the login handler.  It
 * takes two arguments: the player and the type of login event.  It checks
 * a property on the player so that it doesn't process the list of buggy
 * starts once for each player every single time they log in.
 * @param player the player's name to check.
 * @param type the type of login event, as defined in <login_handler.h>.
 */
void login_callback( string player, string type ) {
   object playerob;

   if ( !player ||
        !type ||
        type != LOGIN )
   {
      return;
   }
   player = lower_case( player );

   if ( !( playerob =  find_player( player ) ) ||
        ( playerob->query_property( CHECKED_PROP ) >= _last_added ) )
   {
      return;
   }

   remove_buggy_starts( playerob );
} /* login_callback() */

/** @ignore */
void dest_me() {
   destruct( this_object() );
} /* dest_me() */
