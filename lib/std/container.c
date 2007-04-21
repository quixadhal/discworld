/**
 * The basic container inheritable, please note this is *not* a container
 * like a bucket.  This now allows things inside the container to
 * inform us that they should be used as part of the inventory of this
 * object.  This means the contents of containers will look like they
 * are not inside containers to the outside world.
 * @author Pinkfish
 * @see /obj/container.c
 * @see /obj/baggage.c
 * @see /obj/vessel.c
 * @see /obj/clothing.c
 */
#include <move_failures.h>
#include <player.h>
#include <player_handler.h>

inherit "/std/object";
inherit "/std/basic/cute_look";
inherit "/std/basic/export_inventory";
inherit "/global/auto_load";

private nosave int _max_weight;
private nosave int _loc_weight;
private nosave int _max_items;
private nosave int _prevent_insert;
private nosave string _ownership;
private nosave object _player;
private nosave int _n_tracked_items;
private nosave int _tracking;

void create() {
  registered_containers = ({ });
  _n_tracked_items = 0;
  _tracking = 1;
  export_inventory::create();
  object::create();
} /* create() */

/**
 * This method returns the maximum number of items that can
 * be carried in this container.
 * @return the maximum number of items
 * @see set_max_items()
 */
int query_max_items() {
  if(_max_items)
    return _max_items;
  if(_max_weight)
    return 4 * sqrt(_max_weight);
  return -1;
}

/**
 * This method sets the maximum number of items that can
 * be carried in this container.
 * @param number the new maximum number of items
 * @see query_max_items()
 */
void set_max_items( int number ) { _max_items = number; }

/**
 * This method returns the maximum amount of weight that can
 * be carried in this container.
 * @return the maximum weight
 * @see set_max_weight()
 */
int query_max_weight() { return _max_weight; }

/**
 * This method sets the maximum amount of weight that can
 * be carried in this container.
 * @param number the new maximum weight
 * @see query_max_weight()
 */
void set_max_weight( int number ) { _max_weight = number; }

/**
 * This method returns the current local weight in this
 * container.
 * @return the local weight
 */
int query_loc_weight() { return _loc_weight; }

/**
 * This method determins the current local weight from all the
 * objects inside the container
 * @see query_loc_weight()
 */
void update_loc_weight() {
   object thing;

   _loc_weight = 0;
   foreach ( thing in all_inventory( this_object() ) )
      _loc_weight += (int)thing->query_complete_weight();
} /* update_loc_weight() */

/**
 * This method returns the complete weight of the object.  This is the
 * weight of the container itself, plus the weight of the things
 * inside it
 * @return the complete_weight()
 * @see /std/basic/misc->query_weight()
 * @see query_loc_weight()
 */
int query_complete_weight() {
   return ::query_complete_weight() + _loc_weight;
} /* query_complete_weight() */

/**
 * This method is called in the move functions, it adds extra
 * weight onto the object when something is moved inside it.
 * @return 1 if successfuly added
 * @param n the amount to add
 * @see /std/basic/misc->query_weight()
 */
int add_weight( int n ) {
#ifdef 0
  // This shouldn't be here should it? 
  if ( _prevent_insert )
    return 0;
#endif
  if ( !_max_weight ) {
    _loc_weight += n;
    return 1;
  }
  if ( n + _loc_weight > _max_weight )
    return 0;
  if ( !environment() ) {
    _loc_weight += n;
    return 1;
  }
  if ( !environment()->add_weight( n ) )
    return 0;
  _loc_weight += n;
  return 1;
} /* add_weight() */

/**
 * This returns who owns the container, if it set to 0 then no one
 * owns it.  This is used in the theft determination for the object.
 * @return who owns the container
 * @see set_ownership()
 */
string query_ownership() { return _ownership; }

/**
 * This sets who owns the container, if it set to 0 then no one
 * owns it.  This is used in the theft determination for the object.
 * @param word who owns the container
 * @see query_ownership()
 */
void set_ownership( string word ) {
  if( word )
    _ownership = lower_case( word );
  else
    _ownership = word;
}

/**
 * This method checks to see if the object can be taken out of
 * us.
 * @param thing the object coming out
 * @param flag the move flag
 * @param dest the destination object
 * @see /std/basic/move.c
 * @return 1 if it can be taken out, 0 if not.
 */
