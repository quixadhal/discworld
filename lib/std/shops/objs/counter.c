/**
 * Pub counter for the pub_shop.c inheritable.  It will automatically clear
 * away any empty items on it that have the property "pub item" on them every
 * 3 seconds.
 * @author Lemming
 * @started 28/10/1999
 */

#include <position.h>

inherit "/std/surface";

/** @ignore */
void create() {
   ::create();
   set_name( "counter" );
   set_short( "counter" );
   set_long( "The counter seems to be where you should leave glasses and "
      "plates to be cleared away.\n" );
   reset_get();

   add_property(CAN_POSITION_PROPERTY, 1);

} /* create() */

/** @ignore */
void clear_item( object item ) {
   if( environment( item ) == this_object()
       && !sizeof( all_inventory( item ) ) )
      item->move( "/room/rubbish" );
} /* clear_item() */

/** @ignore */
void event_enter( object ob, string mess, object from ) {
   if( ob->query_property( "pub item" )
       && !sizeof( all_inventory( ob ) ) )
      call_out( "clear_item", 3, ob );

} /* event_enter() */
