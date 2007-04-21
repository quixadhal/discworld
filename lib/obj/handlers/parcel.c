/**
 *
 * Post Office Delivery Handler
 * @author Terano
 * @started 26 December, 1998
 *
 */
#define BASE_PATH "/save/parcels/"
#define SAVE_PATH BASE_PATH + location + "/" + name[0..0]
#define SAVE_FILE BASE_PATH + location + "/" + name[0..0] + "/" + name + ".o"
#define POST_LOG "PARCEL_LOG"

inherit "/global/auto_load";

// This is undefined since uncompress() is broken. Once uncompress() is
// repaired it should be possible to turn compression on but please test
// it first. -- Ceres
#undef USE_COMPRESSION

#include <player_handler.h>

/**
 * Parcels is a mapping with player names as keys, and arrays of class parcel
 * as values
 */
class parcel {
  string from;
  string *autoloadinfo;
  int exp_time;
}

mapping parcels;
private nosave string _current_name, _current_location;
nosave class parcel *_parcels;

int test_expire( class parcel temp );

/**
 * @ignore
 */
void create() {
  string dir;
  
  seteuid( master()->creator_file( base_name( this_object() ) ) );
  parcels = ([ ]);
  if(parcels) {
    parcels = copy(parcels);
    parcels = ([ ]);
  }
  _parcels = ({ });

  /*  foreach(dir in get_dir("/save/parcels/*")) {
    if(dir != "." && dir != ".." && file_size("/save/parcels/" + dir) == -2) {
      tell_creator("ceres", "Starting %s", dir);
      call_out("do_migrate", random(10), 'a', dir);
    }
  }
  */
}

/**
 * @ignore
 */
private void save_file(string name, string location) {
  string str;
  
  // Save the old parcels variable.
  map_delete(parcels, name);
  if(sizeof(keys(parcels)))
    unguarded( (: save_object, SAVE_PATH, 2 :));
  
  if(file_size(SAVE_PATH) == -1)
    unguarded( (: mkdir, SAVE_PATH :) );

  // If they have a parcel then save it to a file. If not delete any existing
  // parcel file.
  if(_parcels != ({ })) {
    str = save_variable(_parcels);
#ifdef USE_COMPRESSION
    if(!ungarded( (: write_buffer, SAVE_FILE + ".gz", 0, compress(str) :) ))
#else     
    if(!unguarded((: write_file, SAVE_FILE, str, 1 :) ))
#endif
      log_file(POST_LOG, "%s: error writing to %s\n",
               ctime(time()), SAVE_FILE);
  } else {
    if(file_size(SAVE_FILE) > 0)
      unguarded( (: rm, SAVE_FILE :));
    if(file_size(SAVE_FILE+".gz") > 0)
      unguarded( (: rm, SAVE_FILE + ".gz" :));

  }
}

/**
 * @ignore
 */
private void load_file( string name, string location ) {
  // no need to load if it's the currently loaded one.
  if(_current_name == name && _current_location == location)
    return;

#ifdef USE_COMPRESSION
  if(file_size(SAVE_FILE + ".gz") > 0) {
    _parcels = restore_variable(uncompress(read_file(SAVE_FILE)));
  } else
#endif    
  if(file_size(SAVE_FILE) > 0) {
    debug_printf("%s", SAVE_FILE);
    _parcels = restore_variable(unguarded( (: read_file, SAVE_FILE :) ));
  } else if (file_size(SAVE_PATH+ ".o.gz") > 0) {
    unguarded( (: restore_object, SAVE_PATH+".o.gz" :));
    _parcels = parcels[name];
  } else if(file_size(SAVE_PATH+".o") > 0) {
    unguarded( (: restore_object, SAVE_PATH :) );
    _parcels = parcels[name];
  } else {
    parcels = ([ ]);
    _parcels = ({ });
  }

  _current_name = name;
  _current_location = location;
}

/**
 * This function deposits some items for a player
 * @param to The name of the player items are going to
 * @param from The name of the player sending the items
 * @param location The place the items are being sent from
 * @param obs An array of the objects being sent
 * @return 1 if it worked, or 0 if it screwed up :)
 */
