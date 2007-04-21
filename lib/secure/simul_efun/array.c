/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: array.c,v 1.3 2002/05/12 21:24:51 wodan Exp $
 * $Log: array.c,v $
 * Revision 1.3  2002/05/12 21:24:51  wodan
 * added all_environment() it returns the environment, it's environment etc
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
 * This method deletes and element from an array.  This is an old method and
 * should not be used any more.  The method can be replaced with the
 * code  "arr[0..start-1] + arr[start+len..]".
 *
 * @param arr the array to delete an element from
 * @param start the start position to delete from
 * @param len the number of elements to delete
 * @return the new array
 */
mixed delete(mixed arr, int start, int len) {
  return arr[0..start-1] + arr[start+len..];
} /* delete() */

/**
 * This method inserts an element into the array.  This method is obsolete
 * and should not be used.  It inserts an element into the array at the
 * specified point.
 * This function accepts strings as well as arrays.
 * <p>
 * The code to do this is: "arr[0..pos-1] + ({ el }) + arr[pos..]".
 *
 *
 * @param arr the array to insert the element into.
 * @param el the element to insert
 * @param pos the position at which to insert it
 * @return the array with the element inserted into it
 */
mixed insert(mixed arr, mixed el, int pos) {
  if (stringp(arr))
    return arr[0..pos-1] + el + arr[pos..];
  return arr[0..pos-1] + ({ el }) + arr[pos..];
} /* insert() */

//ok, so this is not the obvious place, didn't want to add yet another small inherited file

object *all_environment(object ob){
  object *ret = ({});

  if(!ob)
    ob = previous_object();
  while(environment(ob)){
    ob = environment(ob);
    ret += ({ob});
  }

  return ret;
}
