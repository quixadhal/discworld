/**
 * This is the include file for the data handler. The main bits of the data
 * handler were written by Jeremy.  The lisp like compiler for the data
 * was added by Pinkfish March 10th 2002.
 * @author Pinkfish
 */

#ifndef __DATA_HANDLER_H
/** @ignore yes */
#define __DATA_HANDLER_H

/**
 * This is the path to the data handler. 
*/
#define DATA_HANDLER "/obj/handlers/data"

/**
 * This is the class used for dealing with dice like structures in the
 * the lisp like data structure compiler.
 * @element die the die to roll
 * @element number the number of times to roll it
 * @element modifier the amount to be added or subtracted from the result
 */
class data_dice {
   int die;
   int number;
   int modifier;
}

#endif
