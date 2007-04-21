/**
 * A Room Rental Handler
 * <p>
 * This keeps track of who rented what rooms where
 * and for how much.
 * <p>
 * After including <room/room_rental.h> there are two options
 * available for room inherits.  ROOM_RENTAL_INHERIT_BASIC_ROOM 
 * includes the basic room inherit while ROOM_RENTAL_INHERIT_ROOM
 * is for use in areas that have their own base room inherits.
 * <p> 
 * The most important function is the one that checks that someone
 * is allowed to enter.
 * <br>
 * int check_entry( object player, object room )
 * <br>
 * it returns:
 * RENTAL_SUCCESS if the player rents the room successfully
 * RENTAL_NO_SUCH_ROOM if the room isn't on the books
 * RENTAL_ALREADY_RENTED if the room is rented to someone else
 * RENTAL_AVAILABLE if the player doesn't rent the room but it is available
 *
 * @author goldenthread
 * @started 3 Sep y2k1
 *
 */

#include <money.h>
#include <room/room_rental.h>


protected void do_load();
protected void do_save();
protected void evict_tenant( object room_ob );

private nosave int loaded = 0;
private mapping rented_rooms = ([]);
private mapping rental = ([]);
private mapping eviction_times = ([]);
private mapping locations = ([]);

/* maps room shorts to paths */
private mapping paths = ([]);

/** @ignore yes */
void create() {
  do_load();
} /* create() */

/** @ignore yes */
void reset() {
   string room;
   int now = time();

   foreach( room in keys( rented_rooms ) ) {
      if ( stringp( rented_rooms[ room ] ) ) {
         if ( ( eviction_times[ room ] < now ) &&
              ( eviction_times[ room ] > -1 ) ) {
            /* chuck out the tenant! */
            evict_tenant( find_object( room ) );

            tell_creator( "goldenthread", "RENTAL: %s no longer rents %s\n",
                          rented_rooms[ room ], room );
            rented_rooms[ room ] = 0;
            eviction_times[ room ] = -1;
         }
      }
   }

   do_save();
} /* reset() */


/** @ignore yes */
protected void do_load() {
   if ( !loaded )
      unguarded( (: restore_object( ROOM_RENTAL_SAVE_FILE, 1 ) :) );
   loaded = 1;
} /* do_load */

/** @ignore yes */
protected void do_save() {
   unguarded( (: save_object( ROOM_RENTAL_SAVE_FILE , 0 ) :) );
} /* do_save */


/**
 * Evicts the tenant ( and guests ).
 *
 * @param room_ob the room object
 */
void evict_tenant( object room_ob ) {
   object *occupants;

   occupants = filter( all_inventory( room_ob ), (: living :) );
   room_ob->evict_tenant( occupants );
}

/**
 * Adds a room to the handler with a rental cost.
 *
 * @see delete_room()
 *
 * @param room the path of the room to add to the handler
 * @param cost the rental cost per day of this room
 * @param office the current office
 *
 * @example
 *    add_room( "/w/goldenthread/workroom", 1000000 )
 */
int add_room( string room, int cost, object office ) {
   object room_ob;
   if( !room_ob = load_object( room ) )
      return 0;
      
   rented_rooms[ room ] = 0;
   rental[ room ] = cost;
   eviction_times[ room ] = -1;
   locations[ room ] = office->query_location();
   paths[ room_ob->short() ] = room;
   
   do_save();
   
   return 1;
} /* add_room */

/**
 * Returns the name of whoever is renting the room specified.
 *
 * @param room the short of the room you want to check
 * @param office the current office
 *
 * @return the short of the player who's renting the room
 */
string query_who_rents( string room, object office ) {
   if ( locations[room] == office->query_location() ) {
      if ( stringp( rented_rooms[room] ) )
         return rented_rooms[room];
      else
         return "nobody";
   } else
      return "no such room in this location";
} /* query_who_rents */

/**
 * Checks if a particular player is renting a particular room.
 *
 * @param player_ob the player object who you think might be renting the room
 * @param room_ob   the room object that you think the player might be renting
 *
 * @return value dependent on success of check.  See below for details
 *
 * @main
 * returns:
 * RENTAL_SUCCESS if the player rents the room successfully
 * RENTAL_NO_SUCH_ROOM if the room isn't on the books
 * RENTAL_ALREADY_RENTED if the room is rented to someone else
 * RENTAL_AVAILABLE if the player doesn't rent the room but it is available
 */
