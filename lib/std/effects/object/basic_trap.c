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
 * This effect has a classification of "object.trap.basic".
 * <p>
 * No arguments.
 * <p>
 * This effect has a shadow associated with it. It is a basic trap for a
 * container.
 * @classification object.trap.basic
 * @see help::effects
 */
#include "path.h"

/** @ignore yes */
string query_classification() { return "object.trap.basic"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS +"basic_trap"; }

int query_indefinite() { return 1; }

/** @ignore yes */
int beginning( object thing, int amount, int id ) {
	tell_object(find_player("ceres"), sprintf("Adding trap to %O\n",
																						thing));
	thing->set_lock_trap(thing, "trap_lock");
	return 0;
}
