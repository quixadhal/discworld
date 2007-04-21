/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: find_other_call_out.c,v 1.2 1998/03/24 07:28:59 pinkfish Exp $
 * $Log: find_other_call_out.c,v $
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
 * This method allows you to find a call_out on another object.
 * @param ob the object to find the call out on
 * @param co the name of the call out to find
 * @return the same as from find_call_out()
 * @see efun::find_call_out()
 */
mixed find_other_call_out(object ob, string co){
  return evaluate(bind((: find_call_out($(co)) :), ob));
}
