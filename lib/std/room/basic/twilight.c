/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: twilight.c,v 1.2 2000/04/11 19:58:57 drakkos Exp $
 *
 *
 */

/*
 * $Locker:  $
 * $Id: twilight.c,v 1.2 2000/04/11 19:58:57 drakkos Exp $
 *
 * $Log: twilight.c,v $
 * Revision 1.2  2000/04/11 19:58:57  drakkos
 * Fixed an ugly typo.
 *
 * Revision 1.1  1998/11/01 12:02:03  terano
 * Initial revision
 *
 * Revision 1.1  1998/01/15 01:37:46  ceres
 * Initial revision
 *
*/
/*
 * $Locker:  $
 * $Id: twilight.c,v 1.2 2000/04/11 19:58:57 drakkos Exp $
 *
 * $Log: twilight.c,v $
 * Revision 1.2  2000/04/11 19:58:57  drakkos
 * Fixed an ugly typo.
 *
 * Revision 1.1  1998/11/01 12:02:03  terano
 * Initial revision
 *
 * Revision 1.1  1998/01/15 01:37:46  ceres
 * Initial revision
 *
*/
/*
 * $Locker:  $
 * $Id: twilight.c,v 1.2 2000/04/11 19:58:57 drakkos Exp $
 *
 * $Log: twilight.c,v $
 * Revision 1.2  2000/04/11 19:58:57  drakkos
 * Fixed an ugly typo.
 *
 * Revision 1.1  1998/11/01 12:02:03  terano
 * Initial revision
 *
 * Revision 1.1  1998/01/15 01:37:46  ceres
 * Initial revision
 *
*/
/*
 * $Locker:  $
 * $Id: twilight.c,v 1.2 2000/04/11 19:58:57 drakkos Exp $
 *
 * $Log: twilight.c,v $
 * Revision 1.2  2000/04/11 19:58:57  drakkos
 * Fixed an ugly typo.
 *
 * Revision 1.1  1998/11/01 12:02:03  terano
 * Initial revision
 *
 * Revision 1.1  1998/01/15 01:37:46  ceres
 * Initial revision
 *
*/
// This (hopefully) will cause the long of the room to work (correctly and
// constantly), along with appropriate add_items.
//
// Functions:
//   int add_day_item( mixed shorts, mixed desc, mixed no_plural ) 
//      These items are visible only by day.
//   int add_night_item( same as above )
//      These items are visible only by night.
//   void set_day_long( string long )
//      The long of the room during day.
//   void set_night_long( string long )
//      The long of the room during the night, and when the player has light
//      to see by.
//   void room_day_chat( mixed *chats ), room_night_chat( mixed *chats )
//      The chat that pops up during the day and night, respectively.
//      Either use these or use room_chat( mixed *chats ), BUT NOT BOTH!
//      IE:  No room_day_chat( chats ) and room_chat( chats ).  Bad stuff
//      happens.  (Well, not bad, but just not what you want.)
//   int query_time()
//      Returns last_updated.  0 = NIGHT, 1 = DAY.
//
// Functions to define in your room: (Optional)
//   void changing_to_day( int just_cloned ) {...}
//   void changing_to_night( int just_cloned ) {...}
//      These functions are called whenever day turns to night or night turns
//      to day.  Just_cloned is an integer which is always either 0 or 1.  
//      The only time that it's 1 is when the room is just cloned.  This helps
//      avoid silly messages: IE: If you have a room that has an NPC which is
//      to be in the room only during the day, you don't want a "The NPC 
//      leaves the room." message if the room is cloned during the night and
//      the changing_to_night() function is called.  Unfortunately, it takes
//      a second or two for this function to be called after the room is
//      created. 
//
// This function is handled by /std/outside.c and others...
//   add_item( string|string* item, string|string* desc, int noplural )
//      Works as normal.  These items will appear day or night.
//   set_dark_mess( string long )
//      The long of the room if it's too dark.
//   set_bright_mess( string long )
//      The long of the room if it's too bright.

#include <twilight.h>
#include <climate.h>

int last_updated;
mixed roomitems;
string *roomlong;
mixed roomchats;
object shadowed_obj;

void set_current_long();
string return_long( mixed desc );
void do_change();
void change( int time_now );

void create() {
  seteuid( (string)"/secure/master"->creator_file( file_name(
    this_object() ) ) );
}