int check_tenant( object player_ob, object room_ob, object office ) {
   string player = player_ob->short();
   string room = room_ob->short();

   if ( player == query_who_rents( room, office ) )
      return RENTAL_SUCCESS;
   else if ( player == "nobody" )
      return RENTAL_AVAILABLE;
   else if ( player == "no such room!" )
      return RENTAL_NO_SUCH_ROOM;
   else
      return RENTAL_ALREADY_RENTED;
} /* check_tenant */

/**
 * Deletes a room from the handler
 *
 * @see add_room()
 *
 * @param room the path of the room that will be deleted from the handler
 * @param office the current office
 *
 * @return 0 on success, 1 on failure
 */
int delete_room( string room, object office ) {
  string room_short = load_object( room )->short();

  if ( locations[room] == office->query_location() ) {
    map_delete( rented_rooms, room );
    map_delete( rental, room );
    map_delete( eviction_times, room );
    map_delete( locations, room );
    map_delete( paths, room_short );

    do_save();

    return 1;
  } else
    return 0;
} /* delete_room */

/**
 * Clears the list of rooms available for rent.
 *
 * @param office the current office
 *
 * WARNING: This clears all rooms bookable from this office.
 */
int clear_rooms( object office ) {
   object room_ob;
   
   foreach( string room in keys( filter( locations,
            (: $2 == $( office->query_location() ) :) ) ) ) {
      room_ob = load_object( room );
    map_delete( rented_rooms, room );
    map_delete( rental, room );
    map_delete( eviction_times, room );
    map_delete( locations, room );
    map_delete( paths, room_ob->query_short() );
  }

  do_save();

  return 1;
} /* clear_rooms */


/**
 * Returns a list of rooms available in a particular location.
 *
 * @return a string ready for presentation to a player containing a
 * list of all rooms available in that office.
 *
 * @param office the office
 */
string *query_rooms_to_rent( object office ) {
  return keys( filter( locations,
               (: $2 == $( office->query_location() ) :) ) );
} /* query_rooms_to_rent */


/**
 * Handles the actual renting of the room.  If the prospective
 * tenant is already renting the room, it extends the rental.
 *
 * @see remove_rental()
 *
 * @param player_ob the player who is renting the room
 * @param room_short the short of the room that is to be rented
 * @param office the current office
 *
 * @return value dependent on success of rental. See below for details
 *
 * @main
 * returns:
 * RENTAL_SUCCESSFUL if the rental is successful
 * RENTAL_NO_SUCH_ROOM if the room isn't on the books
 * RENTAL_ALREADY_RENTED if someone else is renting the room already
 * RENTAL_NO_MONEY if the player hasn't enough money
 */
int do_rental( object player_ob, string room_short, object office ) {
   int result;
   string domain = office->query_domain();
   string room = paths[ room_short ];
   
   /* is the room available for rent */
   if ( locations[room] == office->query_location() ) {
      if ( !rented_rooms[room] ||
           ( rented_rooms[room] == player_ob->short()) ) {
         if ( (int)player_ob->query_value_in( domain ) < rental[room] )
            return RENTAL_NO_MONEY;

         rented_rooms[room] = player_ob->short();
         eviction_times[room] = time() + ONE_DAY;
         result = RENTAL_SUCCESS;
      } else
         result = RENTAL_ALREADY_RENTED;
   } else
      result = RENTAL_NO_SUCH_ROOM;
   
   do_save();

   return result;
} /* do_rental() */

/**
 * Returns the amount needed to pay for a rental
 *
 * @param room the room being queried
 * @return rental amount
 */
int query_rental_value( string room ) {
   return rental[ room ];
} /* query_rental_value() */

/**
 * Removes the rented status from a room.  Should be called when the
 * time limit of the rental is over.
 *
 * @see do_rental()
 *
 * @param room_ob the room object that is currently rented
 *
 * @return 0 on success, 1 on failure
 */
int remove_rental( object room_ob ) {
   int result;
   string room_short = room_ob->short();
   string room = paths[ room_short ];

   if ( !undefinedp( rented_rooms[room] ) ) {
      rented_rooms[room] = 0;
      result = RENTAL_SUCCESS;
   } else
      result = RENTAL_NO_SUCH_ROOM;
   
   do_save();
   
   return result;
} /* remove_rental() */







