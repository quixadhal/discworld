/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: virtual_room.c,v 1.1 1998/01/06 04:58:17 ceres Exp $
 * $Log: virtual_room.c,v $
 * Revision 1.1  1998/01/06 04:58:17  ceres
 * Initial revision
 * 
*/
/* Base class for virtual rooms
 * Has additional functions to handle the creation of objects
 * and npc's in the room.
 * 
 * reset_objects is an array of the objects 
 * reset_definitions is an array of the definitions of how these objects
 * should be cloned
 * the value of a reset definition is an array containig the following:
 *   ({ flag, delay, clone_info, entry_mess })
 * flag can be REMOVABLE, FIXED or UNIQUE defining whether a new item
 *   should be cloned if this one is removed from the room, only cloned if
 *   this one is destructed, or as fixed, but it's the master objects that's
 *   put here(mainly for unique npc's with their own file).
 * delay is the delay in seconds between reset being called and the cloning  
 * clone_info can be a string, in which case it's considered to be the
 *   filename of the thing to put here, or a function pointer meaning
 *   it's a function returning the thing.
 * entry_mess is the parameter given to move when moving the item to this 
 *   room.
 */

inherit "/std/room";

#include <virtual_room.h>

nosave mixed  *reset_definitions;
nosave object *reset_objects;

void add_cloned_object( int flag, int delay, mixed clone_info, 
                       string entry_mess ) {
   if ( functionp( clone_info ) )
      clone_info = bind( clone_info, this_object() );
   if ( !sizeof( reset_definitions ) )
      reset_definitions = ({ ({ flag, delay, clone_info, entry_mess }) });
   else
      reset_definitions += ({ ({ flag, delay, clone_info, entry_mess }) });
} /* add_cloned_object() */

mixed *query_cloned_objects() {
   return reset_objects + ({ });
} /* query_cloned_objects() */

mixed *query_cloned_definitions() {
   return reset_definitions + ({ });
} /* query_cloned_definitions() */

private object make_clone( mixed func ) {
   if ( stringp( func ) ) {
      return clone_object( func );
   } else if ( functionp( func ) ) {
      return evaluate( func );
   } else
      return 0;
} /* make_clone() */

protected void make_object( int i ) {
   switch ( reset_definitions[ i ][ FLAG ] ) {
    case REMOVABLE:
    case FIXED:
      reset_objects[ i ] = make_clone( reset_definitions[ i ][ CLONE ] );
      reset_objects[ i ]->move( this_object(),
            reset_definitions[ i ][ MESS ] );
      break;
    case UNIQUE:
      if ( !stringp( reset_definitions[ i ][ CLONE ] ) )
         return;
      reset_definitions[ i ][ CLONE ]->move( this_object(),
            reset_definitions[ i ][ MESS ] );
      reset_objects[ i ] = find_object( reset_definitions[ i ][ CLONE ] );
   }
} /* make_object() */

void reset() {
   int i;
   
   if ( !sizeof( reset_definitions ) )
      return;
   if ( !sizeof( reset_objects ) )
      reset_objects = allocate( sizeof( reset_definitions ) );
   for ( i = sizeof( reset_definitions ); i--; ) {
      switch ( reset_definitions[ i ][ FLAG ] ) {
       case REMOVABLE:
         if ( !reset_objects[ i ] || 
             ( environment( reset_objects[ i ] ) != this_object() ) ) {
            if ( reset_definitions[ i ][ DELAY ] >= 0 ) {
               call_out( "make_object", reset_definitions[ i ][ DELAY ], i );
            } else {
               make_object( i );
            }
         }
         break;
       case FIXED:
       case UNIQUE:
         if ( !reset_objects[ i ] ) {
            if ( reset_definitions[ i ][ DELAY ] >= 0 ) {
               call_out( "make_object", reset_definitions[ i ][ DELAY ], i );
            } else {
               make_object( i );
            }
         }
      }
   }
} /* reset() */
