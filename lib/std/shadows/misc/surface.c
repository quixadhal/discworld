/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: surface.c,v 1.1 1998/01/06 04:39:04 ceres Exp $
 * $Log: surface.c,v $
 * Revision 1.1  1998/01/06 04:39:04  ceres
 * Initial revision
 * 
*/
object shadowed, surface;

void setup_shadow( object thing1, object thing2 ) {
  shadow( thing1, 1 );
  shadowed = thing1;
  surface = thing2;
} /* setup_shadow() */

object query_shadowed( object thing ) {
  if ( thing == this_object() ) return shadowed;
  return (object)thing->query_shadowed( thing );
} /* query_shadowed() */

object *find_inv_match( string words ) {
  object *things;
  things = (object *)shadowed->find_inv_match( words );
  if ( surface )
    things += (object *)surface->find_inv_match( words );
  else
    call_out( "destruct_shadow", 1, this_object() );
  return things;
} /* find_inv_match() */

void destruct_shadow( object thing ) {
  if ( thing == this_object() ) destruct( this_object() );
  else thing->destruct_shadow( thing );
} /* destruct_shadow() */
