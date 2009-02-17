/*  -*- LPC -*-  */
/**
 * This is the map helper object.  It contains all the current information
 * about the files.  It stores the current directory and keeps track of
 * if things have changed so it does not save or load excessively.
 * @author Wodan
 */
#include <map_handler.h>

mapping shorts = ([ ]), movezones = ([ ]), changetimes = ([ ])
#if !efun_defined(db_exec)
, clusters = ([ ]), map = ([ ])
#endif
;
#ifdef COORDS
mapping coords = ([ ]), sizes = ([ ]);
#endif
string directory;      /* directory of current rooms */
nosave string loaded;  /* name of savefile */
nosave int save;       /* save needed? */

void create() {
   seteuid( (string)master()->get_bb_uid() );
} /* create() */

/**
 * This method saves the state of the currently loaded directory.
 * @see check_dir()
 */
void save_dir() {
#ifdef HELPLOGTO
   tell_object( find_living( LOGTO ), "%^RED%^saving%^RESET%^ "+ loaded +"\n");
#endif   
   // Check to make sure their is actually something in the directory.
   if (sizeof(shorts) ||
       sizeof(movezones) ||
       sizeof(changetimes) ||
#ifdef COORDS
       sizeof(coords) ||
       sizeof(sizes) ||
#endif
#if !efun_defined(db_exec)
       sizeof(clusters) ||
       sizeof(map) ||
#endif
        0) {
      unguarded((: save_object, "/save/map/" + loaded :));
   } else {
      // Nothing in the dir, remove it.
      unguarded((: rm("/save/map/" + loaded) :));
   }
   save = 0;
} /* save_dir */

/**
 * This method attempts to load in the save file information for
 * the specified directory.  If the directory has no save file
 * then the mappings will all be initialised to empty.  It will cause
 * the previous directory to be saved if this is attempting to load a
 * different one.
 * @param dir the directory to load the save information for
 * @see save_dir()
 */
void check_dir( string dir ) {
   string tmp_file;
   
   if ( dir == directory ) {
      return;
   }
   
   if ( save )  {
      save_dir();
   }

   tmp_file = replace_string( dir, "/", "_" );
   
   shorts = ([ ]);
   movezones = ([ ]);
   changetimes = ([ ]);
#if !efun_defined(db_exec)
   clusters = ([ ]);
   map = ([ ]);
#endif
#ifdef COORDS
   coords = ([ ]);
   sizes = ([ ]);
#endif
   
   if ( file_size( "/save/map/" + tmp_file + ".o" ) > 0 ) {
#ifdef HELPLOGTO
      tell_object( find_living( LOGTO ), "%^GREEN%^loading%^RESET%^ "+ tmp_file
                   +"\n");
#endif   
      unguarded((: restore_object, "/save/map/" + tmp_file :));
   } else {
#ifdef HELPLOGTO
      tell_object( find_living( LOGTO ), "%^BLUE%^newmap%^RESET%^ "+ tmp_file +"\n");
#endif   
      shorts = ([ ]);
      movezones = ([ ]);
      changetimes = ([ ]);
#if !efun_defined(db_exec)
      clusters = ([ ]);
      map = ([ ]);
#endif
#ifdef COORDS
      coords = ([ ]);
      sizes = ([ ]);
#endif
   }
   
   loaded = tmp_file;
   directory = dir;
   
} /* check_dir */

#if !efun_defined(db_exec)
/**
 * This method will change the destination direction array for the
 * given room.
 * @param dir the directory to change
 * @param room the room to change
 * @param dd the new destination directory
 */      
void change_map( string dir, string room, string *dd ) {
   check_dir( dir );
   map[ room ] = dd;
   save = 1;
} /* change_map() */

/**
 * This method returns the current destination direction mapping for
 * the given directory.
 * @param dir the directory to return the mapping for
 * @return the destination direction mapping
 */
mapping query_map( string dir ) {
   check_dir( dir );
   if ( !map )
     return ([ ]);
   return map;
} /* query_map() */
#endif

/**
 * This method changes the short for the specified directory and room.
 * @param dir the directory to change the short for
 * @param room the room the change the short for
 * @param sh the new short for the room
 */
void change_shorts( string dir, string room, string sh ) {
   check_dir( dir );
   shorts[ room ] = sh;
   save = 1;
} /* change_shorts() */

/**
 * This method returns the mapping containing all the shorts for the
 * specified directory.
 * @param dir the directory to return the shorts for
 * @return the mapping of short descriptions
 */
mapping query_shorts( string dir ) {
   check_dir( dir );
   if ( !shorts ) {
     return ([ ]);
   }
   return shorts;
} /* query_shorts() */

