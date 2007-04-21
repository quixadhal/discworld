/**
 * This is the standard inheritable for a player-owned house outside room,
 * like for gardens and things.
 * <p>
 * All you need to do is inherit this file and call a couple of setup
 * functions and off you go.
 * <p>
 * In addition you will need to have a lord register the house with the
 * housing command if you wish the house to be available for sale or rent
 * using the normal processes.
 *
 * @example
 * inherit "/std/room/player_housing_outside";
 *
 * void setup() {
 *   set_light(100);
 *   set_theft_handler(HOSPITAL);
 *   set_save_file("/save/player_housing/am/short/flat1_garden");
 *
 *   set_short("front room");
 *   set_base_desc("a small garden" );
 *   add_surface( "floor", "grass" );
 *   add_surface( "north wall", "red brick" );
 *   add_surface( "south wall", "red brick" );
 *   add_surface( "east wall", "red brick" );
 *   add_surface( "west wall", "red brick" );
 *   add_exit("south", PATH+"flat1", "door");
 * }
 *
 * @author Belle, Gototh, Ceres, Pinkfish
 */
#include <dirs.h>
#include <housing.h>
#include <login_handler.h>
#include <player.h>

inherit "/std/room/outside";
inherit "/std/room/inherit/player_housing";

/** @ignore yes */
void create() {
  add_help_file("player_housing");
  do_setup++;
  outside::create();
  player_housing::create();
  do_setup--;
  if ( !do_setup ) {
    this_object()->setup();
    this_object()->reset();
  }
  seteuid("Room");
  add_property("no burial", 1);
}

/** @ignore yes
 * Internal function to update the dynamic long description of the room.
 */
string query_long() {
  return player_housing::query_long();
}

/**
 * This method returns the current owner of the house.
 */
string query_owner() {
   return HOUSING->query_owner(base_name(this_object()));
} /* query_owner() */

/** @ignore yes
 * Just makes sure room exits are relative inside houses and that doors
 * are closed.
 */
int add_exit(string direc, mixed dest, string type) {
  int tmp;

  tmp = ::add_exit(direc, dest, type);
  setup_exit(direc, 0);
  return tmp;
}

/** @ignore yes
 * Stops positioned objects from showing.  If it has a CURRENT_ROOM_VERB
 * property it's obviously a placed object and shouldn't been seen.
 */
string process_non_living(object *int_non_livings, int start) {
  object *not_positioned;
 
  not_positioned = player_housing::my_process_non_living(int_non_livings);
  return ::process_non_living(not_positioned, start);
} /* process_non_living() */

/** @ignore yes */
void init() {
  player_housing::init();
  outside::init();
}

/** @ignore yes */
void event_exit(object ob, string message, object to) {
  outside::event_exit(ob, message, to);
  player_housing::event_exit(ob, message, to);
}

/** @ignore yes
 * Cleanup could screw us up so we stay loaded.
 */
int query_keep_room_loaded() { return 1; }

/** @ignore yes */
void dest_me() {
  player_housing::dest_me();
  outside::dest_me();
}

/**
 * @ignore yes
 * Calls appropriate test_remove() functions and does PK checks for removal
 * of items from this house.
 */
int test_remove(object thing, int flag, mixed dest) {
  object thief;
  mixed *stuff;
  
  if(!outside::test_remove(thing, flag, dest))
    return 0;

  thief = this_player();
  if(!thief)
    return player_housing::test_remove(thing, flag, dest);
  
  if(thief->query_caster())
    thief = find_player(thief->query_caster());
  else if(thief->query_owner())
    thief = thief->query_owner();

  stuff = thing->query_property("dropped");

  // Do PK checks for taking this rooms inventory away.
  if(thief &&
     test_save(thing) && 
     !test_occupier(thief->query_name()) &&
     (!stuff || stuff[0] != thief->query_name())) { 
    log_file("HOUSING_THEFT", "%s: %s stole %s from %s in %s\n",
             ctime( time() )[4..18], (string)thief->query_short(),
             thing->query_short(),
             (string)query_owner(), file_name());
    
    event(this_object(), "theft", thief, this_object(), ({ thing }));
  }

  return player_housing::test_remove(thing, flag, dest);
}

/**
 * @ignore yes
 * Makes sure furniture is saved when its put in this room.
 */
int test_add( object ob, int flag) {
  int retval;
  
  retval = outside::test_add(ob, flag);
  if(retval)
    retval = player_housing::test_add(ob, flag);
  return retval;
} /* test_add() */
