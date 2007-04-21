/**
 * List of defines for the various typoes of move failures.  So you can figure
 * out why the move failed (if this is important to you).
 */
#ifndef __SYS__MOVE_FAILURES
/**
 * @ignore yes
 */
#define __SYS__MOVE_FAILURES

/**
 * The move succeeded.
 */
#define MOVE_OK 0
#define MOVE_EMPTY_DEST 1
/**
 * The remove function returned a fail result when the object was attempted
 * to be removed.  This can be changed with the set_get and reset_get functions.
 *
 * @see /std/basic/move->set_get()
 * @see /std/basic/move->reset_get()
 */
#define MOVE_NO_GET 2
/**
 * Unable to remove the object from the players inventory.  This is controlled
 * by the set_drop and reset_drop functions.
 *
 * @see /std/basic/move->set_drop()
 * @see /std/basic/move->reset_drop()
 */
#define MOVE_NO_DROP 3
/**
 * The object ius too heavy to go into the inventory.
 *
 * @see /std/basic/misc->set_weight()
 * @see /std/basic/misc->query_weight()
 */
#define MOVE_TOO_HEAVY 4
#define MOVE_INVALID_DEST 5
#define MOVE_NO_UNHOLD 6

#endif /* __SYS__MOVE_FAILURES */
