/**
 * The include file to deal with player housing.
 * @author Ceres
 */
#ifndef __SYS__HOUSING
/** @ignore yes */
#define __SYS__HOUSING

/** The location of the housing handler.  */
#define HOUSING "/obj/handlers/housing"
/** The location of the rental agreement. */
#define AGREEMENT "/obj/misc/agreement"
/** The location of the housing owners guide. */
#define BOOK "/obj/misc/home_owners_guide"

// These are the house ownership types.
/** Define the house ownership type as being a sale. */
#define SALE 0
/** Define the house ownership type as being a rental. */
#define RENT 1
/** Define the house ownership type as being a leasehold */
#define LEASE 2

// These are the ownership timeouts

// 1 discworld year
/** The amount of time for the owner to time out. */
#define OWNER_TIMEOUT 11520000
// 6 discworld months
/** The amount of time for the renter to time out. */
#define RENTER_TIMEOUT  5760000
// 2 discworld years
/** The default length of a lease. */
#define DEFAULT_LEASE 23040000

#define GET_COMMANDS ({"get", "take", "drag", "palm", "gather", "order", "grab", "ask"})

#endif /* __SYS__HOUSING */
