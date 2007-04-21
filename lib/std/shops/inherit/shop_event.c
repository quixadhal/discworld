/**
 * This inheritable allows broadcasting of shop events to whoever
 * wants to hear them.  The most useful case of this is broadcasting
 * to yourself so you can tell when shops do specific things.  The
 * shop event will always be called 'event_shop' + type.  For example
 * 'event_shop_failed_buy'.  Each specific
 * shop will setup different parameters to the shop event, except
 * the first three parameters will always be the event name,
 * shop object and the third is the player.
 * @author Pinkfish
 * @started Mon Oct 23 12:23:41 PDT 2000
 */

private nosave object* _events;

void create() {
   _events = ({ });
} /* create() */

/**
 * This method adds an object into the list to receive shop events.
 * @param observer the observer to add
 */
void add_shop_observer(object observer) {
   if (!_events) {
      _events = ({ });
   }
   if (member_array(observer, _events) == -1) {
      _events += ({ observer });
   }
} /* add_shop_observer() */

/**
 * This method removes an object from the list to recieve shop events.
 * @param observer the observer to remove
 */
void remove_shop_observer(object observer) {
   _events -= ({ observer });
} /* remove_shop_observer() */

/**
 * This method sends the event to all the people who have been set to
 * receive shop events.  The shop event function is called
 * event_shop(object shop, ...).
 * <p>
 * If this method returns 1 then the default messages are supressed.
 * @param type the type of the event
 * @param player the player in the event
 * @param args the arguements in the event
 * @return 1 to supress the default messages
 */
int broadcast_shop_event(string type, object player, mixed args ...) {
   debug_printf("Calling %O on %O\n", "event_shop_" + type, _events);
   if (sizeof(filter(call_other(_events, "event_shop_" + type, this_object(),
                                player, args ...),
                    (: $1 :)))) {
      return 1;
   }
   return 0;
} /* broadcast_shop_event() */
