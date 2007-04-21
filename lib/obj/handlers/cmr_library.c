/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: cmr_library.c,v 1.3 1998/10/05 21:44:25 ceres Exp $
 * $Log: cmr_library.c,v $
 * Revision 1.3  1998/10/05 21:44:25  ceres
 * sojan fixed a bug
 *
 * Revision 1.2  1998/10/04 23:31:55  ceres
 * Added autodoc and fixed parameter names
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/

/**
 * Handler to keep track of which players know which materials. Allows you to
 * find out which materials a player knows about and add new ones.  This is
 * typically used in conjunction with the cmr_handler for materials which
 * the player must be taught about.
 *
 * @see cmr_handler
 * @author Deutha
 */
#define RESTORE_PATH "/save/cmr_library/"
 
inherit "/std/object";

string player_name, *materials;

/**
 * Initialise a players array of known materials.
 *
 * @param pname The name of the player.
 */
void init_data( string pname ) {
  player_name = pname;
  materials = ({ });
} /* init_data() */

/**
 * @ignore yes
 */
int get_data_file( string pname ) {
  if ( player_name != pname ) {
    if ( file_size( RESTORE_PATH + pname +".o" ) > 0 )
      unguarded((: restore_object, RESTORE_PATH + pname :));
    else {
      init_data( pname );
      return 0;
    }
  }
  return 1;
} /* get_data_file() */
 
/**
 * @ignore yes
 */
void save_data_file( string word ) {
  unguarded((: save_object, RESTORE_PATH + word :));
} /* save_data_file() */

/**
 * Return the materials known by a player.
 *
 * @param pname The name of the player
 * @return A string array of known materials.
 */
string *query_known_materials( string pname ) {
  get_data_file( pname );
  if ( !materials ) {
    return ({ });
  }
  return materials + ({ });
} /* query_known_materials() */

/**
 * Return whether the player knows a particular material.
 *
 * @param pname The name of the player.
 * @param material The name of the material
 * @return 1 if known 0 if not known.
 */
int query_known_material( string pname, string material ) {
  get_data_file( pname );
  if ( !materials ) return 0;
  return ( member_array( material, materials ) != -1 );
} /* query_known_material() */

/**
 * Mark a material as known by a given player.
 *
 * @param pname The name of the player
 * @param material The name of the material
 *
 * @return Returns 0 if they already knew the material, 1 if they know it now.
 */
int add_known_material( string pname, string material ) {
  get_data_file( pname );
  if ( member_array( material, materials ) != -1 ) {
    return 0;
  }
  materials += ({ material });
  save_data_file( player_name );
  return 1;
} /* add_known_material */
