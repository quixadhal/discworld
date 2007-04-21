/**
 * This file contains the control functions for exporting your inventory.
 * @author Pinkfish
 * @started Thu Mar  5 21:03:40 CST 1998
 */
 
nosave object *registered_containers;
nosave int can_export_inventory;
 
void create() {
   registered_containers = ({ });
} /* create() */
 
/**
 * This method adds a container to the list of registered containers.
 * A registered container will be used to enhance the inventory of
 * the current container.  This will only be used with find_match
 * inventory items.  The item *must* be contained inside this object
 * to be added.  When checking only visible added objects will have
 * their inventory added.
 * @param cont the new container to add
 * @see find_inv_match()
 * @see remove_inventory_container()
 */
void add_inventory_container(object cont) {
   if (member_array(cont, registered_containers) == -1 &&
       environment(cont) == this_object()) {
      registered_containers += ({ cont });
   }
} /* add_inventory_container() */
 
/**
 * This method removes a container to the list of registered containers.
 * A registered container will be used to enhance the inventory of
 * the current container.  This will only be used with find_match
 * inventory items.
 * @param cont the new container to add
 * @see find_inv_match()
 * @see add_inventory_container()
 */
void remove_inventory_container(object cont) {
  if(!registered_containers || !cont)
    return;
  registered_containers -= ({ cont });
} /* remove_inventory_container() */
 
/**
 * This method allows the objects inside it (which are registered) to
 * export their invetories as well.  You do not want to be able to do
 * this with player or living objects (for instance).
 * The default value for this is reset()
 * @see add_inventory_container()
 * @see query_can_export_inventory()
 * @see reset_can_export_inventory()
 */
void set_can_export_inventory() {
   can_export_inventory = 1;
} /* set_can_export_inventory() */
 
/**
 * This method disables the object from exporting its inventory.
 * @see set_can_export_inventory()
 * @see query_can_export_inventory()
 * @see add_inventory_container()
 */
void reset_can_export_inventory() {
   can_export_inventory = 0;
} /* reset_can_export_inventory() */
 
/**
 * This method returns if the container will export its inventory.
 * @see add_inventory_container()
 * @see query_can_export_inventory()
 * @see reset_can_export_inventory()
 * @see set_can_export_inventory()
 * @return 1 if the container does export its inventory, 0 if not
 */
int query_can_export_inventory() {
   return can_export_inventory;
} /* query_can_export_inventory() */
 
/**
 * This method is not designed to be called from outside
 * the object itself.  It returns the list of objects able to
 * be matched that are container in containers in the container.
 * The third parameter is the set of objects the container
 * must appear in to be used.  This has to be handled sperately
 * so thet the plural matchin does not return the ones deep
 * inside your containers.
 * @param words the words which were matched with
 * @param looker the person doing the matching
 * @param things the current list of things matched.
 */
protected object *find_expanded_inv_match( string words, object looker,
                                           object *things) {
   object *stuff;
   object bing;
 
   stuff = ({ });
   if (can_export_inventory) {
      foreach (bing in registered_containers) {
         if (member_array(bing, things) != -1) {
            stuff += bing->find_inv_match(words, looker);
         }
      }
   }
 
   return stuff;
} /* find_inv_match() */
