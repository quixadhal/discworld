/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: str_inven.c,v 1.2 1998/03/24 07:28:59 pinkfish Exp $
 * $Log: str_inven.c,v $
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
 * This method returns an inventory sorted on descriptions.
 * The format of the return array is:
 * <pre>
 * ({
 *    "desc", ({ ob, ... }),
 *    "desc2", ({ ob11, ob12, ... })
 *    ...
 *  })
 * </pre>
 * @param arr the array to get the inventory for
 * @return an array structured as above
 */
mixed *query_strange_inventory(mixed *arr) {
  mixed *inv, desc;
  object ob;
  int i;

  inv = ({ });
  foreach (ob in arr) {
    if (!(desc = (string)ob->short()) || (desc == ""))
      continue;
    if ((i = member_array(desc, inv)) >= 0)
      inv[i+1] += ({ ob });
    else
      inv += ({ desc, ({ ob }) });
  }
  return inv;
} /* query_strange_inventory() */
