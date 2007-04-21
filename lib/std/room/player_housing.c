/**
 * This is the standard inheritable for a player-owned house room.
 *
 * All you need to do is inherit this file  and call a couple of setup
 * functions and off you go.
 *
 * In addition you will need to register the house with the
 * housing command if you wish the house to be available for sale or rent
 * using the normal processes. Make sure you have permission and have
 * read the help on the housing command before you do this.
 *
 * @example
 * inherit "/std/room/player_housing";
 *
 * void setup() {
 *   set_light(100);
 *   set_theft_handler(HOSPITAL);
 *   set_save_file("/save/player_housing/am/short/flat1");
 *
 *   set_short("front room");
 *   set_base_desc("a small and cosy front room" );
 *   add_surface( "floor", "finished pine board" );
 *   add_surface( "ceiling", "plain plaster" );
 *   add_surface( "north wall", "plain plaster" );
 *   add_surface( "south wall", "plain plaster" );
 *   add_surface( "east wall", "plain plaster" );
 *   add_surface( "west wall", "plain plaster" );
 *   add_exit("out", PATH+"courtyard", "door");
 *   add_exit("north", PATH+"flat1bed", "door");
 * }
 *
 * In the room outside the house you also need to add the following line to
 * make sure the doors from the street to the house get setup correctly (ie.
 * know if the user left the door locked or not and what kind of lock/trap
 * is in use in the door):
 * (PATH+"flat1")->setup_doors();
 *
 * @author Belle, Gototh, Ceres
 */
#include <dirs.h>
#include <housing.h>
#include <login_handler.h>
#include <player_handler.h>

inherit "/std/room/basic_room";
inherit "/std/room/inherit/player_housing";

/** @ignore yes */
void create() {
  add_help_file("player_housing");
  do_setup++;
  basic_room::create();
  player_housing::create();
  do_setup--;
  if ( !do_setup ) {
    this_object()->setup();
    this_object()->reset();
  }
  seteuid("Room");
  add_property("no burial", 1);
  add_property("no teleport", 1);
}

/** @ignore yes
 * Internal function to update the dynamic long description of the room.
 */
string query_long() {
  return player_housing::query_long();
}

/** @ignore yes
 * Just makes sure room exits are relative inside houses and that doors
 * are closed.
 */
int add_exit(string direc, mixed dest, string type) {
  int tmp;

  tmp = ::add_exit(direc, dest, type);
  setup_exit(direc, 1);
  return tmp;
}

/**
 * This method returns the address of the player house.
 * @return the address of this house
 */
string query_address() {
   return HOUSING->query_address(file_name(this_object()));
}

/** @ignore yes
 * Stops positioned objects from showing.  If it has a CURRENT_ROOM_VERB
 * property it's obviously a placed object and shouldn't been seen.
 */
string process_non_living(object *int_non_livings, int start) {
  object *not_positioned;

  not_positioned = player_housing::my_process_non_living(int_non_livings);
  return ::process_non_living(not_positioned, start);
}

/** @ignore yes */
void init() {
  player_housing::init();
  basic_room::init();
}

/** @ignore yes */
void event_exit(object ob, string message, object to) {
  //basic_room::event_exit(ob, message, to);
  player_housing::event_exit(ob, message, to);
}

/** @ignore yes
 * Cleanup could screw us up so we stay loaded.
 */
int query_keep_room_loaded() {
  player_housing::query_keep_room_loaded();
}

/** @ignore yes */
void dest_me() {
  player_housing::dest_me();
  basic_room::dest_me();
}

/**
 * @ignore yes
 * Calls appropriate test_remove() functions and does PK checks for removal
 * of items from this house.
 */
int test_remove(object thing, int flag, mixed dest) {
  object thief;
  mixed *stuff;
  
  if(!basic_room::test_remove(thing, flag, dest))
    return 0;

  thief = this_player();
  if(!thief)
    return player_housing::test_remove(thing, flag, dest);

  // Check for controlled NPCs and find their owner/caster
  if(thief->query_caster())
    thief = find_player(thief->query_caster());
  else if(thief->query_owner())
    thief = thief->query_owner();

  stuff = thing->query_property("dropped");
  
  // Do PK checks for taking this rooms inventory away.
  if(thief &&
     test_save(thing) && PLAYER_HANDLER->test_user(query_owner()) &&
     !this_object()->test_occupier(thief->query_name()) &&
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
  
  retval = basic_room::test_add(ob, flag);
  if(retval)
    retval = player_housing::test_add(ob, flag);
  return retval;
}