public int deposit(string to, string from, string location, object *obs) {
  class parcel temp;
  object boxy;
  string obsstuff, tmp;

  obsstuff = "/global/events"->convert_message(query_multiple_short (obs));
  
  if(!PLAYER_HANDLER->test_user(to))
    return 0;

#ifndef __DISTRIBUTION_LIB__
    // Do multiplayer checks.
    if("/secure/alist"->query_alt(from, to) == 100) {
      tmp = capitalize(from) + " sent a parcel containing " +
        obsstuff + " to his/her alt " + capitalize(to) + ".";
      user_event("inform", tmp, "multiplayer");
      log_file("MULTIPLAYERS", ctime(time()) + ": " +
               this_player()->convert_message(tmp) + "\n");
    } else if("/secure/alist"->query_alt(from, to) > 40) {
      tmp = capitalize(from) + " sent a parcel containing " +
        obsstuff + " to his/her POSSIBLE alt "+ capitalize(to) + ".";
      user_event("inform", tmp, "multiplayer");
      log_file("MULTIPLAYERS", ctime(time()) + ": " +
               this_player()->convert_message(tmp) + "\n");
    }
#endif

  load_file(to, location ); // Load up the right save file
  if(undefinedp(_parcels) || !arrayp(_parcels))
    _parcels = ({ });
  
  boxy = clone_object("/obj/baggage");
  boxy->set_name("boxy");
  boxy->set_max_weight(100000);

  obs->move( boxy );
  temp = new( class parcel,
              from : from,
              exp_time: time() + ( 14 * 24 * 60 * 60));
  temp->autoloadinfo = create_auto_load( ({ boxy }), 0 );
  _parcels += ({ temp });
  save_file(to, location);
  boxy->dest_me();
  log_file (POST_LOG, "%s: %s deposited %s at %s for %s.\n", ctime(time()), 
      from, obsstuff, location, to);  
}

/**
 * This function collects any items available
 * @param name Name of the player collecting
 * @param location Location collecting from
 * @param target An object to move the items into, a room or container
 * @param copy An int, if its 1 it doesnt delete the objects from the parcels
 *        array
 * @return 0 if no items or failure, 1 if successful
 */
public int collect(string name, string location, object target, int copy) {
  class parcel temp;
  object *box_array = ({ });
  object *obs = ({ });
  string tmp;
  
  load_file(name, location ); // Load up the right save file
  
  if(!_parcels || _parcels == ({ }))
    return 0;
  
  foreach(temp in _parcels) {
    box_array += load_auto_load_to_array(temp->autoloadinfo, this_player());
#ifndef __DISTRIBUTION_LIB__
    obs = all_inventory(box_array[<1]);
    // Do multiplayer checks.
    if("/secure/alist"->query_alt(name, temp->from) == 100) {
      tmp = capitalize(name) + " received a parcel containing " + 
        "/global/events"->convert_message(query_multiple_short(obs)) +
        " from his/her alt " + capitalize(temp->from) + ".";
      user_event("inform", tmp, "multiplayer");
      log_file("MULTIPLAYERS", ctime(time()) + ": " +
               this_player()->convert_message(tmp) + "\n");
    } else if("/secure/alist"->query_alt(name, temp->from) > 40) {
      tmp = capitalize(name) + " received a parcel containing " +
        "/global/events"->convert_message(query_multiple_short(obs)) +
        " from his/her POSSIBLE alt "+ capitalize(temp->from) + ".";
      user_event("inform", tmp, "multiplayer");
      log_file("MULTIPLAYERS", ctime(time()) + ": " +
               this_player()->convert_message(tmp) + "\n");
    }
#endif
  }
  
  foreach( object box in box_array ) {
    obs += all_inventory (box);
    all_inventory( box )->move( target );
    box->dest_me();
  }
  
  if ( !copy )
    _parcels = ({ });

  log_file (POST_LOG, "%s: %s collected %s at %s.\n", ctime(time()), 
            name, "/global/events"->convert_message(query_multiple_short(obs)),
            location);  
  save_file(name, location);
  return 1;
}

/**
 * Queries if any parcels are at a location for a person
 * @param name of the person
 * @param location of the post office
 */
class parcel *queryparcels_for(string name, string location) {
  load_file(name, location);

  return _parcels;
} 

/**
 * Migrate packages
 * @param letter to expire
 * @param location to expire
 */
void do_migrate(int letter, string location) {
  string name;
  class parcel temp2;

  tell_creator("ceres", "Processing %c in %s\n", letter, location);
  load_file(sprintf("%c", letter), location);
  foreach(name, temp2 in parcels) {
    _parcels = filter(temp2, (: test_expire :));
    save_file(name, location);
  }
  if(!sizeof(keys(parcels))) {
    name = BASE_PATH + location + "/" + sprintf("%c", letter) + ".o.gz";
    unguarded((: rm, name :));
  }
  if(letter < 'z') {
    letter++;
    call_out("do_migrate", 15 + random(30), letter, location);
  }
}

/**
 * @ignore
 */
int test_expire(class parcel temp ) {
  if ( temp->exp_time > time() )
    return 1;

  return 0;
}

/**
 * Called when a player deletes
 * Deletes all their parcels
 * @param string name
 * @param int Total Flag
 */
void player_refreshed(mixed name, int total ) {
  string location;
  string *dirs = map( filter( get_dir( BASE_PATH, -1 ), 
                              (: $1[1] == -2 :) ), (: $1[0] :) );

  if (objectp(name))
    name = name->query_name();
  
  foreach(location in dirs) {
    load_file(name, location);
    map_delete(parcels, name);
    save_file(name, location);
  }
}
