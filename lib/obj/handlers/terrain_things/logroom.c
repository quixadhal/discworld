/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: logroom.c,v 1.1 1998/01/06 05:06:10 ceres Exp $
 * $Log: logroom.c,v $
 * Revision 1.1  1998/01/06 05:06:10  ceres
 * Initial revision
 * 
*/
#include <terrain.h>

#define SAVE_FILE RESTORE_PATH +"logroom"

inherit "/std/room";

mapping locations;

void setup() {
   set_short( "limbo" );
   set_light( 50 );
   set_long( "You are between worlds, surrounded by raw potentiality and "+
         "not a lot else.\n" );
   add_item( "potentiality", "Well, it's sort of potentially something and "+
         "potentially nothing, and it can't quite decide at the moment.  "+
         "With any luck, though, you won't be here long enough to find "+
         "out...\n" );
   add_exit( "drum", "/d/am/am/mendeddrum", "path" );
   locations = ([ ]);
   if ( file_size( SAVE_FILE +".o" ) )
      unguarded((: restore_object, SAVE_FILE :));
} /* setup() */

void save_this() {
  unguarded((: save_object, SAVE_FILE :));
}

void player_quitting( object player, object place ) {
   if ( player->query_property( "guest" ) )
      return;
   locations[ (string)player->query_name() ] =
         ({ (string)place->query_property( "terrain name" ),
         (int *)place->query_co_ord() });
   save_this();
   call_out( "check_quitted", 5, (string)player->query_name() );
} /* player_quitting() */

void check_quitted( string word ) {
   if ( !find_player( word ) )
      return;
   if ( locations[ word ] )
      locations = m_delete( locations, word );
   save_this();
} /* check_quitted() */

void event_enter( object player, string words, object thing ) {
   object location;
   mixed *data;
   data = locations[ (string)player->query_name() ];
   if ( !data ) {
      player->move( (string)player->query_start_pos(),
            "$N enter$s the game." );
      return;
   }
   location = (object)TERRAIN_MAP->find_location( data[ 0 ],
         data[ 1 ] );
   player->move( location );
   locations = m_delete( locations, (string)player->query_name() );
   save_this();
} /* event_enter() */
