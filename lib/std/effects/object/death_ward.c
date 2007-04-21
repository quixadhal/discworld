/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: death_ward.c,v 1.1 1998/01/06 04:19:27 ceres Exp $
 * $Log: death_ward.c,v $
 * Revision 1.1  1998/01/06 04:19:27  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "magic.ward.death".
 * <p>
 * Describe the arguments in here.
 * <p>
 * This effect has a shadow associated with it.
 * @classification magic.ward.death
 * @see help::effects
 */
#include "path.h"

/** @ignore yes */
string query_classification() { return "magic.ward.death"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS +"death_ward"; }

int query_indefinite() { return 1; }

/** @ignore yes */
int beginning( object thing, int amount, int id ) { return 0; }
