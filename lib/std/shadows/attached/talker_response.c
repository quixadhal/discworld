/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: talker_response.c,v 1.4 2002/03/16 04:14:50 wobin Exp $
 *
 *
 */

/**
 * NPC talker response shadow 
 * @author Wobin
 * @started 22/05/01
 * This effect/shadow pair is to be used for NPCs with whom you wish to
 * add interactivity on the talker. It is formatted much like 
 * add_respond_to_with, but with a few alterations. The e/s is placed on
 * the listeners talker item with the NPC itself as the argument. I haven't
 * made it a requirement for the argument NPC to be the holder of the 
 * talker, but it makes more sense to use it that way. (Note, you can
 * only use NPCs as arguments, due to technical difficulties involved 
 * with new_parser.
 *
 * @example
 *     listener called Frog who is holding a talkerised cabbage and
 *     listener called Womble who is holding a talkerised electric shaver
 *
 *     cabbage->add_effect( EFFECTS + "talker_response", frog );
 *     cabbage->add_respond_to_talker_with( "one", "Womble", 
 *               ({ "lost", "brooch" }),
 *               ({ "one Haha!", "one You ditsy gnoll you!" }), 
 *               50 );
 *
 *     shaver->add_effect( EFFECTS + "talker_response", womble );
 *     shaver->add_respond_to_talker_with("one", "Frog",
 *               ({ "gnoll" }), 
 *               ({ "one That's it! I'm telling Pinkfish!" }) );
 *
 *     will have the two of these listener's chatting at one another, with 
 *     the frog responding half of the time to the womble's possible chat 
 *     of a lost brooch, and the womble reacting 1/10 times she is insulted.
 */

class response {
    string  sender;
    string *triggers;
    string *responses;
    int     probability;
    int     delay;
}

inherit "/std/effect_shadow";

public          object  query_listener      ( );
public  varargs void    query_responses     ( string );
public  varargs void    add_respond_to_talker_with( string, string, 
                                                        string *, int, int );
public          int     delete_response     ( string, string, string * ); 
public          void    receive             ( string, string, string, 
                                                        object * );
public          void    process_response    ( string *, string * );
private varargs int     register_listener   ( object );
private         int     compare_arrays      ( mixed *, mixed *);

mapping responses = ([ ]);
object listener;

/**
 * Adds the NPC as the listener, or the arg of the effect as default.
 * Automatically added when add_respond_to_talker_with is called.
 * @param target listener who is to be doing the listening
 * @see add_respond_to_talker_with()
 */ 
private varargs int register_listener(object target) {
  if(objectp(target)) {
    listener = target;
  }
  else
    listener = this_object()->arg_of(this_object()->
                                effects_matching("object.talker.response")[0]);
  if(!objectp(listener))
    this_object()->delete_effect(this_object()->
                                effects_matching("object.talker.response")[0]);
  return 1;
  
} /* register_listener */

/**
 * Who is registered as the listener NPC
 * @return The listener who is listening
 */
public object query_listener() {
  return listener;
}

/**
 * The list of responses that have been entered in for this NPC
 * @param key Optional. The channel to list responses for
 * @return mapping of responses for either the specified key, or all of them
 */
public varargs void query_responses(string key) {
  if(stringp(key)) {
    class response *womble = responses[key];
    
    foreach(class response cabbage in womble) {
      printf("Channel: %s\n\tChatter: %s\n", key, cabbage->sender);
      
      printf("\tTrigger:\n");
      foreach(string trigger in cabbage->triggers)
        printf("\t\t\"%s\"\n",trigger);
      
      printf("\tResponse:\n");
      foreach(string resp in cabbage->responses)
        printf("\t\t\"%s\"\n", resp);
      
      printf("\tProbability:\t%d\n\tDelay:\t%d\n\n",
             cabbage->probability, cabbage->delay);
    }
    return;
  } else {
    foreach(string nextkey in keys(responses)) {
      query_responses(nextkey);
    }
    return;
  }
} /* query_responses */      

