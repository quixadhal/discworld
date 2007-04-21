/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: chatter.c,v 1.10 2003/07/14 19:41:13 ceres Exp $
 *
 */

/**
 * The standard chatter inheritable for room chats.  
 * @author Pinkfish
 * @see /std/room.c
 * @see room_chat
 */
#include <room.h>

nosave int _off;
nosave object _room;
nosave mixed *_chats;

/**
 * Allows the chat interval to be changed.
 * @param min minimum interval between chats (seconds)
 * @param max maximum interval between chats (seconds)
 */
void set_chat_min_max( int min,int  max ) {
  if (_chats) {
    _chats[0]=min;
    _chats[1]=max;
  }
} /* set_chat_min_max() */

/**
 * Gives the chatter a room to chat at and the chat args.
 * @param my_room room object
 * @param chat_args chat data: ({ min, max, chats array })
 * @see room_chat
 * @see query_room_chats
 * @see /std/room/basic/chatter
 */
void setup_chatter( object my_room,  mixed *chat_args ) {
   _off = 1;
   _room = my_room;
   
   if( !arrayp( chat_args ) ) {
      chat_args = ({ chat_args });
   }
   
   _chats = chat_args + ({ 0 });
} /* setup_chatter() */

/** @ignore yes */
void dest_me() {
  destruct( this_object() );
} /* dest_me() */

/** @ignore yes */
void make_chat() {
   int saying;
   mixed stuff;

   if(!_room) {
     dest_me();
     return; 
   }
   
   if ( !_chats || sizeof(_chats) < 3 || !sizeof(_chats[2]) ||
        !sizeof( filter( all_inventory( _room ), (: interactive($1) :),
         this_object() ) ) ) {
      _off = 1;
      return;
   }
   saying = random( sizeof( _chats[ 2 ] ) );
   if ( saying == _chats[ 3 ] ) {
     saying = ( saying + 1 ) % sizeof( _chats[ 2 ] );
   }
   _chats[ 3 ] = saying;
   stuff = _chats[ 2 ][ saying ];
   if (stringp(stuff)) {
      if ( stuff[ 0 ] == '#' ) {
         call_other( _room, stuff[ 1 .. 99 ] );
      } else {
         tell_room( _room, _chats[ 2 ][ saying ] +"\n" );
      }
   }
   if ( intp( stuff ) ) {
      map( filter( all_inventory( _room ), (: $1 && living( $1 ) 
         && $1->query_creator() :) ), (: tell_creator( $1, "Warning: "
         "room_chat array contains integer argument. Did you mix up "
         "the syntax with load_chat()'s?\n" ) :) );
   }
   ROOM_HANDLER->add_chatter( this_object(), _chats[ 0 ] +
         random( _chats[ 1 ] - _chats[ 0 ] + 1 ) );
} /* make_chat() */

/**
 * Checks that the chatter is on and being activated
 * properly from the room_handler. 
 * @see /std/room/basic/chatter
 * @see /obj/handlers/room_handler
 */
void check_chat() {
   if ( !_off || !pointerp( _chats ) ) {
      return;
   }
   _off = 0;
   ROOM_HANDLER->add_chatter( this_object(), ( _chats[ 0 ] +
         random( _chats[ 1 ] - _chats[ 0 ] + 1 ) ) / 2 );
} /* check_chat() */

/**
 * Adds more chats to the existing set of room chats
 * managed by this chatter object.
 * @param new_chats an array of new chat strings
 * @see remove_room_chats
 * @see query_room_chats
 * @see /std/room/basic/chatter
 */
void add_room_chats( string *new_chats ) {
   if (sizeof(new_chats)) { 
     if (sizeof(_chats[2])) {
        _chats[2] = _chats[2] + new_chats;
     } else {
        _chats[2]= new_chats;
     }
   }
} /* add_room_chats() */

/**
 * Removes chats from the set of room chats
 * managed by this chatter object.  If there are no chats
 * left the chatter is destructed.
 * @param dead_chats an array of chat strings to remove
 * @see add_room_chats
 * @see query_room_chats
 * @see /std/room/basic/chatter
 */
void remove_room_chats( string *dead_chats ) {
   if (sizeof(_chats[2])) {
      _chats[2] = _chats[2] - dead_chats;
   }
   if (!sizeof(_chats[2])) {
      destruct(this_object());
   }
} /* remove_room_chats() */

/**
 * Returns the the set of room chats
 * managed by this chatter object.  
 * @return pointer to the mixed array of chat args
 * @example
 *  ({ 120, 240, ({ "A frog gimbles the curtains.",
 *                  "A truly revolting smell drifts insidiously "
 *                  "from the rug." }) })
 * @see add_room_chats
 * @see remove_room_chats
 * @see room_chat
 * @see /std/room/basic/chatter
 */
mixed *query_room_chats() { return _chats; }

/** @ignore yes */
int clean_up( int parent ) {
  /* Clones of this object are pointless without a room to chat to. */
  if (parent)
    return 0;
  if (!_room || _room->query_chatter() != this_object())
    dest_me();
}
