/*  -*- LPC -*-  */
// Planning to rewrite the thing from scratch, probably not this millenium 
// though, Wodan 31-5-1999.
/**
 * This contains information about all the rooms in the mud.  It is used
 * by things like the route handler to determine routes between two
 * locations.
 * <p>
 * As coded by Wodan and overhauled by Olorin and overhauled again by Wodan.
 * and rewritten by Olorin
 * The thing is somehow improving though :)
 * @author Wodan
 */

/* All mappings are indexed on [ directory ][ file ] 
 * map contains direction, destination pairs
 * shorts contain the a_short() of the rooms
 * movezoness contain the movezones
 * coords holds the coordinates of the rooms
 * sizes holds the sizes of the rooms
 * changetimes contain the time the file where last changed
 * forced_rooms hold the rooms that are to be updated even if the file is old.
 * 
 * cache contains the directories remembered in the mappings and
 * is used for flushing them to keep the size of the handler down.
 */
#include <map_handler.h>
#include <config.h>
inherit "/obj/handlers/inherit/distance_handler";

mapping shorts = ([ ]);
mapping movezoness = ([ ]);
mapping changetimes = ([ ]);
#if !efun_defined(db_exec)
mapping clusters = ([ ]);
mapping map = ([ ]);
#else
int db, loop = 0;
#include <db.h>
#endif
mapping forced_rooms = ([ ]);
#ifdef COORDS
mapping coords = ([ ]);
mapping sizes = ([ ]);
#endif
int load = 0, time, stopped;
string *cache = ({ }), *queue = ({ });

int check_next_call_out;

mapping stats = ([]);

private void do_room1( string room );
private void check_next();
private void check_map2( string );

void create() {
   seteuid( (string)"secure/master"->get_root_uid() );
   "/room/void"->force_load();
   check_next_call_out = call_out( (: check_next :), SKEWTIME );
#if efun_defined(db_exec)
   db = db_connect("localhost","discworld",CONFIG_DB_USER);
#endif
} /* create() */


/**
 * This method goes through the cahce on the directory and
 * removes all the current references to the directory.  If the
 * load parameter is non-zero then the directory is forced to
 * be reloaded even if it already exists.  If this parameter is
 * not set then the directory will only be loaded if it is not
 * already in the cache.
 * <p>
 * This method uses the functions in the map helper to set the
 * values in all the internal variables.  It also swaps the
 * specified directory onto the end of the cache list, so it
 * will be removed later.
 * @param directory the directory to update
 * @param load should we force this to reload
 * @see clear_cache()
 * @see /obj/handlers/map_helper.c
 */
protected void update_cache( string directory, int load ) {
   if ( !shorts[ directory ] || load ) {
      cache = ( cache - ({ directory }) ) + ({ directory });
      if ( sizeof( cache ) > CACHESIZE ) {
         map_delete( shorts, cache[ 0 ] );
         map_delete( movezoness, cache[ 0 ] );
         map_delete( changetimes, cache[ 0 ] );
#if !efun_defined(db_exec)
         map_delete( clusters, cache[ 0 ] );
         map_delete( map, cache[ 0 ] );
#endif
#ifdef COORDS
         map_delete( coords, cache[ 0 ] );
         map_delete( sizes, cache[ 0 ] );
#endif
         cache -= ({ cache[ 0 ] });
         LOG( sprintf( "%%^RED%%^uncaching %s%%^RESET%%^\n", cache[ 0 ] ) );
      }
      shorts[ directory ] = MH->query_shorts( directory );
      movezoness[ directory ] = MH->query_movezones( directory );
      changetimes[ directory ] = MH->query_changetimes( directory );
#if !efun_defined(db_exec)
      clusters[ directory ] = MH->query_clusters( directory );
      map[ directory ] = MH->query_map( directory );
#endif
#ifdef COORDS
      coords[ directory ] = MH->query_co_ords( directory );
      sizes[ directory ] = MH->query_sizes( directory );
#endif
   }
} /* update_cache() */

/**
 * This method completely clears the cache of all information.  This method
 * is used for debugging purposes.
 * @see update_cache()
 */
void clear_cache() {
   shorts = ([ ]);
   movezoness = ([ ]);
   changetimes = ([ ]);
#if !efun_defined(db_exec)
   clusters = ([ ]);
   map = ([ ]);
#endif
#ifdef COORDS
   coords = ([ ]);
   sizes = ([ ]);
#endif
   cache = ({ });
} /* clear_cache() */

#if !efun_defined(db_exec)
/**
 * This method attempts to return the map for the current directory.  This
 * returns a mapping which contains an index being the room and the
 * indicies are direction, destination pairs.
 * <pre>
 * ([
 *    file : ({
 *              direction1,
 *              destination1,
 *              ...
 *            }),
 *  ])
 * </pre>
 * @param dir the directory to load the information for
 * @return the directions mapping
 * @see update_cache()
 * @see change_map()
 */
mapping query_map( string dir ) {
   if ( !map[ dir ] ) {
     update_cache( dir, 1 );   
   }
   stats["map"]++;
   return map[ dir ];
} /* query_map() */

/**
 * This method changes the current values of the map values for the
 * specified room.
 * @param dir the directory to change the values in
 * @param room the room to change the directory for
 * @param dd the array containing the direction, destination pairs
 * @see query_map()
 */
