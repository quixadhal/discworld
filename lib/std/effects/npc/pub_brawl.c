/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: pub_brawl.c,v 1.1 1998/01/06 04:16:50 ceres Exp $
 * $Log: pub_brawl.c,v $
 * Revision 1.1  1998/01/06 04:16:50  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "npc.pub_brawl".
 * <p>
 * Describe the arguments in here.
 * <p>
 * This effect has a shadow associated with it.
 * @classification npc.pub_brawl
 * @see help::effects
 */
#include <effect.h>

#include "path.h"

/*
 * Do something when something leaves the room....
 * 
 * The argument set on this one...  If the is a function pointer it is
 * evaluated.  If it is array, the function in position 1 is called on
 * object in position 0. 
 */
/** @ignore yes */
string query_classification() { return "npc.pub_brawl"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS + "pub_brawl"; }
