/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: extract.c,v 1.2 1998/03/24 07:28:59 pinkfish Exp $
 * $Log: extract.c,v $
 * Revision 1.2  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
varargs string extract(string str, int start, int end) {
  if (end)
    return str[start..end];
  return str[start..];
} /* extract() */