void change_map( string dir, string room, string *dd ) {
   MH->change_map( dir, room, dd );
   map[ dir ][ room ] = dd;
} /* change_map() */
#endif

/**
 * This method will return all the shorts for the specified directory.
 * The mapping returned has keys of the room names and values of the
 * short description of the room.
 * @param dir the directory to return the shorts for
 * @return the shorts descriptions mapping
 * @see change_shorts()
 */
mapping query_shorts( string dir ) {
   if ( !shorts[ dir ] ) {
      update_cache( dir, 1 );
   }
   stats["shorts"]++;
   return shorts[ dir ];
} /* query_shorts() */

/**
 * This method changes the short description for the room and directory.
 * @param dir the directory to change the short in
 * @param room the room to change the short in
 * @param sh the short description for the room
 * @see query_shorts()
 */
void change_shorts( string dir, string room, string sh ) {
   MH->change_shorts( dir, room, sh );
   shorts[ dir ][ room ] = sh;
} /* change_shorts() */

/**
 * This method returns all the move zones for the specified directory.
 * This method returns a mapping whose keys are the rooms and the
 * values are the movezones for that room.
 * @param dir the directory to return the move zones for
 * @return the movezones mapping
 * @see change_movezones()
 */
mapping query_movezones( string dir ) {
   if ( !movezoness[ dir ] ) {
      update_cache( dir, 1 );
   }
   stats["movezones"]++;
   return movezoness[ dir ];
} /* query_movezones() */

/**
 * This method changes the value of the move zones for the specified
 * room and directory.
 * @param dir the directory to change the values in
 * @param room the room the change the values in
 * @param zones the new set of move zones
 * @see query_movezones()
 */
void change_movezones( string dir, string room, string *zones ) {
   MH->change_movezones( dir, room, zones );
   movezoness[ dir ][ room ] = zones;
} /* change_movezones() */

/**
 * This method returns all the change times for the specified directory.
 * The change times are the time at which the file was last changed.
 * This method returns a mapping, the key of the mapping is the
 * room and the index of the mapping is the time at which it changed
 * (an integer).
 * @param dir the directory to return the change times for
 * @return the change times mapping
 * @see change_changetimes()
 */
mapping query_changetimes( string dir ) {
   if ( !changetimes[ dir ] ) {
      update_cache( dir, 1 );
   }
   stats["changetimes"]++;
   return changetimes[ dir ];
} /* query_changetimes() */

/**
 * This method changes the current change time for the room in the
 * dirctory.
 * @param dir the directory to change the room in
 * @param room the room to change
 * @param time the new change time for the room
 * @see query_changetimes()
 */
void change_changetimes( string dir, string room, int time ) {
   MH->change_changetimes( dir, room, time );
   changetimes[ dir ][ room ] = time;
} /* change_changetimes() */

/**
 * This method returns the current clusters for the specified dir.
 * The returned mapping has keys of the room and the values are
 * the cluster number.
 * @param dir the directory to return the clusters for
 * @return the clusters mapping
 * @see change_clusters()
 */
#if !efun_defined(db_exec)
mapping query_clusters( string dir ) {
  if ( !clusters[ dir ] ) {
    update_cache( dir, 1 );
  }
  return clusters[ dir ];
} /* query_clusters() */

/**
 * This method change sthe current cluster for the given room.
 * @param dir the directory in which to change the room
 * @param room the room to change the cluster in
 * @param cluster the new value for the cluster of the room
 * @see quert_clusters()
 */
void change_clusters( string dir, string room, int cluster ) {
   MH->change_clusters( dir, room, cluster );
   if(clusters[ dir ])
     clusters[ dir ][ room ] = cluster;
} /* change_clusters() */
#endif

#ifdef COORDS
/**
 * This method returns the coordinates for the specified directory.
 * The mapping containers keys of the room and the values are the
 * coordinates of the room.
 * @param dir the directory to return the coordinates for
 * @return the coordinates mapping
 * @see change_co_ords()
 */
mapping query_co_ords( string dir ) {
   if ( !coords[ dir ] )
      update_cache( dir, 1 );
   stats["co_ords"]++;
   return coords[ dir ];
} /* query_co_ords() */

/**
 * This method changes the coordinates of the room.
 * @param dir the directory to change the coordinates in
 * @param room the room to change the coordinates for
 * @param coord the new coordinates for the room
 */
void change_co_ords( string dir, string room, int *coord ) {
   MH->change_co_ords( dir, room, coord );
   coords[ dir ][ room ] = coord;
} /* change_co_ords() */

/**
 * This method will return the sizes for all the rooms in the directory.
 * @return the sizes mapping
 * @param dir the directory to return the sizes mapping for
 * @see change_sizes()
 */
mapping query_sizes( string dir ) {
   if ( !sizes[ dir ] )
      update_cache( dir, 1 );
   stats["sizes"]++;
   return sizes[ dir ];
} /* query_sizes() */

/**
 * This method changes the currently stored sizes for the room
 * @param dir the direct to change the size in
 * @param room the room to change the size in
 * @param size the new size for the room
 * @see query_sizes()
 */
