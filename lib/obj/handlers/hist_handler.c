/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: hist_handler.c,v 1.5 2001/08/01 23:38:00 ceres Exp $
 * 
*/
/**
 * The channel history handler. by Turrican@Discworld.
 * Based on the tell history.
 * @started 23-3-1995
 * @change 20-6-1995, Turrican
 * Added the lord channel
 * @change 13-12-1995, Turrican
 * Added chat line 1 
 * @change 28-5-1996, Turrican
 * Rewritten somewhat
 * @change 13-9-1998
 * Modified to remove ancient histories on temporary channels.
 */

#define SAVE "/save/hist_handler.o"
#define MAX_HIS 20
#define HISTORY_TIME (3600*24*2)

mapping history;

mixed *query_lord_history();
void add_lord_history( string start, string mess );

void create() {
  seteuid(master()->get_root_uid());
  history = ([]);
  if(!unguarded((: restore_object, SAVE+".gz" :)))
    unguarded((: restore_object, SAVE :));
  call_out("tidy_history", 2);
} /* create() */

/** Save the current state. */
void save_me() {
  unguarded((: save_object, SAVE, 2 :));
} /* save_me() */

/**
 * Add the history of the creator channel.
 * @param start the start bit, emote or not
 * @param mess the message they said
 */
void add_cre_history(string start, string mess) {
  mixed *cre_history = history["cre"];

  if (undefinedp(history["cre"]))
    cre_history = ({ ({ start, mess, time() }) });
  else
    cre_history += ({ ({ start, mess, time() }) });
  if (sizeof(cre_history) > MAX_HIS)
    cre_history = cre_history[1..];
  history["cre"] = cre_history;
} /* add_cre_history() */

/**
 * Returns the current creator history. The array contains arrays of
 * three elemnts each.,   In each array the first element is the
 * start, the second the mssage and thwe third the time.
 * <p>
 * <code>({  .. ({ start, mess, time }), ({ start, mess, time }), .. })</code>
 * @return an array of the creator history.
 */
mixed *query_cre_history() {
  return history["cre"];
} /* query_cre_history() */

/**
 * Keep a mesage from a specified line on the chat stick.
 * @param channel the channel being stored
 * @param start the start string (name)
 * @param mess the message being stored
 */
void add_chat_history(string channel, string start, string mess) {
  mixed *chat_history = history[channel];

  if ( !chat_history ) {
    chat_history = ({ });
  }
  if (channel == "lord")   {
    add_lord_history( start, mess );
    return;
  } else {
    chat_history += ({ ({ start, mess, time() }) });
  }
  if (sizeof(chat_history) > MAX_HIS) {
    chat_history = chat_history[1..];
  }
  history[channel] = chat_history;
} /* add_chat_history() */

/**
 * Returns the current chat channel history. The array contains arrays of
 * three elemnts each.,   In each array the first element is the
 * start, the second the mssage and thwe third the time.
 * <p>
 * <code>({  .. ({ start, mess, time }), ({ start, mess, time }), .. })</code>
 * @return an array of the chat channel history.
 * @param channel the channel to get the history of
 */
mixed *query_chat_history(string channel) {
  if (channel == "lord")
    return query_lord_history();
  return history[channel];
} /* query_chat_history() */

/**
 * Adds a amessage into the lord channel history.
 * @param start the start staring
 * @param mess what they actually said
 */
void add_lord_history(string start, string mess) {
  mixed *lord_history = history["lord"];

  if (!master()->query_lord(previous_object(-1)))
    return;
  if (undefinedp(history["lord"]))
    lord_history = ({ ({ start, mess, time() }) });
  else
    lord_history += ({ ({ start, mess, time() }) });
  if (sizeof(lord_history) > MAX_HIS)
    lord_history = lord_history[1..];
  history["lord"] = lord_history;
} /* add_lord_history() */

/**
 * Returns the current lord history. The array contains arrays of
 * three elemnts each.,   In each array the first element is the
 * start, the second the mssage and thwe third the time.
 * <p>
 * <code>({  .. ({ start, mess, time }), ({ start, mess, time }), .. })</code>
 * @return an array of the lord history.
 */           
mixed *query_lord_history() {
  if (!master()->query_lord(previous_object(-1)))
    return ({ });
  return history["lord"];
}  /* query_lord_history() */

/** @ignore yes */
void dest_me() {
  save_me();
  destruct(this_object());
} /* dest_me() */

/** @ignore yes */
void reset() {
  save_me();
}

/**
 * @ignore yes
 * Clean up old history items.
 */
void tidy_history() {
  string channel;
  int i;
  
  foreach(channel in keys(history)) {
    for(i = 0; i<sizeof(history[channel]); i++)
      if(history[channel][i][2] < time() - HISTORY_TIME) {
  history[channel] = history[channel][1..];
      } else
  break;
    if(!sizeof(history[channel]))
      map_delete(history, channel);
  }
  save_object(SAVE, 2);
}

/**
 * @ignore yes
 * Zap all the history channels back to nothing.
 */
void reset_history() {
  if (!master()->high_programmer(previous_object(-1)))
    return;
  history = ([]);
  save_object(SAVE, 2);
} /* reset_history() */
