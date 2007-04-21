/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: surface.c,v 1.2 1998/03/03 05:24:41 pinkfish Exp $
 * $Log: surface.c,v $
 * Revision 1.2  1998/03/03 05:24:41  pinkfish
 * Added documentation.
 *
 * Revision 1.1  1998/01/06 03:59:10  ceres
 * Initial revision
 * 
*/
/**
 * This file contains the methods to handle surfaces, like book
 * shelves and stuff.
 * @see /std/shadows/misc/surface.c
 */
#include <surface.h>

nosave object *surface_shadows;

/**
 * This method returns the current set of surface shadows associated with
 * the object.
 * @return the current set of surface shadows
 * @see make_shadow()
 * @see check_shadows()
 */
object *query_surface_shadows() {
   return surface_shadows;
} /* query_surface_shadows() */

/**
 * This method makes a surface shadow which is attached to the given object.
 * @param thing the object to attach the shadow too
 */
void make_shadow( object thing ) {
  object shadow;

  shadow = clone_object( SURFACE_SHADOW );
  shadow->setup_shadow( thing, this_object() );
  if ( !surface_shadows ) surface_shadows = ({ });
  surface_shadows += ({ shadow });
} /* make_shadow() */

/**
 * This method checks to make sure all the shadows are currenly valid.  If
 * an invalid shadow is found it is removed.
 * <p>
 * The method check_environment() is called at the end of the function.
 * @see check_environment()
 */
void check_shadows() {
  int i;

  for ( i = 0; i < sizeof( surface_shadows ); i++ ) {
    if ( surface_shadows[ i ] ) {
      if ( environment() == (object)surface_shadows[ i ]->
          query_shadowed( surface_shadows[ i ] ) ) {
        surface_shadows[ i ]->destruct_shadow( surface_shadows[ i ] );
        surface_shadows = surface_shadows[0..i-1] + surface_shadows[i + 1..];
        i--;
      }
    } else {
      surface_shadows = surface_shadows[0..i-1] + surface_shadows[i + 1..];
      i--;
    }
  }
  call_out( "check_environment", 0 );
} /* check_shadows() */

/**
 * This method checks to make sure the environment has a shadow associated
 * with it.
 * @see check_shadows()
 */
void check_environment() {
  if ( environment() ) {
     make_shadow( environment() );
  }
} /* check_environment() */

/** @ignore yes */
void dest_me() {
  int i;

  for ( i = 0; i < sizeof( surface_shadows ); i++ )
    if ( surface_shadows[ i ] )
      surface_shadows[ i ]->destruct_shadow( surface_shadows[ i ] );
} /* dest_me() */