void change_sizes( string dir, string room, int *size ) {
   MH->change_sizes( dir, room, size );
   sizes[ dir ][ room ] = size;
} /* change_sizes() */
#endif

/**
 * This method deletes a room from the specified directory.
 * @param dir the directory to delete the room from
 * @param room the room to delete
 */
void del( string dir, string room ) {
   MH->del( dir, room );
   update_cache( dir, 1 );
} /* del() */

#if !efun_defined(db_exec)
/**
 * This method returns the cluster number for the specified room.
 * @param room the room to find the cluster for
 * @return the cluster number for the room
 * @see set_cluster()
 */
int query_cluster( string room ) {
  string directory = implode( explode( room, "/" )[ 0..<2 ], "/" );
  string troom = explode( room, "/" )[ <1 ];

  if ( troom[ <2.. ] == ".c" ) {
    troom = troom[ 0..<3];
  }
  if( !clusters[ directory ] ) {
    update_cache( directory, 1 );
  }
  if( !clusters[ directory ] ) {
    clusters[ directory ] = ([]);
  }
  return query_clusters( directory )[ troom ];
} /* query_cluser() */

/**
 * This method sets the cluster number for the specified room.
 * @param room the room to set the cluster number for
 * @param cluster the new cluster number
 * @see query_cluster()
 */
void set_cluster( string room, int cluster ) {
  string directory = implode( explode( room, "/" )[ 0..<2 ], "/" );
  string troom = explode( room, "/" )[ <1 ];

  if ( troom[ <2.. ] == ".c" ) {
    troom = troom[ 0..<3];
  }
  change_clusters( directory, troom, cluster);
} /* set_cluster() */
#endif

/**
 * This method returns the destination directory array for the specified
 * room.  This is equivilant to the method of the same name on /std/room.c
 * Returns an array of ({ direction, destination, ... })
 * @see /std/room->query_dest_dir()
 * @return an array of direction, destination pairs
 * @param room the room to return the destination direction array for
 * @see query_destination()
 */
string *query_dest_dir( string room ) {
#if !efun_defined(db_exec)
  string directory, troom;
#else
  mixed rows;
  string *ret;
  int i = 0;
#endif

  stats["dest_dir"]++;
  
  if(!room || room == "/" || room == "" || strsrch(room, "/") == -1)
    return ({ });

#if !efun_defined(db_exec)
  directory = implode( explode( room, "/" )[ 0..<2 ], "/" );
  troom = explode( room, "/" )[ <1 ];

  if ( troom[ <2.. ] == ".c" ) {
    troom = troom[ 0..<3];
  }
   
  if( !query_map( directory )[ troom ] ) {
    check_map2( room );
  }
  return query_map( directory )[ troom ];
#else
  
  if ( room[ <2.. ] == ".c" ) {
    room = room[ 0..<3];
  }

  if(loop == time())
    return ({});

  loop = time();
  
  rows = db_exec(db, "select exit, dest from roomexit where id = '%s'", db_escape(room));
  if(stringp(rows)||!rows){
    forced_rooms[ room ] = 1;
    check_map2( room );
    rows = db_exec(db, "select exit, dest from roomexit where id = '%s'", db_escape(room));
    if(stringp(rows)){
      loop = 0;
      //      tell_creator("wodan", rows);
      return ({ });
    }
  }

  ret = allocate(rows * 2);

  for(i=0; i<rows; i++){
    string *row;
    row = db_fetch(db, i + 1);
    ret[i*2] = row[0];
    ret[i*2+1] = row[1];
  }
  loop = 0;
  return ret;
#endif
} /* query_dest_dir() */

/**
 * @ignore yes
 * I sincerely hope this is just a debugging function...
 * Nope, Wodan's lazy, O.
 * May also be used to return only route related stuff later on, W.
 */
string *qdd( string room ) {
   return query_dest_dir( room );
} /* qdd() */

/**
 * This method returns the destination associated with the exit in the
 * room. 
 * @param exit the exit in the room to check
 * @param room the room in which to check for the exit
 * @return the destination for the exit
 * @see query_dest_dir()
 */
string query_destination( string exit, string room ) {
   int i;
   string *dest_dir;

   stats["destination"]++;
   
   if( !room || room == "") {
      room = file_name( environment( previous_object() ) );
      if( !room ) return "/room/void";
   }
   dest_dir = query_dest_dir( room );
   i = member_array( exit, dest_dir );
   if( i < 0 ) return "/room/void";
   return dest_dir[ i + 1 ];
} /* query_destination() */

/**
 * @ignore yes
 * I sincerely hope this is just a debugging function...
 * Nope, Wodan's lazy, O.
 */
string qdes( string exit, string room ) {
   return query_destination( exit, room );
} /* qdes() */

/**
 * @ignore yes
 */
string *qdir( string room ) {
   int i;
   string *qddir, *dir=({});

   stats["qdir"]++;
   
   if( !room || room == "") {
      room = file_name( environment( previous_object() ) );
      if( !room ) return ({ "/room/void" });
   }
   qddir = query_dest_dir( room );
   for( i = sizeof( qddir ) - 2;i > -1;i -= 2)
     dir += ({ qddir[ i ]} );
   return dir;
} /* qdir() */

