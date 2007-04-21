/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: multichatter.c,v 1.1 1999/09/21 02:44:39 pinkfish Exp $
 *
 */

/**
 * The multiple rates chatter inheritable for room chats.  
 * This chatter handles multiple calls to room_chat
 * by adding the chats independently with their own 
 * chat interval.
 * @author Shaggy
 * @see /std/room.c
 * @see room_chat
 */
#include <room.h>

nosave int off;
nosave object room;
nosave mixed *chats;

/**
 * Allows the chat interval to be changed.
 * @param min minimum interval between chats (seconds)
 * @param max maximum interval between chats (seconds)
 */
void set_chat_min_max( int min,int  max ) {
  if (chats) {
    chats[0]=min;
    chats[1]=max;
  }
}

/**
 * Gives the chatter a room to chat at and the chat args.
 * If you call it again it you can give the room an
 * additional set of chats without destroying the
 * first, with the new set operating independently at
 * the new rate.  You can call this function as many
 * times as you like.
 * @param my_room room object
 * @param chat_args chat data: ({ min, max, chats array })
 * @see room_chat
 * @see query_room_chats
 * @see /std/room/basic/chatter
 */
void setup_chatter( object my_room,  mixed *chat_args ) {
   off = 1;
   room = my_room;
   if (sizeof(chats)) chats += chat_args + ({0,0});
   else chats = chat_args + ({0,0});
} /* setup_chatter() */

void dest_me() {
  destruct( this_object() );
}

int query_multichatter() { return 1; }

int check_interactive( object thing ) { return interactive( thing ); }

void make_chat() {
  int saying;
  string stuff;
  int set,nextt,t;

  if (!sizeof( filter_array( all_inventory( room ), "check_interactive",
        this_object() ) ) ) {
    off = 1;
    return;
  }
  nextt=2147483647; // largest possible int, roughly the year 2030 :)
  t=time();
  for (set=0;set<sizeof(chats);set+=5) {
    if (sizeof(chats[set+2])) {
      if (t>=chats[set+4]) {
        saying = random( sizeof( chats[ set+2 ] ) );
        if ( saying == chats[ set+3 ] )
        saying = ( saying + 1 ) % sizeof( chats[ set+2 ] );
        chats[ set+3 ] = saying;
        stuff = chats[ set+2 ][ saying ];
        if ( stuff[ 0 ] == '#' )
          call_other( room, stuff[ 1 .. 99 ] );
        else
          tell_room( room, chats[ set+2 ][ saying ] +"\n" );
        chats[ set+4 ]=time()+chats[ set ] 
            +random(chats[ set+1 ] - chats[ set ] + 1)-4;
// The -4 is to do with the fact that chats only occur at most
// every 4 seconds (see the room_handler)
      }
      if (chats[ set+4 ]<nextt) nextt=chats[ set+4 ];
    }
  }
  if (nextt!=2147483647) {
    if ((nextt+=4-t)<5) nextt=5; 
    ROOM_HANDLER->add_chatter( this_object(), nextt );
  }
} /* make_chat() */

/**
 * Checks that the chatter is on and being activated
 * properly from the room_handler. 
 * @see /std/room/basic/chatter
 * @see /obj/handlers/room_handler
 */
void check_chat() {
  int set,t,nextt;

  if ( !off || !pointerp( chats ) )
      return;
  off = 0;

  t=time();
  nextt=2147483647;
  for (set=0; set<sizeof(chats); set+=5) {
    if (t>=chats[set+4]+chats[set]) chats[set+4]=t+random(chats[set+1]);
    if (chats[set+4]<nextt) nextt=chats[set+4];
  }
  if ((nextt+=4-t)<5) nextt=5;
  ROOM_HANDLER->add_chatter( this_object(), nextt );
} /* check_chat() */

/**
 * Adds more chats to the existing set of room chats
 * managed by this chatter object.  This function
 * always adds to the first set of chats even if
 * multiple sets exist on the chatter.
 * @param new_chats an array of new chat strings
 * @see remove_room_chats
 * @see query_room_chats
 * @see /std/room/basic/chatter
 */
void add_room_chats( string *new_chats ) {
   if (sizeof(new_chats)) { 
     if (sizeof(chats[2])) chats[2] = chats[2] + new_chats;
     else chats[2]= new_chats;
   }
} /* add_room_chats() */

/**
 * Removes chats from the set of room chats
 * managed by this chatter object.  If there are no chats
 * left the chatter is destructed.  This function
 * removes chats from any set by identifying all
 * copies in each set and removing them.
 * @param dead_chats an array of chat strings to remove
 * @see add_room_chats
 * @see query_room_chats
 * @see /std/room/basic/chatter
 */
void remove_room_chats( string *dead_chats ) {
  int set;
  mixed *new_chats;

  new_chats=({ });
  for (set=0;set<sizeof(chats);set+=5) {
    if (sizeof(chats[set+2])) chats[set+2] = chats[set+2] - dead_chats;
    if (set==0 || sizeof(chats[set+2])) 
       new_chats=new_chats+chats[(set)..(set+4)];
  }
  chats=new_chats;
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
mixed *query_room_chats() { return chats; }

/** @ignore yes */
int clean_up( int parent ) {
/* Clones of this object are pointless without a room to chat to. */
  if (parent) {
    return 0;
  }
  if (!room) dest_me();
} /* clean_up() */





