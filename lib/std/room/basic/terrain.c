/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: terrain.c,v 1.1 1998/11/01 12:02:03 terano Exp $
 *
 *
 */

/*
 * $Locker:  $
 * $Id: terrain.c,v 1.1 1998/11/01 12:02:03 terano Exp $
 */
#include <dirs.h>
#include <terrain.h>

string terrain_name;
object room;

void setup_shadow(object thing, string word) {
  shadow(thing, 1);
  TERRAIN_MAP->setup_location(thing, word);
  thing->add_property("terrain name", word);
  terrain_name = word;
  room = thing;
} /* setup_shadow() */

void destruct_shadow(object thing) {
  if (thing == this_object()) {
    TERRAIN_MAP->delete_cloned_location(terrain_name, file_name(room));
    destruct(this_object());
  } else {
    thing->destruct_shadow( thing );
  }
} /* destruct_shadow() */

void event_exit(object thing, string message, object to) {
  if (!objectp(room)) {
    room = query_shadowing(this_object());
  }
  if (clonep(room)) {
    if (find_call_out("free_location") == -1) {
      call_out("free_location", 10);
    }
  }
  room->event_exit(thing, message, to);
} /* event_exit() */

void free_location() {
  if (sizeof(all_inventory(room)) || (int)this_object()->dont_free())
    return;
  TERRAIN_MAP->delete_cloned_location(terrain_name, file_name(room));
  room->dest_me();
} /* free_location() */

void set_destination(string direc) {
  int i, j, delta, *co_ords, *new_co_ords = allocate(3);
  string dest_name;
  object destination;

  if ((i = member_array(direc, STD_ORDERS)) == -1) {
    return;
  }
  if (!objectp(room)) {
    room = query_shadowing(this_object());
  }
  dest_name = (string)room->query_destination(direc);
  delta = (int)room->query_room_size() + TERRAIN_MAP->get_room_size(dest_name);
  co_ords = room->query_co_ord();
  for (j = 0; j < 3; j++) {
    new_co_ords[j] = co_ords[j] - delta * STD_ORDERS[i+1][j];
  }
  destination = TERRAIN_MAP->find_location(terrain_name, new_co_ords);
  if (!destination) {
    return;
  }
  room->modify_exit(direc, ({ "dest", file_name(destination) }));
} /* set_destination() */

string query_quit_handler() { return TERRAIN_LOG; }

mixed *query_cloning_info() {
  return ({ TERRAIN_MAP, "find_location",
            terrain_name, (int *)room->query_co_ord() });
} /* query_cloning_info() */
