/**
 * Include file for reaction handler.
 * This file includes some useful defines for parameters passed to
 * function pointers.
 * To use these, you'd do $OBJ_A instead of $1, etc.
 * @author Jeremy
 */

#ifndef __REACTION_H
/** @ignore yes */
#define __REACTION_H

/**
 * The class used in the reaction handler.
 *
 * @element ratio Amount of first substance needed; can be a float
 * (a:b) or an array of two floats (min/max ratio)
 * @element results Resulting object path
 * @element result_amt Amount of result per total reagents (ie, if
 * 2 ounces of a and 3 ounces of b should make 5
 * ounces of result, this would be 1.0
 * @element message Message to be printed out at reaction (probably
 * obsolete; using 'func' below would be better)
 * @element func Array of call-by-value function strings or
 * function pointers
 */
class reaction {
    mixed ratio;        // Amount of first substance needed; can be a float
                        // (a:b) or an array of two floats (min/max ratio)
    string result;      // Resulting object path
    float result_amt;   // Amount of result per total reagents (ie, if
                        // 2 ounces of a and 3 ounces of b should make 5
                        // ounces of result, this would be 1.0
    string message;     // Message to be printed out at reaction (probably
                        // obsolete; using 'func' below would be better)
    mixed *func;        // Array of call-by-value function strings or
                        // function pointers
}

/*
 * Some useful defines for parameters passed to function pointers.
 * To use these, you'd do $OBJ_A instead of $1, etc.
 *
 *  OBJ_A    The first reagent in the reaction spec
 *  OBJ_B    The second reagent
 *  OBJ_X    The result of the reaction
 *  ENV      The environment in which the reaction is taking place
 *  ENV2     The environment of the environment
 *  AMT_A    The amount of obj_a that will be consumed
 *  AMT_B    The amount of obj_b that will be consumed
 *
 * These last two were necessary because the amounts aren't actually
 * consumed until after the function pointers are called.  So
 * $OBJ_A->query_amount() returns the original amount, and $AMT_A
 * is the amount actually used.
 */

/** The first reagent in the reaction spec. */
#define OBJ_A   1
/** The second reagent in the reaction spec. */
#define OBJ_B   2
/** The result of the reaction. */
#define OBJ_X   3
/** The environment in which the reaction is taking place. */
#define ENV     4
/** The environment of the environment. */
#define ENV2    5
/**
 * The amount of obj_a that will be consumed.
 * These last two were necessary because the amounts aren't actually
 * consumed until after the function pointers are called.  So
 * $OBJ_A->query_amount() returns the original amount, and $AMT_A
 * is the amount actually used.
 */
#define AMT_A   6
/**
 * The amount of obj_b that will be consumed.
 * These last two were necessary because the amounts aren't actually
 * consumed until after the function pointers are called.  So
 * $OBJ_A->query_amount() returns the original amount, and $AMT_A
 * is the amount actually used.
 */
#define AMT_B   7

#endif