/**
 * This method returns the current cache stores in the map handler.
 * @return the list of directories currently loaded
 * @see update_cache()
 * @see clear_cache()
 */
string *query_cache() { return cache; }

/**
 * This method returns the current quere stores in the map handler.  The
 * queue is a list of rooms waiting to be processed into the map
 * handler.
 * @return the current queue
 */
string *query_queue() { return queue; }

/**
 * This method sets the stopped attribute of the map handler.
 * If the stopped attribute is non-zero then the map handler will 
 * stop processing the currently queued rooms.
 * @param s the new value of the stopped flag
 */
void set_stopped( int s ) { stopped = s; }

/**
 * This method compares too arrays to see if they are the same.
 * @return 1 if they are not the same, 0 if they are
 * @param ar1 the first array to compare
 * @param ar2 the second array to compare
 */
int compare_arrays( mixed *ar1, mixed *ar2 ) {
   int i;
   if ( !ar1 && !ar2 ) return 0;
   if ( sizeof( ar1 ) != sizeof( ar2 ) ) return 1;
   i = sizeof( ar1 );
   while ( i-- ) {
      if ( ar1[ i ] != ar2[ i ] ) {
         return 1;
      }
   }
   return 0;
} /* compare_arrays() */

/**
 * This method will queue the room without doing any tests.
 * This should only be used by people who know what they are doing
 * as it bypasses several checks.
 * @param room the room to queue
 * @param force if it is forced to queue
 * @param reason the reason it was queued
 */
varargs void queue_room( string room, int force, string reason ) {
   queue -= ({ room });
   queue += ({ room });
   forced_rooms[ room ] = force;
#ifdef LOGTO
   if ( !reason ) reason = "";
   LOG( sprintf( "%%^RED%%^Queueing %s(%i) %s.%%^RESET%%^\n",
                room, sizeof( queue ), reason ) );
#else 
   reason = reason; //the unused warning is hard to stop otherwise
#endif
} /* queue_room() */

/**
 * This method will decide if room should be checked, using the checks that
 * can be done without loading the room, queue the check of this room.
 * The callout is to alleviate a problem of too deep recursion errors
 * eg. when the room to be checked it loaded because of a command like drag
 * This functions should NOT be private, since it's the fundamental interface
 * toorooms.
 * @param room the room to check
 * @see queue_room()
 */
void do_room( string room ) {
   call_out( (: do_room1 :), 1, room );
} /* do_room() */

private void do_room1( string room ) {
   mixed tmp;
   int changetime;
   string directory = implode( explode( room, "/" )[ 0 .. <2 ], "/" );
   string troom = explode( room, "/" )[ <1 ];

   if ( troom[ <2.. ] == ".c" ) /* ensure sanity if the destination 
                                 * has .c on the end */
      troom = troom[ 0 .. <3 ];
   if ( directory[ 0 ] == 'w' ) {
      LOG( sprintf( "%%^BLUE%%^Skipping %s,%%^RESET%%^ in /w/.\n", room ) );
      return; /* ignore rooms in /w/ */
   }
   if ( directory[ 0 .. 2 ] == "std" ) {
      LOG( sprintf( "%%^BLUE%%^Skipping %s,%%^RESET%%^ in /std/.\n", room ) );
      return; /* ignore rooms in /std/ */
   }
   /* does it exist? */
   tmp = stat( room +".c" );
   if( !pointerp( tmp ) || sizeof( tmp ) < 2 ) {
      tmp = stat( room );
      if( !pointerp( tmp ) || sizeof( tmp ) < 2 )
         return;
   }
   tmp = find_object( room );
   if ( tmp ) { /* if the room is loaded and has coordinates
                 * and they aren't saved, do a calc
                 */
      if( tmp->query_property( "no map" ) ) {
         LOG( sprintf( "%%^BLUE%%^Skipping %s%%^RESET%%^, it asked me to.\n",
                      tmp->a_short() ) );
         return;
      }
#ifdef COORDS
      tmp->calc_co_ord();
      if ( sizeof( tmp->query_co_ord() ) >
          sizeof( query_co_ords( directory )[ troom ] ) ) {
         queue_room( room, 1, "new co_ordinates");
         return;
      }
#endif
   }
   /* Is it newer? */
   tmp = stat( room +".c" );
   if ( query_changetimes( directory )[ troom ] == 
       ( changetime = tmp[ TIME ] ) ) {
      return;
   }
   queue -= ({ room });
   queue += ({ room });
   LOG( sprintf( "%%^RED%%^Queueing %s(%i).%%^RESET%%^\n",
                room, sizeof( queue ) ) );
} /* do_room1() */

/**
 * This method attempts to remove the next room to be checked off the
 * queue.
 */
private void check_next() {
   string room;

   if ( sizeof( queue ) && !stopped ) {
      //if ( find_call_out( check_next_call_out ) == -1 ) {
        check_next_call_out = call_out( (: check_next :), SKEWTIME );
      //}
      room = queue[ 0 ];
      queue -= ({ room });
      check_map2( room );
   } else {
      check_next_call_out = call_out( (: check_next :), 60 );
   }
} /* check_next() */