/**
 * This method changes the move zones for the specified room.
 * @param dir the directory to change the move zones in
 * @param room the room to change the move zones in
 * @param zones the new move zones
 */
void change_movezones( string dir, string room, string *zones ) {
   check_dir( dir );
   movezones[ room ] = zones;
   save = 1;
} /* change_movezones() */

/**
 * This method returns the mapping of move zones for the specified
 * room.
 * @param dir the directory to return the move zones for
 * @return the move zone mapping
 */
mapping query_movezones( string dir ) {
   check_dir( dir );
   if ( !movezones )
     return ([ ]);
   return movezones;
} /* query_movezones() */

#if !efun_defined(db_exec)
/**
 * This method will change the cluster for the specified room.
 * @param dir the directory to change the cluster for
 * @param room the room to change the cluster for
 * @param cluster the new cluster number
 */
void change_clusters( string dir, string room, int cluster ){
  check_dir( dir );
  if(!clusters)
    clusters = ([]); //how can this be??
  clusters[ room ] = cluster;
  save = 1;
} /* change_clusters() */

/**
 * This method returns the cluster mapping for the specified directory.
 * @param dir the directory to return the cluster mapping for
 * @return the cluster mapping for the directory
 */
mapping query_clusters( string dir ){
  check_dir( dir );
  if( !clusters )
    return ([ ]);
  return clusters;
} /* query_clusters() */
#endif


/**
 * This method changes the time at which the specified room was last
 * changed.
 * @param dir the directory to change the last modified date for
 * @param room the room to set the modied time for
 * @param time the new time
 */
void change_changetimes( string dir, string room, int time ) {
   check_dir( dir );
   if ( !changetimes )
      changetimes = ([ ]);
   changetimes[ room ] = time;
   save = 1;
} /* change_changetimes() */

/**
 * This method returns the mapping of rooms to last modified dates.
 * @param dir the directory to return the mapping for
 * @return the mapping of last modified dates
 */
mapping query_changetimes( string dir ) {
   check_dir( dir );
   if ( !changetimes )
     return ([ ]);
   return changetimes;
} /* query_changetimes() */

#ifdef COORDS
/**
 * This method changes the coordinates for the specified room/.
 * @param dir the directory to change the room in
 * @param room the room to change in the directory
 * @param coord the new coordinate
 */
void change_co_ords( string dir, string room, int *coord ) {
   check_dir( dir );
   if ( !coords )
      coords = ([ ]);
   coords[ room ] = coord;
   save = 1;
} /* change_co_ords() */

/**
 * This method returns the mapping of coordinates to rooms.
 * @param dir the directory to return the mapping for
 * @return the mapping of coordinates to rooms
 */
mapping query_co_ords( string dir ) {
   check_dir( dir );
   if ( !coords )
     return ([ ]);
   return coords;
} /* query_co_ords() */

/**
 * This method changes the roomsize for the specified room.
 * @param dir the directory to change the room in
 * @param room the room to change in the directory
 * @param coord the new size;
 */
void change_sizes( string dir, string room, int size ) {
   check_dir( dir );
   if ( !sizes )
      sizes = ([ ]);
   sizes[ room ] = size;
   save = 1;
} /* change_sizes() */

/**
 * This method returns the mapping of sizes for rooms.
 * @param dir the directory to return the mapping for
 * @return the mapping of sizes for rooms.
 */
mapping query_sizes( string dir ) {
   check_dir( dir );
   if ( !sizes )
     return ([ ]);
   return sizes;
} /* query_sizes() */
#endif

/**
 * This method deletes the specified room from the directory.
 * @param dir the directory to remove the room from
 * @param room the room to remove
 */
void del( string dir, string room ) {
   check_dir( dir );
#if !efun_defined(db_exec)
   if (mapp(map)) {
     map_delete( map, room );
   } else {
     map = ([ ]);
   }
#endif
   if (mapp(shorts)) {
     map_delete( shorts, room );
   } else {
     shorts = ([ ]);
   }
   if (mapp(movezones)) {
     map_delete( movezones, room );
   } else {
     movezones = ([ ]);
   }
   if (mapp(changetimes)) {
     map_delete( changetimes, room );
   } else {
     changetimes = ([ ]);
   }
#if !efun_defined(db_exec)
   if (mapp(clusters)) {
     map_delete( clusters, room );
   } else {
     clusters = ([ ]);
   }
#endif
#ifdef COORDS
   if (mapp(coords)) {
     map_delete( coords, room );
   } else {
     coords = ([ ]);
   }
   if (mapp(sizes)) {
     map_delete( sizes, room );
   } else {
     sizes = ([ ]);
   }
#endif
   save = 1;
} /* del() */
