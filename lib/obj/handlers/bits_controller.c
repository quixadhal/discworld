/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: bits_controller.c,v 1.2 1998/01/06 22:22:10 sin Exp $
 * $Log: bits_controller.c,v $
 * Revision 1.2  1998/01/06 22:22:10  sin
 * made_the_do_update_routine_more_resilient,_added_a_kickstart_routine.
 *
 * Revision 1.1  1998/01/06 05:03:33  ceres
 * Initial revision
 * 
*/
/**
 * Decays all the decable objects in the game.
 * @author Ember
 * @changed 17 Dec 1997 -- Sin
 *    I added a reset_eval_cost() call into the do_update() routine
 *    so that it wouldn't quit when there were a whole bunch of
 *    bits to decay.
 * @changed 6 Jan 1998 -- Sin
 *    I modified the do_update() routine so it'll register a callback
 *    even if one of the bits fails to decay.  That should help prevent
 *    the callout from disappearing.  If it does disappear, I added
 *    a kickstart function.
 * @see /obj/food.c
 * @see /std/bit.c
 * @see /obj/corpse.c
 */
#include <bits_controller.h>
object *bits;

private void do_update();

void create() {
  bits = ({ });
} /* create() */

/**
 * Adds in a decable object into ther decaying array.
 * @param ob the bit to decay
 */
void add_bit(object ob) {
  if (!sizeof(bits))
    call_out( (: do_update() :), TIME_OUT);
  bits += ({ ob });
} /* add_bit() */

/**
 * Removes the decable object from the decaying array.
 * @param ob the bit to remove
 */
void remove_bit(object ob) {
  int i;

  if ((i = member_array(ob, bits)) == -1)
    return ;
  bits = delete(bits, i, 1);
} /* remove_bit() */

private void do_update() {
  int i;

  bits = bits - ({ 0 });
  if (sizeof(bits))
    call_out((: do_update() :), TIME_OUT);
  for (i = sizeof(bits) - 1; i > -1; i--) {
    reset_eval_cost();
    bits[i]->do_decay();
  }
  bits = bits - ({ 0 });
} /* do_update() */

int query_num_bits() { return sizeof(bits); }
object *query_bits() { return bits; }
void kickstart() {
  do_update();
}
