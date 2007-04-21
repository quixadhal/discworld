/*   -*- LPC -*-   */
/*
 * $Locker:  $
 * $Id: key_handler.c,v 1.2 1998/04/23 14:01:25 pinkfish Exp $
 * $Log: key_handler.c,v $
 * Revision 1.2  1998/04/23 14:01:25  pinkfish
 * Added documentation.  And cleanedup a little.
 *
 * Revision 1.1   1998/01/06 05:03:33   ceres
 * Initial revision
 * 
 */
/**
 * This handler keeps track of the keys.  The keys are arranged in
 * groups and each key type in the group has a name and a set of
 * permissions associated with it.
 * @author Deutha
 */
#define RESTORE_PATH "/save/keys/"
 
inherit "/std/object";

mapping keys;
mapping key_names;
nosave string _current_group;

/**
 * This sets up the variables with the correct statup values.
 * @param group the key ground to initialise
 */ 
private void init_data( string group ) {
   keys = ([ ]);
   key_names = ([ ]);
} /* init_data() */

/**
 * This gets all the current data for a specified key group.
 * @param group the key group to get the data for
 * @return 0 if the group was not found
 */
private int get_data_file( string group ) {
   if (_current_group == group) {
      return 1;
   }

   _current_group = group; 
   if ( file_size( RESTORE_PATH + group +".o" ) > 0 ) {
      unguarded((: restore_object, RESTORE_PATH + group :));
   } else {
      init_data( group );
      return 0;
   }
   return 1;
} /* get_data_file() */

/**
 * This method saves the data currently in memory to the specified key
 * group.
 */
private void save_data_file( ) {
   unguarded((: save_object, RESTORE_PATH + _current_group :));
} /* save_data_file() */

/**
 * This method returns all the keys in the current group.  The mapping
 * consists has an index of the key name and a value of an array of
 * key permission types.
 * @param group the key group to get permissions for
 * @return a mapping containing all the permissions
 * @see remove_key_permission()
 * @see add_key_permission()
 * @see query_key_permission()
 * @see set_key_permissions()
 * @see query_all_key_permissions()
 */
mapping query_all_keys( string group ) {
   get_data_file( group );
   return keys;
} /* query_all_keys() */

/**
 * This method should never be called.  It assumes internal knowedge.
 * Bad bad bad!
 * @ignore yes
 */
private void set_keys( string group, mapping list ) {
   get_data_file( group );
   keys = list;
   save_data_file( );
} /* set_keys() */

/**
 * This method returns all the permision types associated with the key.
 * The permission types are an array of string permision type names.  If
 * the key name is non-existant an empty array will be returned.
 * @param group the key group to lookup
 * @param key the key name to lookup
 * @return the array of permission types
 * @see remove_key_permission()
 * @see add_key_permission()
 * @see query_key_permission()
 * @see set_key_permissions()
 * @see query_all_keys()
 */
string *query_all_key_permissions( string group, string key ) {
   get_data_file( group );
   if ( !keys[ key ] ) {
      return ({ });
   }
   return keys[ key ];
} /* query_all_key_permissions() */

/**
 * This method will set the key permissions for a certain sort of
 * key.  It will overwrite any current possible key permission sets.
 * @param group the key group to set the permissions for
 * @param key the key to set the permission for
 * @param types the new set of permission types
 * @see remove_key_permission()
 * @see add_key_permission()
 * @see query_key_permission()
 * @see query_all_key_permissions()
 * @see query_all_keys()
 */
void set_key_permissions( string group, string key, string *types ) {
   get_data_file( group );
   if (sizeof(types)) {
      keys[ key ] = types;
   } else {
      map_delete(keys, key);
   }
   save_data_file( );
} /* set_key_permissions() */

