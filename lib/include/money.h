/**
 * This file contains all the data needed for handling money.  This
 * includes the money handler and the money object itself.
 * @author Pinkfish
 */
#ifndef __SYS__MONEY
/** @ignore yes */
#define __SYS__MONEY
#include <shops/bank.h>

/**
 * The object to use for money.
 */
#define MONEY_OBJECT "/obj/money"
/**
 * The money handler object, used for routines and opetations on money.
 */
#define MONEY_HAND "/obj/handlers/money_handler"
/**
 * The money alias, this is used by the money object to identify
 * itself.  The reason it has spaces in it is so that it is not
 * matched by find_match(), only by present().
 * @example
 * // Find the players money object.
 * money = present(MONEY_ALIAS, player);
 */
#define MONEY_ALIAS "Some Money For Me"

#define NO_MATCH -1
#define NO_MONEY -2
#define NO_LEGAL -3

/**
 * The money recycling room
 */
#define MONEY_VAULT "/room/vault"

/**
 * The index into the array returned by the make_payment functions for
 * the change.
 */
#define MONEY_PAY_CHANGE 1
/**
 * The index into the array returned by the make_payment functions for
 * the return array.  This is the actual value to fiddle with.
 */
#define MONEY_PAY_RETURN 0
/**
 * The index into the array returned by make_payment functions for the
 * the depleted money array.
 */
#define MONEY_PAY_DEPLETED 2

/**
 * The index into the array returned by query_details_for function
 * for the return array.  This is the word printed when the coin is 
 * flipped and it hands in its head.
 */ 
#define MONEY_DETAILS_HEADS 0

/**
 * The index into the array returned by query_details_for function
 * for the return array.  This is the word printed when the coin is 
 * flipped and it hands in its tail.
 */ 
#define MONEY_DETAILS_TAILS 1

/**
 * The index into the array returned by query_details_for function
 * for the return array.   This is what is contained on the reverse
 * of the coin.
 */ 
#define MONEY_DETAILS_REVERSE 2

/**
 * The index into the array returned by query_details_for function
 * for the return array.  This is what is contained on the obverse
 * of the coin
 */ 
#define MONEY_DETAILS_OBVERSE 3

/**
 * The index into the array returned by query_details_for function
 * for the return array.  This is what the coin is made of. 
 */ 
#define MONEY_DETAILS_COMPOSITION 4

/**
 * The index into the array returned by query_details_for function
 * for the return array.  This is what the plural for the coin
 * is.
 */
#define MONEY_DETAILS_PLURAL 5

#endif /* __SYS__MONEY */
