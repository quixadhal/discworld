/**
 * This handler fixes up virtual objects if their files change, so that their static variables will be adjusted.
 * @author Dragonkin

 */

#define SAVE_FILE "/save/virtuals"
#define MAX_NUMBERS 50

mapping load_data;
string *paths;
string *missing_paths;
string *forbidden_paths;

/** @ignore yes */
void create() {
   seteuid( "/secure/master"->creator_file( file_name( this_object() ) ) );
   load_data = ([ ]);
   paths = ({ });
   missing_paths = ({ });
   forbidden_paths = ({ });
   if ( file_size( SAVE_FILE +".o" ) > 0 )
      unguarded( (: restore_object, SAVE_FILE :) );
} /* create() */

/** @ignore yes */
void save_me() { unguarded( (: save_object, SAVE_FILE :) ); }

/**
 * Adds a path and its static auto load information to the mapping
 * and cleans the mapping and paths array.
 * @param path the path of the object to add
 */
void add_data( string path ) {
  object junk_item;
  string tmp_str;
  int tmp_int;

  junk_item = clone_object( path );
  load_data[ path ] = ({ stat( path )[1], junk_item->query_static_auto_load() });
  destruct( junk_item );

  paths -= ({ path });
  paths += ({ path });

  if( sizeof( paths ) > MAX_NUMBERS ) {
    tmp_int = sizeof( paths ) - (MAX_NUMBERS + 1);
    paths = paths[tmp_int+1..tmp_int+MAX_NUMBERS];
  }
  foreach( tmp_str in keys( load_data ) ) {
    if( member_array( tmp_str, paths ) == -1 ) {
      map_delete( load_data, tmp_str );
    }
  }
  save_me();
}

/**
 * Called by virtual objects as they are being cloned, if their timestamp is older than their file.
 * @param path the path of the virtual object
 * @return the object's static auto load info if any, otherwise 0
 */
mapping new_data( string path ) {
  if( ( !load_data[path] ) || ( !sizeof( stat( path ) ) ) || ( load_data[path][0] < stat( path )[1] ) ) {
    if( !sizeof( stat( path ) ) ) {
      return 0;
    }
    add_data( path );
  }
  return load_data[path][1];
} /* new_data() */

/**
 * A function to see what items have faulty paths
 * @return all faulty paths in the handler
 */
string *query_missing_paths() { return missing_paths; }

/**
 * Called by the handler to add a faulty path to the list
 * @param path the path of the virtual object
 * @return 1 if added, 0 if not (because it's already in the list or in a /w directory)
 */
int add_missing( string path ) {
  if( !missing_paths )
    missing_paths = ({ });
  if( member_array( path, missing_paths ) != -1 )
    return 0;
  if( path[0..2] == "/w/" )
    return 0;
  missing_paths += ({ path });
  save_me();
  return 1;
}

/**
 * Called to remove a faulty path from the list.
 * @param path the path of the virtual object
 */
void remove_missing( string path ) {
  missing_paths -= ({ path });
  save_me();
}

/**
 * A function to see if a path is forbidden
 * @param path the path of the virtual object to check
 * @return 1 if forbidden, 0 if not
 */
int query_forbidden( string path ) {
  return ( member_array( path, forbidden_paths ) != -1 );
}

/**
 * A function to see what paths are forbidden
 * @return all forbidden paths in the handler
 */
string *query_forbidden_paths( ) { return forbidden_paths; }

/**
 * Called to add a path to the list of forbidden paths.  A forbidden virtual object being loaded will transform into a receipt.
 * @param path the path of the virtual object to forbid
 * @return 1 if the path is added, 0 if it is already in the list
 */
int add_forbidden( string path ) {
  if( !forbidden_paths )
    forbidden_paths = ({ });
  if( member_array( path, forbidden_paths ) != -1 )
    return 0;
  forbidden_paths += ({ path });
  save_me();
  return 1;
}

/**
 * Called to remove a forbidden path from the list.
 * @param path the path of the virtual object
 */
void remove_forbidden( string path ) {
  forbidden_paths -= ({ path });
  save_me();
}

