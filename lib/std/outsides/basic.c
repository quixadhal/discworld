/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: basic.c,v 1.3 2002/03/26 01:07:07 pinkfish Exp $
 * $Log: basic.c,v $
 * Revision 1.3  2002/03/26 01:07:07  pinkfish
 * Make all of these rooms have the basic details for handling
 * terrain map rooms.
 *
 * Revision 1.2  2000/06/22 01:19:30  pinkfish
 * Make it use autodoc and fix up the inherit paths.
 *
 * Revision 1.1  1998/01/06 04:29:46  ceres
 * Initial revision
 * 
*/
#include <terrain_map.h>
#define CREATOR "Ceres"
/**
 * This is a really basic outside room. It has rocks and that's about all.
 * @author Ceres
 */
inherit TERRAIN_MAP_OUTSIDE_BASE;

mixed do_get_rock( string verb, object *dest ) {
  int rock_size;
  string rock_type;
  object rock;
  if ( query_property( "rock object" ) )
    return ( rock = clone_object( (string)query_property( "rock object" ) ) );
  if ( undefinedp( rock_size = (int)query_property( "rock size" ) ) )
    rock_size = 3;
  if ( !rock_size ) return 0;
  rock = clone_object( "/obj/mineral" );
  if ( undefinedp( rock_type = (string)query_property( "rock type" ) ) )
    rock_type = ({ "cinnabar", "coal", "rosy-quartz", "chalk", "flint",
                   "slate" })[ random( 6 ) ];
  rock->make_mineral( rock_type, 1 + random( rock_size ) );
  if ( !( rock->move( this_player() ) ) ) return 1;
  rock->dest_me();
  return 0;
}
