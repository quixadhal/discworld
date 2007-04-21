/**
 * A saving room inheritable.  This handles rooms which save their inventory,
 * or more specifically part of their inventory.
 *
 * Which objects should be saved or not saved can be controlled by overriding
 * test_save.
 *
 * Containers in this room that need to cause it to save its inventory should
 * generate a save event.
 *
 * @see test_save
 * @see event_save
 *
 * @author ceres
 */
#include <move_failures.h>
#include <player.h>

inherit "/std/room/basic_room";
inherit "/std/room/inherit/room_save";
inherit "/std/basic/auto_load";

/** @ignore yes */
void create() {
   do_setup++;
   basic_room::create();
   room_save::create();
   do_setup--;

   if ( !do_setup ) {
     this_object()->setup();
     this_object()->reset();
   }
} /* create() */

/**
 * @ignore yes
 * Makes sure furniture is removed from the save file
 * when its removed from this room.
 */
int test_remove(object thing, int flag, mixed dest) {
  int result;

  result = basic_room::test_remove(thing, flag, dest);

  if(result) {
    room_save::test_remove(thing, flag, dest);
  }
  
  return result;
} /* test_remove() */

/**
 * @ignore yes
 * Makes sure furniture is saved when its put in this room.
 */
int test_add( object ob, int flag) {
  room_save::test_add(ob, flag);
  return basic_room::test_add(ob, flag);
} /* test_add() */

#ifdef DONT_USE
/** @ignore yes */
void dest_me() {
  room_save::dest_me();
  basic_room::dest_me();
}
#endif
