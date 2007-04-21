/**
 * This is a inheritable that provides an efficient way of storing
 * lots of items. Only one copy of the item is effectively in storage
 * and everytime one is removed, an exact copy is duplicated and
 * put back in the store. Objects are added to the store via
 * add_object(). The actual storage container object
 * can be returned by query_cont() and this container should be
 * searched when you want to find what objects the store contains.
 * <p>
 * Any object can inherit this, and methods
 * should be put in place in the inheriting file that end up calling
 * create_real_object() which will sort out duplicating the item
 * and returning an object pointer to the one you can deal with.
 * @example
 * inherit "clone_on_demand";
 * int do_buy( objects *obs );
 *
 * void setup(){
 *   set_name("shop");
 *   set_short("widget shop");
 *   add_object( "sprocket" );
 * }
 *
 * object create_object( string arg ){
 *   if( arg == "sprocket" )
 *     return clone_object( "/path/of/sprocket" );
 * }
 *
 * void init(){
 *   add_command("buy", "<indirect:object:" +
 *                      base_name( query_cont() ) + ">");
 * }
 *
 * int do_buy( object *obs ){
 *   object ob;
 *   foreach(ob in obs){
 *     widget = create_real_object(ob);
 *     widget->move( this_player() );
 *   }
 *   add_succeeded_mess( "$N buy$s $I.\n", obs );
 *   return 1;
 * }
 *
 * @author Pinkfish Aquilo
 * @see add_object
 */

#include <armoury.h>

#define INFINITE_LEFT 10000
#define MAX_PROP "max per reset"
#define NUM_REMOVED "number removed this reset"
#define REFERENCE "name we are reference by"
#define DISPLAY "display in shop"

nosave object _cont;
nosave string _domain;

void create(){
}

/**
 * This method returns the container which is used to keep one copy of
 * each items in storage.
 * @return the object container
 */
object query_cont(){  return _cont;  }

/** @ignore yes */
void check_cont() {
  if (!_cont) {
     _cont = clone_object("/std/container");
     _cont->set_name( "clone_on_demand-store" );
  }
}

/** @ignore yes */
void reset() {
  object ob;
  check_cont();
  foreach( ob in all_inventory(_cont) ){
    ob->remove_property(NUM_REMOVED);
  }
}

/**
 * This method sets the domain the objects will be created from.
 * The default objects will always be matched as well.  So setting
 * this will allow objects from the default of the specified armoury
 * domain.
 * @param domain the domain to try and create from
 * @example
 * set_object_domain("cwc");
 */
void set_object_domain(string domain) {
   _domain = domain;
} /* set_object_domain() */

/**
 * This method returns the domain the objects will be created from.
 * @return the domain the objects are created from
 */
string query_object_domain() {
   return _domain;
} /* query_object_domain() */

/**
 * This method is used to add an item to the storage.
 * When this method is called, create_object() is called
 * (with the object name as an arg) in the inheriting file.
 * If no object is returned by that function,
 * the name is cloned with clone_object(), and failing that
 * request_item() is called in the armoury against the name.
 *
 * This method makes add_weapon() and add_armour() obsolete.
 *
 * @example
 * add_object( "frog", 1 + random( 3 ) );
 * // This will try and create an object called frog, in the order mentioned
 * // above
 * @example
 * add_object( "/obj/food/apple.food", 0 );
 * // Add unlimited numbers of apples.
 * @param name the name of the object to add.
 * @param max_per_reset the maximum number of items to be available at any one time
 * @param display_as the name that this item is displayed as in shops
 * @return 1 if the item was added successfully to the store, 0 if it was not.
 */
varargs int add_object( string name, int max_per_reset, string display_as ) {
  object ob;

  if(!_cont) {
     _cont = clone_object("/std/container");
     _cont->set_name( "clone_on_demand-store" );
  }
  if (!(ob = this_object()->create_object(name))) {
    if(!ob = clone_object(name)) {
      ob = ARMOURY->request_item(name, 100, _domain);
    }
  }

  if(ob) {
    if (ob->query_decays()) {
      ob->set_decay_speed(0); // Stop decaying objects decaying in inventories.
    }
    ob->move(_cont);

    if (display_as) {
         ob->add_property( DISPLAY, display_as);
    }

    ob->add_property( REFERENCE, name);
    ob->add_property( MAX_PROP, max_per_reset );
    return 1;
  }
  return 0;
}

/**
 * Returns how more times object ob can be duplicated
 * @param ob the object to test
 * @return how many more times
 */