private void check_short( object room_obj, string room_short, string room ) {
   string tmp_short, bad_short;
   
   if(!room_short)
     return;
   tmp_short = lower_case(room_short);
   bad_short = 0;
   if ( tmp_short[ 0 .. 4 ] == "an a " ||
       tmp_short[ 0 .. 5 ] == "an an " ||
       tmp_short[ 0 .. 5 ] == "the the " ||
       tmp_short[ 0 .. 5 ] == "a the " ) {
      bad_short = "strange or missing determinate";
   } else {
      if(!sizeof(tmp_short))
         bad_short = "no short";
      else if ( tmp_short[ <1 ] == '.' ) {
         bad_short = "period in short";
      } else {
         if ( !(tmp_short = (string)room_obj->query_property( "determinate" ) ) ) {
            tmp_short = (string)room_obj->query_short();
            if ( tmp_short )
            if ( tmp_short[ 0 .. 0 ] != lower_case( tmp_short[ 0 .. 0 ] ) )
               bad_short = "suspicious capitalisation";
         } else {
            if ( tmp_short != "" ) {
               if ( tmp_short[ <1 ] != ' ' ) {
                  bad_short = "missing space in determinate";
               } else {
                  if ( tmp_short[ 0 .. 0 ] != lower_case( tmp_short[ 0 .. 0 ] ) ) {
                     bad_short = "suspicious capitalisation in determinate";
                  }
               }
            }
         }
      }
   }
   if ( bad_short ) {
      log_file( "STRANGE_SHORT","%s, %s (%s)\n",
               room_short, bad_short, room );
      LOG( sprintf( "%%^RED%%^%s.%%^RESET%%^\n", bad_short ) );
   }
} /* check_short() */

void check_map( string room ) {
   do_room( room );
} /* check_map() */

/**
 * This checks to make sure all the bits of the map are correct still.
 * I think...
 */
