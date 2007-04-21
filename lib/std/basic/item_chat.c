/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: item_chat.c,v 1.9 2002/04/19 20:51:06 aquilo Exp $
 * $Log: item_chat.c,v $
 * Revision 1.9  2002/04/19 20:51:06  aquilo
 * Added call to check_chat() in item_chat(), as per Sousjagne's suggestion.
 *
 * Revision 1.8  2001/11/05 21:03:50  pinkfish
 * Fix up a weird error with interactive and environments.
 *
 * Revision 1.7  2001/08/20 16:23:54  aquilo
 * Fixed up saving & restoring of chats
 *
 * Revision 1.6  2001/08/18 14:43:11  aquilo
 * Made the changes work this time. (didn't take into account environment() == 0 first
 * time)
 *
 * Revision 1.3  2000/03/28 21:23:46  ceres
 * Can't remember
 *
 * Revision 1.2  1999/09/14 05:08:27  shaggy
 * Made documentation more complete
 *
 * Revision 1.1  1998/01/06 03:59:10  ceres
 * Initial revision
 * 
*/
/** -*- LPC -*-
 * This class handles atmospheric chat messages.
 */
#include <room.h>

private nosave int _off, _already_added;
private nosave mixed *_chats;

void check_chat();

/**
 * @return The raw data telling us what to chat and when
 */
mixed *query_chats() { return _chats; }


/** @ignore yes */
void init() {
  check_chat();
} /* init() */


/**
 * This method adds atmospheric chat messages to the object.
 * The array which is passed into the object has three 
 * elements, the first element is the minimum time between chats, the second
 * parameter is the maximum time bewteen chats and the third parameter
 * is the list of actual chats.
 * <p>
 * Item chats are strings which are printed at (semi) random
 * intervals to living objects holding or in the same room as
 * someone holding the object. Currently they will not be
 * heard outside containers. They are used to add
 * atmosphere to an Item. A chat will be picked at random
 * from the array of chats with a frequency controlled by the
 * times min and max. ie. one will be picked every n seconds
 * where is varies between min and max seconds. Please
 * don't make the values for min and max too small or the
 * messages just become annoying! 
 * <p>
 * Currently, there's a maximum on the min/max values, every
 * value higher than 320 seconds is treated as that.
 * <p>
 * The chats can contain one of $a_short$, $the_short$ and
 * $poss_short$ which will be replaced with the
 * corresponding value. 
 * <p>
 * To call a function defined on the item in place of a chat message
 * use "#function_name" in place of a chat string.
 * <p>
 * To use item chats, the object needs to inherit
 * "/std/basic/item_chat" and if the object defines an init()
 * function, that function should also call ::init()
 * <p>
 * To make the chatting object save the values, it's necessary
 * to define a couple of supporting functions in the item: 
 * <pre>
 * mapping query_dynamic_auto_load() {
 *   mapping tmp;
 * 
 *   tmp = ([ "::" : object::query_dynamic_auto_load(),
 *            "chat" : item_chat::query_dynamic_auto_load(), ]);
 *   return tmp;
 * } \/\* query_dynamic_auto_load() *\/
 *
 * void init_dynamic_arg(mapping map) {
 *   if (map["::"])
 *     object::init_dynamic_arg(map["::"]);
 *   if (map["chat"])
 *     item_chat::init_dynamic_arg(map["chat"]);
 * } /\* init_dynamic_arg() *\/
 *
 * </pre>
 * Use object in the previous if the object is inheriting /std/object, if not,
 * then use whatever it's inheriting from instead. 
 * @example item_chat( ({ 120, 240,
 *            ({ "A revolting smell drifts from $the_short$.",
 *              "Something rustles in $the_short$." }) }) );
 */
