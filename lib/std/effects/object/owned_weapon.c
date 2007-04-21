/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: owned_weapon.c,v 1.1 1998/01/06 04:19:27 ceres Exp $
 * $Log: owned_weapon.c,v $
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
string query_classification() { return "mudlib.owned.weapon"; }

/** @ignore yes */
string query_shadow_ob() { return SHADOWS +"owned_weapon"; }

int query_indefinite() { return 1; }

/** @ignore yes */
void beginning( object thing, string owner, int id ) { return; }

/** @ignore yes */
string merge_effect( object thing, string old_owner, string new_owner,
      int id ) {
   write( "There is a bug with the ownership of "+
         (string)thing->the_short() +" ("+ file_name( thing ) +
         ")!  Please bug report it and contact a creator!\n" );
   return old_owner;
} /* merge_effect() */