private void check_map2( string room ) {
   mixed tmp;
   int i, changetime, do_update;
   string *dest_dir, *tmpdest_dir, room_short, *movezones;
#ifdef COORDS
   int *coord;
   mixed size;
#endif
   object room_obj;
   string directory = implode( explode( room, "/" )[ 0..<2 ], "/" );
   string troom = explode( room, "/" )[ <1 ];

   if ( troom[ <2.. ] == ".c" ) {
     troom = troom[ 0..<3];
   }

   if ( directory[ 0 ] == 'w' ) {
      LOG( sprintf( "%%^BLUE%%^Skipping %s,%%^RESET%%^ in /w/.\n", room ) );
      return; /* ignore rooms in /w/ */
   }
   if ( directory[ 0 .. 2 ] == "std" ) {
      LOG( sprintf( "%%^BLUE%%^Skipping %s,%%^RESET%%^ in /std/.\n", room ) );
      return; /* ignore rooms in /std/ */
   }
   do_update = forced_rooms[ room ];

   tmp = stat( room +".c" );
   if( !pointerp( tmp ) || sizeof( tmp ) < 2 )
      return;
#ifdef COORDS
   /* check for changed coordinates for loaded rooms
    * supercede check for old file
    */
   if ( room_obj = find_object( room ) ) {
      room_obj->calc_co_ord();
      if ( ( sizeof( room_obj->query_co_ord() ) >
            sizeof( query_co_ords( directory )[ troom ] ) ) ||
          compare_arrays( room_obj->query_co_ord(),
                         query_co_ords( directory )[ troom ] ) ) {
         do_update = 1;
      }
   }
#endif
   if ( !do_update &&
       ( query_changetimes( directory )[ troom ] == 
        ( changetime = tmp[ TIME ] ) ) ) {
      return;
   }
   if ( !find_object( room ) ) /* no need to reload if it's loaded already */
      catch( room->force_load() );
   room_obj = find_object( room );

   /* remove rooms that may have been put in the queue more than once
    * done here as the force_load may have put the room here
    */
   queue -= ({ room });
   map_delete( forced_rooms, room );
      
   update_cache( directory, 0 );
   if( !room_obj ) {
      /* cloned rooms don't have their own file, 
       * so this gets those out as well 
       */
      if( shorts[ directory ][ troom ] ) {
         dest_dir = query_dest_dir(room);
         for(i = 1; i < sizeof( dest_dir ); i += 2 )
            check_map( dest_dir[ i ] );
#if !efun_defined(db_exec)
         map_delete( map[ directory ], troom );
#endif
         map_delete( shorts[ directory ], troom );
         map_delete( movezoness[ directory ], troom );
         map_delete( changetimes[ directory ], troom );
#ifdef COORDS
         map_delete( coords[ directory ], troom );
         map_delete( sizes[ directory ], troom );
#endif
         del( directory, room );
         LOG( sprintf( "%%^RED%%^Removing %s,%%^RESET%%^.\n", room ) );
      }
      return;
   }
   if ( room_obj->query_property( "no map" ) ) {
      LOG( sprintf( "%%^BLUE%%^Skipping %s,%%^RESET%%^ it asked me to.\n", 
                   room ) );
      return;
   }
   room_short = (string)"/global/events"->convert_message(
         (string)room_obj->a_short() );
   if ( shorts[ directory ][ troom ] != room_short ) 
      do_update = 1;
   LOG( sprintf( "%%^YELLOW%%^Updating %s%%^RESET%%^. (%s)\n", 
                room_short, troom ) );
#ifdef LOG_SHORTS
   check_short( room_obj, room_short, room );
#endif
   movezones = room_obj->query_zones();
   if ( !pointerp( movezones ) ) movezones = ({ });
   if ( movezoness[ directory ][ troom ] != movezones )
      do_update = 1;
#ifdef COORDS
   room_obj->calc_co_ord();
   coord = room_obj->query_co_ord();
   if ( !coord ) room_obj->calc_co_ord();
   coord = room_obj->query_co_ord();
   if ( !coords[ directory ] ) coords[ directory ] = ([ ]);
   if ( compare_arrays( coords[ directory ][ troom ], coord ) )
      do_update = 1;
   
   size = room_obj->query_room_size_array();
   if ( !sizes[ directory ] ) sizes[ directory ] = ([ ]);
   if ( sizes[ directory ][ troom ] != size )
      do_update = 1;
#endif
   
   if ( changetimes[ directory ][ troom ] != changetime )
      do_update = 1;
   
   if ( do_update ) {
      change_changetimes( directory, troom, changetime );
      change_movezones( directory, troom, movezones );
      change_shorts( directory, troom, room_short );
#ifdef COORDS
      /* only change coordinates if it actually has a new set */
      if ( sizeof( coord ) == 3 )
         change_co_ords( directory, troom, coord );
      change_sizes( directory, troom, size );
#endif
      update_cache( directory, 1 );
   }

   dest_dir = room_obj->query_dest_dir();
   if( !dest_dir ) {
      dest_dir = ({ });
   } else {
      string exit;
      if( room_obj->query_blockage() ) {
        exit = room_obj->query_direction();
        if( !exit ) // there should be a better way to do this
          for( i = 1; i < sizeof( dest_dir ); i += 2 ) 
            if( dest_dir[ i ]->query_blockage() ) {
              exit = dest_dir[ i - 1 ];
              break;
            }
      }
      if(!exit)
         exit = "";
      /* remove exits to cloned destinations ,roadblocks, 
         enter something, /w, /std and /room stuff */
      tmpdest_dir = ({ });
      for ( i = 1; i < sizeof( dest_dir ); i += 2 ) {
         mixed tmp2;
         catch(load_object(dest_dir[i]));
         if ( objectp( find_object( dest_dir[ i ] ) ) &&
                          !clonep( find_object( dest_dir[ i ] ) ) && 
              exit != dest_dir[i - 1] && 
              dest_dir[i - 1][0..5] != "enter " &&
              dest_dir[i][1] != 'w' &&
              dest_dir[i][1..3] != "std" &&
              dest_dir[i][1..4] != "room" &&
                     (catch(tmp2 = dest_dir[i]->query_property( "no map" )), !tmp2)) {
            tmpdest_dir += ({ dest_dir[ i - 1 ], dest_dir[ i ] });
         }
      }
      dest_dir = tmpdest_dir;
   }
   if ( compare_arrays( query_dest_dir(room), dest_dir ) ) {
#if !efun_defined(db_exec)
      map[ directory ][ troom ] = dest_dir;
#else
      db_exec(db, "delete from roomexit where id = '%s'", db_escape(room));
      for(i=0; i<sizeof(dest_dir)/2; i++){
        db_exec(db, "insert into roomexit values ('%s', '%s', '%s')",
                db_escape(room), db_escape(dest_dir[i*2]),
                db_escape(dest_dir[i*2+1]));
      }
#endif
      for( i = 1; i < sizeof( dest_dir );i += 2 ) {
         if ( dest_dir[ i ][ <2.. ] == ".c" ) {
#ifdef LOG_EXITS
            log_file("BAD_EXIT","from %s, %s to %s\n",
                     directory +"/"+ troom, dest_dir[ i-1 ], dest_dir[ i ] );            
#endif   
            dest_dir[ i ] = dest_dir[ i ][ 0..<3 ];
         }
      }
#if !efun_defined(db_exec)
      change_map( directory, troom, dest_dir );
#endif
#ifdef PARANOID
      update_cache( directory, 1 );
#endif
      for( i = 1; i < sizeof( dest_dir ); i += 2 ) {
         do_room( dest_dir[ i ] );
      }
#ifdef PARANOID
      return;
#endif
   }
#ifdef PARANOID
   update_cache( directory, 1 );
#endif
} /* check_map2() */

/**
 * This method returns the short description of the room.  This will return
 * the short description of named cloned objects as well as attempting to
 * force the room to load if it is not found in the internal
 * cache.
 * @param room the room to return the short description of
 */
string query_short( string room ) {
   string directory = implode( explode( room, "/" )[ 0..<2 ], "/" );
   string troom = explode( room, "/" )[ <1 ];

   if ( troom[ <2.. ] == ".c" ) {
     troom = troom[ 0..<3];
   }

   stats["short"]++;
   
   if ( strsrch( room, "#" ) != -1 ) {
      if ( find_object( room ) ) {
         return (string)"/global/events"->convert_message(
                     (string)room->a_short() );
      } else {
         return "The map_handler was called on a non-existent clone";
      }
   }
   if ( !shorts[ directory ] ) {
      update_cache( directory, 1 );
   }
   if ( !shorts[ directory ] ) {
      shorts[ directory ] = ([ ]);
   }
   if ( shorts[ directory ][ troom ] ) {
     return shorts[ directory ][ troom ];
   }
   if ( !catch( load_object(room) ) ) {
     return (string)"/global/events"->convert_message(
                            (string)room->a_short() );
   }
   return "Oops!";
} /* query_short() */

