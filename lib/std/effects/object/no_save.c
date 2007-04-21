/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: no_save.c,v 1.1 2000/04/24 21:18:27 pinkfish Exp $
 * $Log: no_save.c,v $
 * Revision 1.1  2000/04/24 21:18:27  pinkfish
 * Initial revision
 *
 * Revision 1.1  1998/01/06 04:19:27  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "mudlib.owned.weapon".
 * <p>
 * Describe the arguments in here.
 * <p>
 * This effect has a shadow associated with it.
 * @classification mudlib.owned.weapon
 * @see help::effects
 */
#include "path.h"

/** @ignore yes */
string query_classification() { return "mudlib.object.nosave"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS +"no_save"; }

int query_indefinite() { return 1; }

/** @ignore yes */
void beginning( object thing, string owner, int id ) { return; }

/** @ignore yes */
string merge_effect( object thing, string old_owner, string new_owner,
      int id ) {
   return old_owner;
} /* merge_effect() */