int test_remove( object thing, int flag, mixed dest ) {
  int player;
  string str;

  if( !_ownership || !this_player() ) {
    return 1;
  }

  if( objectp( dest ) ) {
    dest = file_name( dest );
  }

  if( dest == "/room/rubbish" || dest == "/room/vault" ) {
    return 1;
  }

  str = "Item " + file_name( this_object() ) + " accessed by " +
    this_player()->query_short() + " which belongs to $C$" + _ownership;

  // It's this_player's
  if( (string)this_player()->query_name() == _ownership ) {
    str += ".  Taking items, no theft event.";
    log_file( "/w/trilogy/CONTAINER", str + "\n" );
    return 1;
  }

  player = PLAYER_HANDLER->test_user( _ownership );
  // Owner is a player
  if( player ) {
    // PK check, even if player is offline
    str += ", who is a player.  ";
    if( !pk_check( this_player(), _ownership, 1 ) &&
      environment( this_player() ) ) {
      str += "PK check succeded: Taking items, theft event triggered.";
      this_player()->zap_harry_shadow();
      event( environment( this_player() ), "theft", this_player(),
        this_object(), ({ thing }) );
      log_file( "/w/trilogy/CONTAINER", str + "\n" );
      return 1;
    } else {
      // Cannot take stuff from NPK's containers
      str += "PK check failed: Cannot take items.";
      write( "An unseen force stays your hand.\n" );
      log_file( "/w/trilogy/CONTAINER", str + "\n" );
      return 0;
    }
  } else {
  // Owner not a player
    str += ", which is not a player.  Taking items, theft event triggered.";
    this_player()->zap_harry_shadow();
    event( environment( this_player() ), "theft", this_player(),
      this_object(), ({ thing }) );
    log_file( "/w/trilogy/CONTAINER", str + "\n" );
    return 1;
  }
} /* test_remove() */

/**
 * This method allows things to be added into us.  If we have an 
 * environment check that for the ability to add into us.
 * @param ob the object being added
 * @param flag the add flags
 * @see /std/basic/move.c
 */
int test_add(object ob, int flag) {

  if(!_max_weight && !_max_items)
    return 1;

  // Prevent larger containers being placed in smaller ones
  if(ob->query_max_weight() > _max_weight - _loc_weight) {
    return 0;
  }

  // Prevent longer/wider objects being placed in shorter/thinner ones.
  // We check for a length/width greater than 1 since many items don't
  // have length/width yet.
  if(this_object()->query_length() > 1 &&
     this_object()->query_length() < ob->query_length()) {
    return 0;
  }
  if(this_object()->query_width() > 1 &&
     this_object()->query_width() < ob->query_width()) {
    return 0;
  }

  // Check if it's truly a container. If it is then count its inventory,
  // if not don't.
  if(ob->query_max_weight())
    return ((sizeof(deep_inventory(this_object())) +
             sizeof(deep_inventory(ob))) < query_max_items());
  else
    return sizeof(deep_inventory(this_object())) < query_max_items();
}

/**
 * items21 stops this container from being put into other containers.
 * @see reset_prevent_insert()
 * @see query_prevent_insert()
 */
int set_prevent_insert() { _prevent_insert = 1; }
/**
 * This allows this container to be put into other containers (default).
 * @see set_prevent_insert()
 * @see query_prevent_insert()
 */
int reset_prevent_insert() { _prevent_insert = 0; }
/**
 * If this is true, this container cannot be put into other containers.
 * @see reset_prevent_insert()
 * @see set_prevent_insert()
 */
int query_prevent_insert() { return _prevent_insert; }

/** @ignore yes */
varargs int move(mixed dest, mixed messin, mixed messout) {
  if (_prevent_insert && _loc_weight && !living(dest) && environment(dest))
    return MOVE_INVALID_DEST;

  return object::move( dest, messin, messout );
} /* move() */

/**
 * This method finds the matching objects inside this object
 * that are visible to the looker.  This will also use the
 * registered containers and add them into the array returned
 * (if visible and contained in the object).
 * @see /secure/simul_efun->find_match()
 * @param words the words matched on
 * @param looker who is looking
 * @see add_inventory_container()
 */