/**
 * This method returns the short description of the room only from
 * within the map handlers files.  This will not cause the room
 * to load and will not work on cloned file names.
 * @param room the room to return the short description of
 * @see query_short()
 */
string static_query_short( string room ) {
   string directory, troom;

   if(!stringp(room))
     return "";
   
   directory = implode( explode( room, "/" )[ 0..<2 ], "/" );
   troom = explode( room, "/" )[ <1 ];

   if ( troom[ <2.. ] == ".c" ) {
     troom = troom[ 0..<3];
   }

   stats["static short"]++;
   
   if ( !shorts[ directory ] ) {
      update_cache( directory, 1 );
   }
   if ( !shorts[ directory ] ) {
      shorts[ directory ] = ([ ]);
   }
   if ( shorts[ directory ][ troom ] ) {
     return shorts[ directory ][ troom ];
   }
   return 0;
} /* static_query_short() */

/**
 * This method returns the move zones for the specified room.  It will
 * load the room if it is not available in the map handlers cache.
 * @param room the room to get the zones for
 * @return the array of move zones for the room
 * @see query_short()
 */
string *query_zones( string room ) {
   string directory = implode( explode( room, "/" )[ 0..<2 ], "/" );
   string troom = explode( room, "/" )[ <1 ];
   object roomob;

   if ( troom[ <2.. ] == ".c" ) {
     troom = troom[ 0..<3];
   }

   stats["zones"]++;
   
   if ( strsrch( room, "#" ) != -1 ) {
      if ( find_object( room ) ) {
         return (string *)room->query_movezones();
      } else {
         return ({ });
      }
   }
   if ( !movezoness[ directory ] ) {
      update_cache( directory, 1 );
   }
   if ( !movezoness[ directory ] ) {
      movezoness[ directory ] = ([ ]);
   }
   if ( movezoness[ directory ][ troom ] ) {
     return movezoness[ directory ][ troom ];
   }
   if ( !catch( roomob = load_object(room) ) ) {
     if(roomob)
       return roomob->query_movezones();
   }
   return ({ "Oops!" });
} /* query_zones() */

#ifdef COORDS
/**
 * This method sets the coordinates for the specified room.
 * @param room the room to set the coordinates for
 * @param coord the coordinates to set the room to
 * @see query_co_ord()
 */
void set_co_ord( string room, int *coord ) {
   string directory = implode( explode( room, "/" )[ 0..<2 ], "/" );
   string troom = explode( room, "/" )[ <1 ];

   if ( troom[ <2.. ] == ".c" ) {
     troom = troom[ 0..<3];
   }
   
   if ( !coords[ directory ] ) {
      update_cache( directory, 1 );
   }
   if ( !coords[ directory ] ) {
      coords[ directory ] = ([ ]);
   }
   MH->change_co_ords( directory, troom, coord );
   coords[ directory ][ troom ] = coord;
} /* set_co_ords() */

/**
 * This method returns the coordinates for the specified room.  This will
 * cause the room to load if possible to extract the coordinates.
 * @param room the room the get the coordinates of
 * @return the coordinates of the room, 0 on failure
 */
int *query_co_ord( string room ) {
   string directory = implode( explode( room, "/" )[ 0..<2 ], "/" );
   string troom = explode( room, "/" )[ <1 ];

   if ( troom[ <2.. ] == ".c" ) {
     troom = troom[ 0..<3];
   }

   stats["co_ord"]++;
   
   LOG( sprintf( "%%^BLUE%%^query_co_ord(%s)%%^RESET%%^\n", room ) );
   if ( strsrch( room, "#" ) != -1 ) {
      if ( find_object( room ) ) {
         return room->query_co_ord();
      } else {
         return 0;
      }
   }
   if ( !coords[ directory ] ) {
      update_cache( directory, 1 );
   }
   if ( !coords[ directory ] ) {
      coords[ directory ] = ([ ]);
   }
   if ( coords[ directory ][ troom ] ) {
     return coords[ directory ][ troom ];
   }
   if ( !catch( load_object(room ) ) ) {
     return room->query_co_ord();
   }
   return 0;
} /* query_co_ord() */

/**
 * This method will return the coordinates of the room only using the
 * internal map information.
 * @param room the room to get the coordinates of
 * @return the coordinates of the room, 0 on failure
 */
int *static_query_co_ord( string room ) {
   string directory = implode( explode( room, "/" )[ 0..<2 ], "/" );
   string troom = explode( room, "/" )[ <1 ];

   if ( troom[ <2.. ] == ".c" ) {
     troom = troom[ 0..<3];
   }

   stats["static co_ord"]++;
   
   if ( strsrch( room, "#" ) != -1 ) {
      if ( find_object( room ) ) {
         return room->query_co_ord();
      } else {
         return 0;
      }
   }
   if ( !coords[ directory ] ) {
      update_cache( directory, 1 );
   }
   if ( !coords[ directory ] ) {
      coords[ directory ] = ([ ]);
   }
   if ( coords[ directory ][ troom ] ) {
     return coords[ directory ][ troom ];
   }
   return 0;
} /* static_query_co_ord() */