void item_chat( mixed *args ) {
  if ( !args || sizeof(args) < 3 || !sizeof(args[2]) ){
    write( "Incorrect args to item_chat.\n" );
    return;
  }
 
  /* We save original version, not replaced one, as the a_short stuff will point
     to wrong obs when we next load */
//  _saved_version = args;
  args = args - ({ 0 });  
  args[2] = map( args[2], 
        (: replace( $1, ({ "$a_short$", this_object()->a_short(),
                           "$the_short$", this_object()->the_short(),
                           "$poss_short$", this_object()->poss_short() }) ) :) );

  _off = 0;
  _chats = args + ({ 0 });

  check_chat();

} /* item_chat() */

/**
 * This starts the object chatting.  This is done automaticly when entering
 * the presence of an interactive object
 * @see item_chat()
 * @see chat_off()
 */
void chat_on(){
   _off = 0; 
   check_chat();
} /* chat_on() */

/**
 * This stops the object chatting.  This is done automaticly when leaving
 * the prescence of all interactive objects.
 * @see chat_on()
 * @see item_chat()
 */
void chat_off(){
   _off = 1; 
} /* chat_off() */

/**
 * This method does the actual chat, it prints a message
 * and figures that stuff out.
 * @see item_chat()
 * @see chat_on()
 * @see chat_off()
 */
void make_chat(){
  int saying;
  string stuff;
  object env, *targets;

  _already_added = 0;

  if( !(env = environment()) || _off )
    return;

  saying = random( sizeof(_chats[ 2 ]) );

  if( saying == _chats[ 3 ] )
    saying = ( saying + 1 ) % sizeof( _chats[ 2 ] );

  _chats[ 3 ] = saying;
  stuff = _chats[ 2 ][ saying ];

  // Chat if i'm in a room, or in an environment one branch down from an (n)pc 
  // who is in a room

  if( env->query_is_room() || 
      ( interactive(env) && (env = environment(env)) && env->query_is_room() ) ){
    targets = filter( all_inventory(env), (: interactive($1) :) );
    if( !sizeof(targets) ) 
      // No one here. Don't chat and don't add ourself to chatter
      return;

    if( stuff[0..0] == "#" )
      call_other( this_object(), stuff[ 1 .. ] );
    else
      tell_room( env, stuff + "\n" );

    // Add ourselves, so we are called again
    _already_added = 1;
    ROOM_HANDLER->add_chatter( this_object(), _chats[ 0 ] +
                               random( _chats[ 1 ] - _chats[ 0 ] + 1 ) );
  }

} /* make_chat() */

/**
 * This method makes sure that we are chatting if we should be.
 * @see chat_on()
 * @see item_chat()
 * @see make_chat()
 */
void check_chat(){
  if( !_off && !_already_added && arrayp(_chats) && environment() ){
    if( interactive(environment()) || 
          ( environment(environment()) &&
            interactive(environment(environment()))
          ) 
       ){
           _already_added = 1;
           ROOM_HANDLER->add_chatter( this_object(), 
           ( _chats[ 0 ] + random( _chats[ 1 ] - _chats[ 0 ] + 1 ) ) / 2 );
        }
  }

} /* check_chat() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
   return ([ "_off"  : _off,
            "_chats" : _chats
          ]);
} /* query_dynamic_auto_load() */

/** Fixes saved chats
 */
string fix_saved_chat( string chat ){
  string old_ob;

  if( !chat )
    return 0;
  sscanf( chat, "%*s$%*s_short:%s$%*s", old_ob );
  if( old_ob )
    chat = replace( chat, old_ob, file_name(this_object()) );

  return chat;
}


/** @ignore yes */
void init_dynamic_arg( mapping map ) {
  mixed info;

  if( !map )
    return;
  if( (info = map["_chats"]) && arrayp(info) && sizeof(info) > 2 ){
    info[2] = map( info[2], "fix_saved_chat", this_object() );
    item_chat( info );
  }
  if( !(_off = map["_off"]) )
    chat_on();
  else
    chat_off();
} /* init_dynamic_arg() */