object *find_inv_match( string words, object looker ) {
   object *things;

   things = all_inventory( this_object() );

   /* Remove the for loop and make this a touch faster. */
   things = filter(things, (: $1->short(0) &&
                              (!$2 || $1->query_visible($2)) :), looker);

   return things;
} /* find_inv_match() */

/**
 * This method handles the case where the return of the move flag is not
 * MOVE_OK.  This can do whatever we want to make it work in a more useful
 * fashion.
 */
int do_restore_inventory_error(object ob, int move_flag) {
   object receipt;
   int ret;

   receipt = clone_object(PLAYER_RECEIPT);
   receipt->setup_receipt(ob);
   // Just in case it was too heavy.
   receipt->set_weight(0);
   ret = receipt->move(this_object());
   if (ret != MOVE_OK) {
      // Bugger it.
      receipt->dest_me();
   } else {
      move_flag = MOVE_OK;
      ob->move("/room/rubbish");
   }
   return move_flag;
} /* do_restore_inventory_error() */

/**
 * This method handles moving objects into the inventory from an auto
 * load.  This should be over ridden by things inheriting us to
 * make sure that the objects can be moved into the inventory.  It
 * should handle the bypassing of open/close/locked etc flags.
 * @param ob the object to move into ourselves
 */
protected int handle_restore_inventory(object ob) {
  /* The standard container needs to do nothing special. */
  int move_flag;

  ob->disable_item_tracking();
  move_flag = ob->move(this_object());
  ob->enable_item_tracking();
  if (move_flag != MOVE_OK) {
     // Turn it into a receipt and pop it in there ourselves.
     move_flag = do_restore_inventory_error(ob, move_flag);
  }
  return move_flag;
} /* handle_restore_inventory() */

/** @ignore yes */
mixed stats() {
  return ::stats() + ({
    ({ "loc_weight", query_loc_weight(), }),
    ({ "max_weight", query_max_weight(), }),
    ({ "max_items", query_max_items(), }),
    ({ "export invent", query_can_export_inventory() }),
  });
} /* stats() */

/** @ignore yes */
mapping int_query_static_auto_load() {
  mapping tmp;
  tmp = ::int_query_static_auto_load();
  return ([
     "::" : tmp,
     "max weight" : _max_weight,
     "prevent insert" : _prevent_insert,
     "can export inventory" : query_can_export_inventory(),
  ]);
} /* int_query_static_auto_load() */

/** @ignore yes */
mapping query_dynamic_auto_load() {
  mixed inventory;

  // Create out auto load stuff.
  catch(inventory = create_auto_load( all_inventory( this_object() ), 0 ) );

  return ([
    "::" : object::query_dynamic_auto_load(),
    "inv" : inventory
  ]);
} /* query_dynamic_auto_load() */

/**
 * This method is used in the auto loading sequence to set the
 * player who is loading the container.
 * @param thing the player loading the container
 * @see query_player()
 */
void set_player( object thing ) {
   ::set_player(thing);
    _player = thing;
}

/**
 * This method returns the player who is loading the container
 * during the autoload sequence.
 * @return the player who loaded the object
 */
object query_player() { return _player; }

/**
 * This method enables item tracking
 * @see event_enter_tracked_item
 * @see disable_item_tracking
 */
nomask void enable_item_tracking() { _tracking = 1; }

/**
 * This method disables item tracking
 * @see event_enter_tracked_item
 * @see enable_item_tracking
 */
nomask void disable_item_tracking() { _tracking = 0; }

/**
 * This method handles the movement of the containers of this object.
 * Tracked items want to be notified if the container moves or anything
 * containing it does.  The movement of the outmost moving container is 
 * propagated down to all tracked items.
 * @param mover    The outermost container object that moved
 * @param from     The start environment of the outermost container 
 * @param to       The destination of the outermost container 
 * @see event_enter_tracked_item
 */
nomask void event_container_move( object mover, mixed from, mixed to ) {
  if (_n_tracked_items) 
    all_inventory()->event_container_move( mover, from, to );
}

/**
 * This method is called from move to notify any tracked items contained of
 * the move using event_container_moved.
 * @param from   start
 * @param to     destination
 */
