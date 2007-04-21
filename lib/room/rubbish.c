inherit "/std/room/basic_room";

nosave private mapping delete_list;
nosave private int destroyed;

void setup() {
  set_short( "rubbish room" );
  set_long( "This is the rubbish room.  Anything coming in here will be "+
            "destroyed five seconds later.\n" );
  set_keep_room_loaded(1);
  delete_list = ([ ]);
  set_heart_beat(1);
  destroyed = 0;
}

string long(string word, int dark) {
  if(query_verb() == "scry" )
    return "empty space.";
  
  return ::long(word, dark);
}

int no_init() { return 1; }

/**
 * @ignore yes
 * Every 2 seconds try to destroy some stuff.
 */
void heart_beat() {
  object ob;
  int i;

  i = 0;
  foreach(ob in all_inventory()) {
    if(!delete_list[ob])
      delete_list[ob] = time();
    else if(delete_list[ob] < time() - 4) {
      destroyed++;
      map_delete(delete_list, ob);
      destruct(ob);
      reset_eval_cost();
    }
  }
}

/**
 * @ignore yes
 * This just cleans up anything that got left behind. If a callout got lost
 * or went wrong or something this'll make sure our rubbish room is clean.
 */
void reset() {
  object *inventory;
   
  set_heart_beat(1);
  inventory = filter( all_inventory( this_object() ), 
                      (: !interactive( $1 ) :) );
    
  map(inventory, (: $1 && destruct($1) :));
}

mixed stats() {
  return ::stats() + ({
    ({ "destroyed", destroyed }),
  });
}

object *query_list() {
  return keys(delete_list);
}