/**
 * This method will see if the given key perrmision exists for this
 * key in this particular group and key.
 * @param group the key group to look up in
 * @param key the key to check
 * @param type the permission type to see if it exists
 * @return 1 if the permission exists, 0 if not
 * @see remove_key_permission()
 * @see add_key_permission()
 * @see set_key_permissions()
 * @see query_all_key_permissions()
 * @see query_all_keys()
 */
int query_key_permission( string group, string key, string type ) {
   get_data_file( group );
   if ( !keys[ key ] ) {
      return 0;
   }
   if ( member_array( type, keys[ key ] ) == -1 ) {
      return 0;
   }
   return 1;
} /* query_key_permission() */

/**
 * This method adds in a specific permission type into the current
 * permission set for this key.
 * @param group the key group to add the permision to
 * @param key the key to add the permission for
 * @param type the permission type to add in
 * @return 0 if it is already added, 1 if not
 * @see remove_key_permission()
 * @see query_key_permission()
 * @see set_key_permissions()
 * @see query_all_key_permissions()
 * @see query_all_keys()
 */
int add_key_permission( string group, string key, string type ) {
   get_data_file( group );
   if ( !keys[ key ] ) {
       keys[ key ] = ({ });
   }
   if ( member_array( type, keys[ key ] ) != -1 ) {
      return 0;
   }
   keys[ key ] += ({ type });
   save_data_file( );
   return 1;
} /* add_key_permission() */

/**
 * This method removes a specific permission type from the given
 * key group and key.
 * @param group the key group to remove the key from
 * @param key the key to remove the permission from
 * @param type the type of permission to remove
 * @return 0 if the permission was not remove, 1 if it was
 * @see add_key_permission()
 * @see query_key_permission()
 * @see set_key_permissions()
 * @see query_all_key_permissions()
 * @see query_all_keys()
 */
int remove_key_permission( string group, string key, string type ) {
   int i;

   get_data_file( group );
   if ( !keys[ key ] ) {
      return 0;
   }
   if ( ( i = member_array( type, keys[ key ] ) ) == -1 ) {
      return 0;
   }
   keys[ key ] = delete( keys[ key ], i, 1 );
   if (!sizeof(keys[key])) {
      map_delete(keys, key);
   }
   save_data_file( );
   return 1;
} /* remove_key_permission() */

/**
 * This method returns all the key names for the given key group.
 * The index of the mapping is the key type and the value is its name.
 * @param group the key group
 * @return the mapping containing the key names
 * @see delete_key_name()
 * @see set_key_name()
 * @see query_key_name()
 */
mapping query_all_key_names( string group ) {
   get_data_file( group );
   return key_names;
} /* query_all_key_names() */

private void set_key_names( string group, mapping list ) {
   get_data_file( group );
   key_names = list;
   save_data_file( );
} /* set_key_names() */

/**
 * This method returns the key name for a given key.
 * @param group the key group to lookup
 * @param key the key type to look up
 * @return the key's name
 * @see delete_key_name()
 * @see set_key_name()
 * @see query_all_key_names()
 */
string query_key_name( string group, string key ) {
   get_data_file( group );
   return key_names[ key ];
} /* query_key_name() */

/**
 * This method sets the name of the specified key.
 * @param group the group to set the name of the key in
 * @param key the type of key to set the name for
 * @param name the name of the key to set
 * @see delete_key_name()
 * @see query_key_name()
 * @see query_all_key_names()
 */
void set_key_name( string group, string key, string name ) {
   get_data_file( group );
   key_names[ key ] = name;
   save_data_file( );
} /* set_key_name() */

/**
 * This method removes a specified key name from the current key name
 * mapping.
 * @param group the group to remove the key from
 * @param key the key type to remove the name of
 * @see set_key_name()
 * @see query_key_name()
 * @see query_all_key_names()
 */
void delete_key_name( string group, string key ) {
   get_data_file( group );
   if ( !key_names[ key ] ) {
      return;
   }
   map_delete( key_names, key );
   save_data_file( );
} /* delete_key_name() */