void event_move_object( mixed from, mixed to ) {
  if (_n_tracked_items && _tracking && !interactive()) {
    all_inventory()->event_container_move( this_object(), from, to );
    if (objectp(from)) from->remove_tracked_items( _n_tracked_items );   
    if (objectp( to )) to->add_tracked_items( _n_tracked_items );   
  }
}

/** 
 * This method is called to adjust the status reason.
 * occurences.  
 * @see event_buried 
 * @see event_player_quit 
 * @see event_enter_tracked_item
 */
nomask void set_tracked_item_status_reason(string reason) {
  if (_n_tracked_items) 
    all_inventory()->set_tracked_item_status( reason );
}

/**
 * This method handles the addition of tracked items.
 * Tracked items want to be notified if the container moves.
 * @param n_items the number of tracked items added
 * @see remove_tracked_items
 * @see event_container_move
 */
nomask void add_tracked_items( int n_items ) {
   _n_tracked_items += n_items;
   if (environment()) environment()->add_tracked_items( n_items );
}

/**
 * This method handles the removal of tracked items.
 * Tracked items want to be notified if the container moves.
 * @param n_items the number of tracked items  removed
 * @see add_tracked_items
 * @see event_container_move
 */
nomask void remove_tracked_items( int n_items ) {
  _n_tracked_items -= n_items;
  if (environment()) environment()->remove_tracked_items( n_items );
}

/**
 * This method returns the number of tracked item contained.
 * Tracked items want to be notified if the container moves.
 * @see add_tracked_items
 * @see event_container_move
 */
nomask int query_tracked_items() {
  return _n_tracked_items;
}

/**
 * This method allows the container to have stuff inside it checked.
 * @param looker the person doing the checking
 * @return 1 on success, 0 on failur
 */
int can_find_match_recurse_into(object looker) {
   object env;

   //
   // If the looked is one of our environments, then yes! they can.
   //
   env = environment();
   while (env &&
          !living(env) &&
          env != looker &&
          env != environment(looker)) {
      env = environment(looker);
   }
   return env == looker || env == environment(looker);
} /* can_find_match_recurse_into() */

/**
 * This method checks to see if the find match code can actually
 * reference this object inside us.
 * @param thing the thing to reference
 * @param looker the person looking at it
 * @return 1 if they can, 0 if they cannot
 */
int can_find_match_reference_inside_object(object thing, object looker) {
   return 1;
} /* can_find_match_reference_inside_object() */

/** @ignore yes */
void init_dynamic_arg( mapping bing, object ) {
  function f;

  if ( bing[ "::" ] ) {
    ::init_dynamic_arg( bing[ "::" ] );
  }
  /*
   * Potential order of inventory generation problem here...  Where the
   * upper parts of the container don't initialise until after we
   * return...
   */
  if ( bing[ "inv" ] ) {
    f = (: handle_restore_inventory($1) :);
    if (!_player) {
      _player = this_player();
    }
    if (_player) {
      load_auto_load_to_inventory( bing["inv"], this_object(), _player, f );
    } else {
      load_auto_load_to_inventory( bing["inv"], this_object(), this_player(), f);
    }
  }
} /* init_dynamic_arg() */

/** @ignore yes */
void init_static_arg( mapping bing ) {
  if ( bing[ "::" ] ) {
    ::init_static_arg( bing[ "::" ] );
  }
  if ( !undefinedp( bing[ "max weight" ] ) ) {
    _max_weight = bing[ "max weight" ];
  }
  if ( !undefinedp( bing[ "prevent insert" ] ) ) {
    _prevent_insert = bing[ "prevent insert" ];
  }
  if (bing["can export inventory"]) {
    set_can_export_inventory();
  } else {
    reset_can_export_inventory();
  }
} /* init_static_arg() */

/** @ignore yes */
mixed query_static_auto_load() {
  if (file_name(this_object())[0..13] == "/std/container") {
    return int_query_static_auto_load();
  }
  return ([ ]);
} /* query_static_auto_load() */

/** @ignore yes */
void dest_me() {

   foreach( object ob in all_inventory( this_object() ) ) { 
       reset_eval_cost();
       ob->dest_me();
   }

   ::dest_me();
} /* dest_me() */