/**
 * Adds a trigger for a certain channel and chatter. If this chatter is 
 * found to chat whatever is in the trigger, it will respond accordingly
 * If you use "#function" for the response, the function listed will be 
 * passed the channel, sender and text of the chat as arguments.
 *
 * @param channel The channel to listen to
 * @param sender The person/listener to respond to or "anyone" to listen to all chats.
 * @param trigger The response to trigger off
 * @param response How to respond to the trigger
 * @param probability Optional - Number out of 100 for the probability of the chat happening. Defaults to 10%
 * @param delay Optional - Number of seconds to delay. Defaults to 1 second. Use -1 for instant returns.
 * @example given a talker item...
 *   item->add_respond_to_talker_with("one", "Wobin", 
 *        ({ "woof", "arf", "bow-wow"}), 
 *        ({ "one Down boy!", "one Good Dog!" }), 10, 2 );
 *   item->add_respond_to_talker_with("two", "Archana", ({ "fluff" }),
 *        ({ "two You're so fluffy, Archana", "#fluff_archana" }), 80, 4 );
 *   
 */
public varargs void add_respond_to_talker_with(string channel, string sender, 
                                               string *trigger,
                                               string *response, int probability,
                                               int delay ) {
  
  class response new_item; 
  
  if(!listener)
    register_listener();
  
  if (!channel || !sender || !trigger || !response ){
    listener->call_out("do_command", 3, "'Err. I haven't been setup "
                       "correctly in regards to my talker!");
    return;
  }

  channel = lower_case(channel);
  
  if (undefinedp(responses[channel]))
    responses[channel] = ({});
  else {
      foreach(class response item in responses[channel]) {
        if(item->sender == sender && item->triggers - trigger == ({}) &&
           item->responses - response == ({}) && item->probability == probability)
            return;
      }
  }

  if(!probability)
    probability = 10;
  if(!delay)
    delay = 1;
  if(delay == -1)
    delay = 0;
  if(probability > 100)
    probability = 100;
  
  new_item = new(class response, sender : sender, triggers : trigger, 
                 responses : response, probability : probability, delay : delay);
  
  responses[channel] += ({ new_item });
  
  return;
} /* add_respond_to_talker_with */


/**
 * Used to delete specific responses
 * @param channel The channel the response is on
 * @param sender The person the response is set to
 * @param triggers An array of strings exactly matching the response
 */
public int delete_response( string channel, string sender, string *triggers ) {
  if( sizeof(responses[channel]) != 
      sizeof(responses[channel]   = responses[channel] - 
             filter(filter(responses[channel], (: $1->sender == $(sender) :)), 
                    (: compare_arrays( $1->triggers, $(triggers) ) :)) ) )
    return 1;
  
  return 0;
  
}

/** @ignore */
public void receive(string channel, string sender, string text,
                    object *receivers) {
  class response response;
  
  if(!listener) {
    register_listener();
    return;
  }
  
  if(sizeof(responses) && listener->query_name() != lower_case(sender) ) {
    if(member_array(lower_case(channel), keys(responses)) > -1 &&
       responses[channel]) {
      foreach( response in responses[channel]) {
        if(response->sender == sender || 
           response->sender == "anyone" ) {
          foreach( string trigger in response->triggers ) {
            if(regexp(text, trigger)) {
              if(random(100) < response->probability) {
                call_out("process_response", response->delay, 
                         response->responses, 
                         ({ channel, sender, text }));
              }
              break;
            }
          }
        }
      }
    }
  } else {
    query_shadowing(this_object())->receive(channel, sender, text, receivers);
  }
} /* recieve() */

/** @ignore */
private int compare_arrays( mixed *ar1, mixed *ar2 ) {
  int i;
  if ( !ar1 && !ar2 )
    return 0;
  if ( sizeof( ar1 ) != sizeof( ar2 ) )
    return 1;
  i = sizeof( ar1 );
  while ( i-- ) {
    if ( ar1[ i ] != ar2[ i ] ) {
      return 1;
    }
  }
  return 0;
}

/** @ignore */    
public void process_response( string *commands, string *arguments) {
  if(!query_listener()) return;
  
  foreach(string response in commands) {
    if(response[0] == '#') {
      if(function_exists(response[1..], listener)) {
        call_other( listener, ({ response[1..] }) + arguments);
      }
    } else {
      listener->do_command(response);
    }
  }
}