int query_num_items_left(object ob) {
   int max;
   int num;

   max = ob->query_property( MAX_PROP );
   num = ob->query_property( NUM_REMOVED );
   if( max ){
     return max - num;
   }
   return INFINITE_LEFT;
}

/**
 * This function returns the quantity of particular object available
 * to be cloned on demand. In matching which object is the one in
 * question it uses the short name of the object, which is passed as
 * an argument to the function.
 * @param name is the short name of the object you wish to query.
 * @return the number left, returns INFINITE_LEFT if the shop has an infinite
 * number, returns -1 if the item isn't stocked.
 */
int query_number_left(string name) {
   object * things;

   things = filter( all_inventory(_cont),
     (: $1->query_short() == $(name) :) );

   if( !sizeof(things) ) {
      return -1; // we don't have that item.
   }
   return query_num_items_left( things[0] );
} /* query_number_left() */

/**
 * This function can be used to check the quantity left of an array
 * of items.  It returns a parallel array of integers.  In other words
 * the array it returns contains the numbers of stock in array positions
 * corresponding to the array positions of the objects it was passed.
 * @example
 * query_items_left( ({ "banana" , "melon" }) )
 * would return ({ 12 , 6 }) if there were 12 bananas and 6 melons left.
 * @param names an array of the short names of the items you wish to query
 * @return an array of integers, each one returning like query_number_left
 * would for the object in that position of the object array.
 * @see query_number_left
 */
int * query_items_left( string *names ) {
   if( !names )
      return ({ });
   return map( names, (: query_number_left($1) :) );
}

/**
 * This function transfers certain tracking properties from the
 * original item to the new item which replaces it in storage. To
 * transfer any additional properties, have switch_extra_properties()
 * defined in your inheriting object and return an array of extra
 * properties to transfer
 */
private void switch_properties(object newone, object original) {
  mixed *extra, *props, prop;

  props = ({ MAX_PROP, NUM_REMOVED, REFERENCE });

  extra = this_object()->switch_extra_properties();
  if( sizeof(extra) )
    props += extra;

  foreach( prop in props ){
    newone->add_property( prop, original->query_property( prop ) );
    original->remove_property( prop );
  }

  if (newone->query_decay_speed()) {
     original->set_decay_speed( newone->query_decay_speed() );
     newone->set_decay_speed(0); // Stop decaying objects decaying in inventories.
  }
}

/**
 * The main point of entry. 'thing' should be an object already placed
 * in the clone_on_demand store container via 'add_object'. This
 * method then duplicates that object, replaces the original copy in
 * the container with this new one, and returns the original which can
 * be delt with as normal.
 * @param thing an object in the store
 * @return the original object
 * @see add_object
 */
protected object create_real_object(object thing) {
   string name;
   object new_thing, *continv, temp_cont;
   int j, max, num;

   name = thing->query_property( REFERENCE );
   if(!name)
     return 0;
   
   new_thing = this_object()->create_object( name );
   if ( !new_thing )  {
      new_thing = clone_object(name);
   }
   if ( !new_thing )  {
      new_thing = ARMOURY->request_item( name, 80 + random( 20 ), _domain );
   }

  if( new_thing ){
    switch_properties( new_thing, thing );

    /* Restore contents order */
    continv = all_inventory(_cont);
    j = member_array( thing, continv );
//    new_thing->move("/room/void");
    temp_cont = clone_object("/std/container");
    thing->move("/room/void");
    continv[j] = new_thing;
    new_thing->move(_cont);

      for (j=sizeof(continv)-1;j>=0;j--) {
      continv[j]->move(temp_cont);
      continv[j]->move(_cont);
    }
    temp_cont->dest_me();

    /* Decrement store if applicable */
    max = new_thing->query_property( MAX_PROP );
    num = new_thing->query_property( NUM_REMOVED );
    if ( max ) {
      new_thing->add_property( NUM_REMOVED, num + 1 );
    }
  }
  return thing;
}

/** @ignore yes */
void dest_me(){
  if( _cont )
    _cont->dest_me();
}

/* Old functions, use add_object instead */

/* ps 0, means infinite */
/** @ignore yes */
varargs int add_armour( string name, int max_per_reset, string display_as ) {
  return add_object(name, max_per_reset, display_as);
} /* add_armour() */

/** @ignore yes */
varargs int add_weapon( string name, int max_per_reset, string display_as ) {
  return add_object(name, max_per_reset, display_as);
} /* add_weapon() */