void destruct_shadow( object thing ) {
  if( thing == this_object() )
    destruct( thing );
  else
    thing->destruct_shadow( thing );
}

void setup_shadow( object this_room ) {
  shadow( this_room, 1 );

  shadowed_obj = this_room;
  
  if( WEATHER_HANDLER->query_day() ) {
    call_out( "changing_to_day", 0, 1 );
    last_updated = DAY;
  } else {
    call_out( "changing_to_night", 0, 1 );
    last_updated = NIGHT;
  }
  roomitems = ({ ({ }), ({ }) });
  roomlong = allocate( 2 );
  roomlong = ({ 0, 0 });
  roomchats = allocate( 2 );
  // Tell the WEATHER_HANDLER that this room wants to be notified.
  WEATHER_HANDLER->weather_notify( this_room, NOTIFY_DAY );
}

// This is a possibly temporary thing to periodically look at the time.
void event_weather(int whats_changed) {
  if (WEATHER_HANDLER->query_day()) {
    if (last_updated == NIGHT) 
      change( DAY );
  } else if (last_updated == DAY)
    change( NIGHT );

  shadowed_obj->event_weather( whats_changed );
}

void change( int time_now ) {
  int i;
  
  /* Time to update the room. */
  last_updated = time_now;
  set_current_long();
  
  /* Turn on/off items */
  /* Changed by Presto 12/20/97 */
  for ( i = 0; i < sizeof( roomitems[ last_updated ] ); i += 2 )  {
    shadowed_obj->modify_item( roomitems[ last_updated ][ i ],
                               roomitems[ last_updated ][ i + 1 ] );
  }
  for ( i = 0; i < sizeof( roomitems[ 1 - last_updated ] ); i += 2 )  {
    if ( member_array( roomitems[ 1 - last_updated ][ i ],
                       roomitems[ last_updated ] ) == -1 )  {
      shadowed_obj->modify_item( roomitems[ 1 - last_updated ][ i ],
                                 "You can't see that now.\n" );
    }
  }
  
  /* New room chats */
  /* Changed by Presto 12/20/97 */
  shadowed_obj->stop_room_chats();
  if( sizeof(roomchats[last_updated]) )
    shadowed_obj->room_chat( roomchats[last_updated] );
  
  /* Tell the room that the time has changed. */
  if( last_updated == DAY )
    shadowed_obj->changing_to_day( 0 );
  else
    shadowed_obj->changing_to_night( 0 );
}

void set_day_long( string str ) {
  roomlong[DAY] = str;
  set_current_long();
}

void set_night_long( string str ) {
  roomlong[NIGHT] = str;
  set_current_long();
}

void set_current_long() {
  if (roomlong[last_updated] != 0 && roomlong[last_updated] != "")
    shadowed_obj->set_long( roomlong[last_updated] );
}

varargs int add_day_item( mixed shorts, mixed desc, mixed no_plural ) {
  string the_item;

  if( pointerp( shorts ) )
    the_item = shorts[0];
  else
    the_item = shorts;

  roomitems[DAY] += ({ the_item, return_long( desc ) });

  if( last_updated == DAY ) 
    return this_object()->add_item( shorts, desc, no_plural );
  
  return 1;
}

varargs int add_night_item( mixed shorts, mixed desc, mixed no_plural ) {
  string the_item;
  
  if( pointerp( shorts ) )
    the_item = shorts[0];
  else
    the_item = shorts;

  roomitems[NIGHT] += ({ the_item, return_long( desc ) });
    
  if( last_updated == NIGHT ) 
    return shadowed_obj->add_item( shorts, desc, no_plural );
  return 1;
}

void room_day_chat( mixed *args ) {
  roomchats[DAY] = args;
  if( last_updated == DAY )
    shadowed_obj->room_chat( args );
}

void room_night_chat( mixed *args ) {
  roomchats[NIGHT] = args;
  if( last_updated == NIGHT )
    shadowed_obj->room_chat( args );
}

/* Returns the long part of the description. */
string return_long( mixed desc ) {
  int ma;

  if( !pointerp( desc ) )
    return (string)desc;

  ma = member_array( "long", desc );
  if( ma < 0 )
    return "Error: No long found.";
  
  return (string)desc[ma+1];
}

int query_time() { return last_updated; }
mixed query_roomitems() { return roomitems; }
string *query_roomlong() { return roomlong; }
mixed query_roomchats() { return roomchats; }
