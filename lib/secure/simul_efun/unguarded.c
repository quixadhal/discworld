/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: unguarded.c,v 1.3 2000/06/10 01:55:00 pinkfish Exp $
 * $Log: unguarded.c,v $
 * Revision 1.3  2000/06/10 01:55:00  pinkfish
 * Remove the spurious nightmare message.
 *
 * Revision 1.2  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/

#include <origin.h>

/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */
/**
 * This does an unguarded apply.  An unguarded apply means it chops off
 * the call tree at this point for security purposes.  This is needed to be
 * done in places where access beyond what the caller may have is needed.
 * For example, the board handler uses this to save the messages away as
 * the players and creators do not have access to save into these directories
 * normally.
 * <p>
 * @param f the fucntion pointer to evaluate unguarded.
 * @return returns the value of the evaluated function pointer
 */
mixed unguarded(function f) {
  object master_ob = master();

  if (!master_ob) {
    master_ob = previous_object();
  }
  if (origin() == ORIGIN_LOCAL) {
    return master_ob->apply_unguarded(f, 1);
  } else {
    return master_ob->apply_unguarded(f);
  }
return 1;
} 
