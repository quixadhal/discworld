/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: spell_effects.c,v 1.1 1998/01/06 04:16:50 ceres Exp $
 * $Log: spell_effects.c,v $
 * Revision 1.1  1998/01/06 04:16:50  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "npc.spell_effects".
 * <p>
 * Describe the arguments in here.
 * <p>
 * This effect has a shadow associated with it.
 * @classification npc.spell_effects
 * @see help::effects
 */
#include <effect.h>

#include "path.h"

/*
 * Do something when an npc died.
 * 
 * The argument set on this one...  If the is a function pointer it is
 * evaluated.  If it is array, the function in position 1 is called on
 * object in position 0.  If the array is three long, and the last
 * arguement is non 0 then the second_life funciton is not called up
 * the object list...
 */
/** @ignore yes */
string query_classification() { return "npc.spell_effects"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS + "spell_effects"; }
