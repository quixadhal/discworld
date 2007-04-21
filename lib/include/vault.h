/**
 * The basic include file for vaults.
 */
#ifndef __VAULT_H
/** @ignore yes */
#define __VAULT_H

#include <money.h>

/**
 * The basic cost of a vault.
 */
#define FEE_AMT 100
/**
 * The amount of a vault.
 */
#define INIT_AMT 2000

//#define FEE_AMT_STRING "twenty five pence"
/**
 * Turn the basic cost into a string based on the current
 * currency of the room.
 */
#define FEE_AMT_STRING (string)(MONEY_HAND->money_value_string(100, this_object()->query_property("place")))
     
//#define INIT_AMT_STRING "five dollars"
/**
 * Turn the initial cost into a string based on the current
 * currency of the room.
 */
#define INIT_AMT_STRING (string)(MONEY_HAND->money_value_string(2000, this_object()->query_property("place")))

/**
 * The save directory for vaults.
 */
#define VAULT_SAVE "/save/vaults/"

#endif
