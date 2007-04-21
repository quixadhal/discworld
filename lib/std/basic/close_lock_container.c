/**
 * This file should be used in containers that wish to notify the
 * above container that they are able to be registered or not.
 * The function we_moved(object from, object to) should be called
 * to allow the object to add and remove itself from the correct
 * places.
 * <p>
 * We only worry about the open/close state, if something is open and
 * locked we don't care.  (I guess stuff can be locked open
 * anyway).
 * @author Pinkfish
 * @started Thu Mar  5 16:20:10 CST 1998
 * @see we_moved()
 */
inherit "/std/basic/close_lock";
 
/** @ignore yes */
void set_closed() {
   ::set_closed();
   if (environment()) {
      environment()->remove_inventory_container(this_object());
   }
} /* set_closed() */

/** @ignore yes */
void reset_closed() {
   ::set_closed();
   if (environment()) {
      environment()->add_inventory_container(this_object());
   }
} /* reset_closed() */

/**
 * This method should be called when an object is moved.  This will
 * remove itself from the leaving container and add itself to the
 * new one.
 * @param from the object we came from
 * @param to the object we are going to
 */
void we_moved(object from, object to) {
   if (!query_closed()) {
      if (from) {
         from->remove_inventory_container(this_object());
      }
      if (to) {
         to->add_inventory_container(this_object());
      }
   }
} /* we_moved() */

/** @ignore yes */
void dest_me() {
   /* Please note, this is not designed to kill the object... */
   if (environment()) {
      environment()->remove_inventory_container(this_object());
   }
} /* dest_me() */
