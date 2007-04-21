/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: broadcaster.c,v 1.12 2002/06/15 00:58:52 pinkfish Exp $
 * $Log: broadcaster.c,v $
 * Revision 1.12  2002/06/15 00:58:52  pinkfish
 * Fix up some issues.
 *
 * Revision 1.11  1999/09/15 02:44:36  pinkfish
 * Make it handle unguardeding the save/loads.
 *
 * Revision 1.10  1999/06/04 00:46:34  pinkfish
 * Make it handle lots of people online when someone shouts.
 *
 * Revision 1.9  1999/05/15 23:43:34  ceres
 * Modified to only colour shouts (hopefully)
 *
 * Revision 1.7  1999/04/19 20:25:37  pinkfish
 * Fix up an error that was causing a runtime.
 *
 * Revision 1.6  1999/01/30 23:12:38  pinkfish
 * Make it remove empty channels and add in a method to determine if a
 * channel exists or not.
 *
 * Revision 1.5  1998/10/23 08:41:00  pinkfish
 * Added in a channel history feasture.
 *
 * Revision 1.3  1998/10/11 12:34:35  pinkfish
 * Add in code to the broadcaster to deal with channels.
 *
 * Revision 1.2  1998/04/06 07:02:48  sin
 * Installed Shaggy's change to keep track of NPCs who want to hear shouts.
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/**
 * Basic sight and sound broadcaster.
 */

#define DIR_ARRAY ({ "east", "northeast", "north", "northwest", \
                     "west", "southwest", "south", "southeast", "east" })

#define SAVE_FILE "/save/broadcaster"
#define BROADCAST_HISTORY_LENGTH 10

void broadcast_event( object *things,   /* things that get told about it */
                      int *centre,      /* coords where event occurs */
                      string message,   /* what the things get told */
                      int range,        /* range of telling in room units */
                      int inside,       /* tell things if indoors */
                      int underwater ); /* tell things if underwater */
void npc_hear_shouts(object newlistener);
int npc_unhear_shouts(object oldlistener);
void npc_shout_event( object shouter,
                      string start, 
                      string message,
                      string lang, 
                      int *coord, 
                      int range ); /* arguments for event_person_shout */
private void load_me();
private void save_me();
  
private nosave object *_listeners;
private nosave mapping _channels;
private nosave mapping _channel_history;
private mapping _channel_ids;

void create() {
   _listeners = ({ });
   _channels = ([ ]);
   _channel_ids = ([ ]);
   _channel_history = ([ ]);
   load_me();
} /* create() */

/**
 * This method returns the square distance, the name of the direction
 * the sound comes from and the up down offset.  It is used by the shout
 * code and by various other things that require this inofmraiont.
 * The return array is formated as:<br>
 * ({ square_distance, name_of_direction, z_offset })
 * @param co_ord1 the first co-ordinate
 * @param co_ord2 the co-ordinate to compare against
 * @return the array as described above
 */
mixed *get_direc_dist( int *co_ord1, int *co_ord2 ) {
   int dx, dy, dz, sector;

   if (!pointerp(co_ord1) ||
       !pointerp(co_ord2)) {
      // Make it right here...
      return ({ 0, DIR_ARRAY[0], 0 });
   }

   dx = co_ord1[ 0 ] - co_ord2[ 0 ];
   dy = co_ord1[ 1 ] - co_ord2[ 1 ];
   dz = co_ord1[ 2 ] - co_ord2[ 2 ];
   if ( dx > 0 ) {
      if ( ( 1000 * dy ) > ( 2414 * dx ) ) {
         sector = 0;
      } else {
         if ( ( 1000 * dy ) > ( 414 * dx ) ) {
            sector = 1;
         } else {
            if ( ( 1000 * dy ) > ( -414 * dx ) ) {
               sector = 2;
            } else {
               if ( ( 1000 * dy ) > ( -2414 * dx ) ) {
                  sector = 3;
               } else {
                  sector = 4;
               }
            }
         }
      }
   } else {
      if ( ( 1000 * dy ) < ( 2414 * dx ) ) {
         sector = 4;
      } else {
         if ( ( 1000 * dy ) < ( 414 * dx ) ) {
            sector = 5;
         } else {
            if ( ( 1000 * dy ) < ( -414 * dx ) ) {
               sector = 6;
            } else {
               if ( ( 1000 * dy ) < ( -2414 * dx ) ) {
                  sector = 7;
               } else {
                  sector = 8;
               }
            }
         }
      }
   }
   return ({ dx * dx + dy * dy + dz * dz, DIR_ARRAY[ sector ], dz });
} /* get_direc_dist() */

/**
 * Sends an event out to all the things.  It checks to make sure they
 * can hear the event and all that stuff.
 *
 * @param things things that get told about it 
 * @param centre coords where event occurs 
 * @param message what the things get told 
 * @param range range of telling in room units 
 * @param inside tell things if indoors 
 * @param underwater tell things if underwater 
 */