/**
 * This method returns the size of the room, it will attempt to load
 * the room if it is not in map handlers current rooms.
 * @param room the room to find the size of
 */
int *query_room_size( string room ) {
   string directory = implode( explode( room, "/" )[ 0..<2 ], "/" );
   string troom = explode( room, "/" )[ <1 ];

   if ( troom[ <2.. ] == ".c" ) {
     troom = troom[ 0..<3];
   }

   stats["room_size"]++;
   
   if ( strsrch( room, "#" ) != -1 ) {
      if ( find_object( room ) ) {
         return room->query_room_size_array();
      } else {
         return 0;
      }
   }
   if ( !sizes[ directory ] ) {
      update_cache( directory, 1 );
   }
   if ( !sizes[ directory ] ) {
      sizes[ directory ] = ([ ]);
   }
   if ( sizes[ directory ][ troom ] ) {
     return sizes[ directory ][ troom ];
   }
   if (!catch(room->force_load())) {
      return room->query_room_size_array();
   }
   return 0;
} /* query_room_size() */
#endif

/**
 * This method tests to see if the specified room exists and is loadable.
 * @param room the room to test for existance
 * @return 1 if it exists and 0 if it does not
 */
int test_room( string room ) {
   string directory = implode( explode( room, "/" )[ 0..<2 ], "/" );
   string troom = explode( room, "/" )[ <1 ];
   
   if ( troom[ <2.. ] == ".c" ) {
     troom = troom[ 0..<3];
   }
   
   if ( strsrch( room, "#" ) != -1 )  {
      return 1;
   }
   if ( !shorts[ directory ] ) {
      update_cache( directory, 1 );
   }
   if ( !shorts[ directory ] ) {
      shorts[ directory ] = ([ ]);
   }
   if ( shorts[ directory ][ troom ] ) {
     return 1;
   }
   if ( !catch( load_object(room) ) ) {
     return 1;
   }
   return 0;
} /* test_room() */

/**
 * This method returns true if the room is known of to the map handler.
 * @param room the room to test for existance
 * @return 1 if it exists, 0 if it does not
 */
int query_known( string room ) {
   string directory = implode( explode( room, "/" )[ 0..<2 ], "/" );
   string troom = explode( room, "/" )[ <1 ];

   if ( troom[ <2.. ] == ".c" ) {
     troom = troom[ 0..<3];
   }

   if ( !shorts[ directory ] ) {
      update_cache( directory, 1 );
   }
   if ( !shorts[ directory ] ) {
      shorts[ directory ] = ([ ]);
   }
   return !undefinedp(shorts[ directory ][troom]);
} /* query_known() */

/**
 * This method checks the save files in the directory to check to make
 * sure all the rooms still exist in the game.
 */
void check_save_files() {
   string *files, file, room, *tmp;

   files = unguarded((: get_dir, "/save/map/" :));
   files = map( files, (: replace( $1, ({ "_", "/", ".o", ""}) ) :) );
   foreach( file in files ) {
      printf( "%s\n", file );
      tmp = keys( MH->query_map( file ) );
      reset_eval_cost();
      foreach( room in tmp ) {
         if( file_size( file +"/"+ room + ".c" ) < 0 ) {
            printf( "%s no longer exists.\n", file + "/" + room );
            MH->del( file, room );
         }
      }
   }
   cache = ({ });
} /* check_save_files() */

/**
 * these two are not here because the handler can be saved, but to keep
 * the info over updates.
 * @ignore yes
 */
mapping query_dynamic_auto_load() {
   mapping tmp;

   tmp = ([ "shorts" : shorts,
            "movezoness" : movezoness,
            "changetimes" : changetimes,
#if !efun_defined(db_exec)
            "clusters" : clusters,
            "map" : map,
#endif
#ifdef COORDS
            "coords" : coords,
            "sizes" : sizes,
#endif
            "cache" : cache,
            "queue" : queue,
          ]);
   return tmp;
} /* query_dynamic_auto_load() */

/**
 * @ignore yes
 */
void init_dynamic_arg(mapping maps) {
   if (maps["shorts"])
      shorts = maps["shorts"];
   if (maps["movezoness"])
      movezoness = maps["movezoness"];
   if (maps["changetimes"])
      changetimes = maps["changetimes"];
#if !efun_defined(db_exec)
   if (maps["clusters"])
      clusters = maps["clusters"];
   if (maps["map"])
     map = maps["map"];
#endif
#ifdef COORDS
   if (maps["coords"])
      coords = maps["coords"];
   if (maps["sizes"])
      sizes = maps["sizes"];
#endif
   if (maps["cache"])
      cache = maps["cache"];
   if (maps["queue"])
      queue = maps["queue"];
} /* init_dynamic_arg() */

/** @ignore yes */
mixed *stats() {
   return ({
      ({ "queue size", sizeof( queue ) }),
      ({ "cache size", sizeof( cache ) }),
      ({ "next check", find_call_out( check_next_call_out ) }),
        }) + map(keys(stats), (:({$1, $(stats)[$1]}):));
}
