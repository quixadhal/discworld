/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: uniq_array.c,v 1.2 1998/03/24 07:28:59 pinkfish Exp $
 * $Log: uniq_array.c,v $
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
 * This method will return an array that contains no duplicates.
 *  uniq_array.c
 *  Written by: Wodan
 * <p>
 *  This function will return an array that contains no duplicates.
 *  Gotta love them mappings. :)
 * @param arr the array to convert
 * @return an array with no duplicates
 */
mixed *uniq_array(mixed *arr){
  return keys(allocate_mapping(arr, 1));
} /* uniq_array() */