void broadcast_event( object *things, int *centre, string message,
      int range, int inside, int underwater ) {
   int *to;
   string text;
   object thing, place;
   mixed *dir_direc;

   if ( !pointerp( things ) ||
        !pointerp( centre ) ||
        sizeof( centre ) != 3 ) {
      return;
   }
   foreach ( thing in things ) {
      if ( !living( thing ) ) {
         continue;
      }
      /* Maybe something to inform non-living things later */
      place = environment( thing );
      if ( !place ) {
         continue;
      }
      to = (int *)place->query_co_ord();
      if ( !pointerp( to ) || sizeof( to ) != 3 ) {
         continue;
      }
      switch ( (string)place->query_property( "location" ) ) {
         case "inside" :
            if ( !inside ) {
               continue;
            }
            break;
         case "underwater" :
            if ( !underwater ) {
               continue;
            }
            break;
         default :
      }
      dir_direc = get_direc_dist( centre, to );
      if ( dir_direc[ 0 ] < 0 ) {
         /* So far away it's hit two's complement. */
         continue;
      }
      /* Lo, no need for square roots. */
      if ( dir_direc[ 0 ] > range * range ) {
         continue;
      }
      // This code is huge and fluffy, need to reset the cost in here a
      // little.
      //reset_eval_cost();
      switch ( dir_direc[ 0 ] ) {
         case 0 :
            /* They're in the same room - they should get told elsewhere. */
            continue;
         case 1 .. 2500 :
            /* Very close: 1 to 50 units */
            if ( ( dir_direc[ 2 ] * dir_direc[ 2 ] ) > ( dir_direc[ 0 ] / 2 ) ) {
               if ( dir_direc[ 2 ] > 0 ) {
                  text = "Right above you, "+ message +"\n";
               } else {
                  text = "Right below you, "+ message +"\n";
               }
            } else {
               text = "Very close to the "+ dir_direc[ 1 ] + ", "+
                     message +"\n";
            }
            break;
         case 2501 .. 40000 :
            /* Nearby: 50 to 200 units */
            text = "Nearby to the "+ dir_direc[ 1 ] + ", "+ message +"\n";
            break;
         case 40001 .. 640000 :
            /* 200 to 800 units */
            text = "To the "+ dir_direc[ 1 ] + ", "+ message +"\n";
            break;
         case 640001 .. 10240000 :
            /* In the distance: 800 to 1600 units */
            text = "In the distance to the "+ dir_direc[ 1 ] + ", "+
                  message +"\n";
            break;
         default:
            /* Too far away */
            continue;
      }
      if(interactive(thing) && query_verb() == "shout") {
        text = thing->colour_event("shout", "") + text + "%^RESET%^";
      }
      thing->add_message( "$I$5="+ text, ({ }) );
   }
} /* broadcast_event() */

/**
 * Adds an NPC to the list of NPC's who are to receive shout events.
 * The NPC need not be removed from this list when he dies -- desting
 * the NPC object has the same effect as calling npc_unhear_shouts().
 * event_person_shout() is activated on the NPC for all shouts.
 * NPC's do not normally detect shouts.
 * event_person_shout() on the NPC should be replaced with similar code 
 * to that in the broadcaster to determine if the shouter is within 
 * range and to generate the desired response.
 *
 * @param newlistener NPC object to be added to the list
 *
 * @see /global/events->event_person_shout()
 * @see npc_unhear_shouts()
 */
void npc_hear_shouts(object newlistener) {
  int i;

  if (member_array(newlistener,_listeners)!=-1) {
     return;
  }
  i = member_array(0,_listeners);
  if (i != -1) {
    _listeners[i]=newlistener;
  } else {
    _listeners=_listeners+({newlistener});
  }
} /* npc_hear_shouts() */

/**
 * Removes an NPC from the list of NPC's who are to receive shout events.
 * The NPC need not be removed from this list when he dies -- desting
 * the NPC object has the same effect as calling npc_unhear_shouts().
 *
 * @param oldlistener NPC object to be removed from the list
 * @return 1 if successfully removed
 *
 * @see /global/events->event_person_shout()
 * @see npc_hear_shouts()
 */
int npc_unhear_shouts(object oldlistener) {
  int i;

  if (sizeof(_listeners)==0) {
    return 0;
  }

  i = member_array(oldlistener,_listeners);
  if (i == -1) {
    return 0;
  }

  // Remove the listener
  _listeners = _listeners[0..i] + _listeners[i+1..];
  return 1;
} /* npc_unhear_shouts() */

/**
 * This method is called by the shout command to filter the shouts onto
 * the npcs that are listening.
 * @param shouter the person who shouted
 * @param start the start of the message
 * @param message the message shouted
 * @param lang the language the message is in
 * @param coord the co-ordinate it was shouted from
 * @param range the range of the shout
 */
