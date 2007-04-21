/**
 * This is used to repeat all events onto the object that requested it
 * (excluding events from the object itself).  The methods it calls on
 * the repeater object are:
 * <ul>
 * <li>repeat_event_person_say
 * <li>repeat_event_emote
 * <li>repeat_event_say
 * <li>repeat_event_enter
 * <li>repeat_event_exit
 * </ul>
 * @author Pinkfish
 * @started Thu Sep 23 19:09:49 PDT 1999
 */

private nosave object _repeater;

/**
 * This method sets the owner of the repeater.
 * @param owner the object that owns the repeater
 */
void set_repeater_owner(object owner) {
   _repeater = owner;
} /* set_repeater_owner() */

/**
 * This method returns the owner of the repeater.
 * @return the object to inform of all events
 */
object query_repeater_owner() {
   return _repeater;
} /* query_repeater_owner() */

/*
 * Stuff to relay the messages from the environment to the person on the
 * horse and vica versa.
 */

/**
 * This is called on the repeater object to process the actual message
 * event_person_say message.
 * @param caller the repeater who sent the message
 * @param ob the object that did the say
 * @param start the start string
 * @param mess the message string
 * @param lang the language the message is in
 */
void repeat_event_person_say(object caller,
                             object ob,
                             string start,
                             string mess,
                             string lang) {
} /* repeat_event_person_say() */

/** @ignore yes */
void event_person_say(object ob, string start, string mess, string lang) {
   if (_repeater) {
      _repeater->repeat_event_person_say(this_object(), 
                                         ob,
                                         start, 
                                         mess, 
                                         lang);
   }
} /* event_person_say() */

/**
 * This is called on the repeater object to process the actual message
 * event_emote message.
 * @param caller the repeater who sent the message
 * @param ob the object that did the semote
 * @param mess the message string
 */
void repeat_event_emote(object caller,
                        object ob,
                        string mess) {
} /* repeat_event_emote() */

/** @ignore yes */
void event_emote( object ob, string mess ) {
   if (_repeater) {
   _repeater->repeat_event_emote(this_object(), 
                                 ob,
                                 mess);
   }
} /* event_emote() */

/**
 * This is called on the repeater object to process the actual message
 * event_say message.
 * @param caller the repeater who sent the message
 * @param ob the object that did the event_say
 * @param mess the message string
 * @param avoid the array of objects not to print the message
 */
void repeat_event_say(object caller,
                      object ob,
                      string mess,
                      object *avoid) {
} /* repeat_event_say() */

/** @ignore yes */
void event_say(object ob, string str, mixed avoid) {
   if (_repeater) {
      _repeater->repeat_event_say(this_object(), 
                                  ob,
                                  str,
                                  avoid);
   }
} /* event_say() */

/**
 * This is called on the repeater object to process the actual message
 * event_soul message.
 * @param caller the repeater who sent the message
 * @param ob the object that did the event_soul
 * @param mess the message string
 * @param avoid the array of objects not to print the message
 */
void repeat_event_soul(object caller,
                       object ob,
                       string mess,
                       object *avoid) {
} /* repeat_event_soul() */

/** @ignore yes */
void event_soul(object ob, string str, mixed avoid) {
   if (_repeater) {
      _repeater->repeat_event_soul(this_object(), 
                                  ob,
                                  str,
                                  avoid);
   }
} /* event_soul() */

/**
 * This is called on the repeater object to process the actual message
 * event_enter message.
 * @param caller the repeater who sent the message
 * @param ob the object that did the event_say
 * @param mess the message string
 * @param prev the last environment of the object
 */
void repeat_event_enter(object caller,
                        object ob,
                        string mess,
                        object prev) {
} /* repeat_event_enter() */

/** @ignore yes */
void event_enter( object thing, 
                  string mess, 
                  object prev ) {
   if (_repeater) {
      _repeater->repeat_event_enter(this_object(), 
                                    thing,
                                    mess,
                                    prev);
   }
} /* event_enter() */

/**
 * This is called on the repeater object to process the actual message
 * event_exit message.
 * @param caller the repeater who sent the message
 * @param ob the object that did the event_say
 * @param mess the message string
 * @param exit where the object is going to
 */
void repeat_event_exit(object caller,
                       object ob,
                       string mess,
                       object dest) {
} /* repeat_event_exit() */

/** @ignore yes */
void event_exit( object thing, 
                 string mess, 
                 object dest ) {
   if (_repeater) {
      _repeater->repeat_event_enter(this_object(), 
                                    thing,
                                    mess,
                                    dest);
   }
} /* event_exit() */
