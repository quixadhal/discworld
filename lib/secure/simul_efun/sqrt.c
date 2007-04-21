/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: sqrt.c,v 1.3 1999/09/08 14:50:30 wodan Exp $
 * $Log: sqrt.c,v $
 * Revision 1.3  1999/09/08 14:50:30  wodan
 * changed to int version to use the float version, faster in most cases, if not all.
 *
 * Revision 1.2  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */
/**
 * This method attempts to find a square root of an input number.  This
 * returns a square root in the same type as the input type.
 * @see efun::sqrt()
 * @return the square root
 * @param number
 */
mixed sqrt( mixed number ) {
  if (floatp(number))
    return efun::sqrt(number);
  if (!intp(number))
    error("Bad argument 1 to sqrt()\nExpected: int or float Got: "+
          typeof(number)+ ".\n");
  if ( number < 0 )
    return -1;
  return to_int(efun::sqrt(to_float(number)));
} /* sqrt() */