void npc_shout_event( object shouter, string start, string message,
      string lang, int *coord, int range ) {
  
  if (_listeners) {
    _listeners -= ({ 0 });
    _listeners->event_person_shout(shouter, start, message,
                                  lang, coord, range );
  }
} /* npc_shout_event() */

/**
 * This method adds an object to the list of objects to be told about the
 * specfied channel.  The method called on the object for the channel
 * will be event_channel_message.  The method will be called with
 * three arguements, the first is the object generating the event
 * the second is the channel the event is generated on and the
 * third is the message being sent to the channel.
 * @param channel_name the name of the channel
 * @param ob the object to add to the list
 */
void add_object_to_channel(string channel_name,
                           object ob) {
   if (!_channels[channel_name]) {
      _channels[channel_name] = ({ });
   }
   _channels[channel_name] += ({ ob });
} /* add_object_to_channel() */

/**
 * This method removes an object from the list of objects to be told about the
 * specified channel.
 * @param channel_name the name of the channel
 * @param ob the object to add to the channel
 */
int remove_object_from_channel(string channel_name,
                               object ob) {
   if (_channels[channel_name]) {
      if (member_array(ob, _channels[channel_name]) != -1) {
         _channels[channel_name] -= ({ ob });
         if (!sizeof(_channels[channel_name])) {
           map_delete(_channels, channel_name);
         }
         return 1;
      }
   }
   return 0;
} /* remove_object_from_channel() */

/**
 * Inform channel of message.  The message will get sent to all the objects
 * added to the list to the method event_channel_message.  The method
 * will be called with three arguements, first is the object that started
 * the event, the second the channel it is being send to and the last is the
 * message being sent to the channel.
 * @param ob the object creating the channel event
 * @param channel the channel to inform people about
 * @param message the message to tell the channel about
 */
void broadcast_to_channel(mixed ob,
                          string channel,
                          mixed message) {
   string str;

   if (!ob) {
      return ;
   }
   if (objectp(ob)) {
      str = ob->query_cap_name();
   }
   if (_channels[channel]) {
      _channels[channel] -= ({ 0 });
      _channels[channel]->event_channel_message(ob, channel, message);

      // Put the message into the history list.
      if (!_channel_history[channel]) {
         _channel_history[channel] = ({ });
      }
      _channel_history[channel] += ({ ({ str, message, time() }) });
      if (sizeof(_channel_history[channel]) > BROADCAST_HISTORY_LENGTH) {
         _channel_history[channel] = _channel_history[channel][1..];
      }
   }
} /* broadcast_to_channel() */

/**
 * This message returns the current history list for the channel.  The
 * array is:<br>
 * ({ ({ person, message }), ... })
 * @param channel the channel to get the history of
 * @return the history of the channel
 * @see broadcast_to_channel()
 */
mixed *query_channel_history(string channel) {
   return _channel_history[channel];
} /* query_channel_history() */

/**
 * This method must only be used for debugging purposes.
 * @param channel the channel which has all these things on it
 * @return the array of objects in the channel
 * @see is_channel()
 */
object *query_channel_members(string channel) {
   return _channels[channel];
} /* query_channel_members() */

/**
 * This method checks to see if the channel exists.
 * @param channel the channel name to check for
 * @return 1 if the channel eixsts, 0 if not
 * @see query_channel_members()
 */
int is_channel(string channel) {
  return !undefinedp(_channels[channel]);
} /* is_channel() */

/** @ignore yes.  THis is definately definately not allowed to be used
 * in code, it is only allow to be used specifically for debugging
 * purposes.
 * @return the nice mapping
 */
mapping query_all_channels() {
   return _channels;
} /* query_all_channels() */

/**
 * This method keeps track of an ever increasing number for a specified
 * channel type.  THis is used in the wizards spell (for instance) to
 * keep track of unique ids for wizard channels.
 * @param channel the name of the channel
 * @return the next number in the series
 */
int query_next_channel_number(string channel) {
   int next_num;

   next_num = ++_channel_ids[channel];
   save_me();
   return next_num;
} /* query_next_channel_number() */

private void save_me() {
   unguarded( (: save_object(SAVE_FILE) :) );
} /* save_me() */

private void load_me() {
   unguarded( (: restore_object(SAVE_FILE, 1) :) );
} /* load_me() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   return ([ "channels" : _channels,
       "history" : _channel_history ]);
} /* query_dynamic_auto_load() */

void init_dynamic_arg(mapping arg) {
   _channels = arg["channels"];
   if (!_channels) {
      _channels = ([ ]);
   }
   _channel_history = arg["history"];
   if (!_channel_history) {
      _channel_history = ([ ]);
   }
} /* init_dynamic_arg() */
