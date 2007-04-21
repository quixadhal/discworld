/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: flu.c,v 1.1 1998/01/06 04:08:27 ceres Exp $
 * $Log: flu.c,v $
 * Revision 1.1  1998/01/06 04:08:27  ceres
 * Initial revision
 * 
*/
/**
 * This is the effect skelton docs.  This effect
 * has a classification of "disease.infection.general.flu".
 * <p>
 * Describe the arguments in here.
 * <p>
 * This effect has a shadow associated with it.
 * @classification disease.infection.general.flu
 * @see help::effects
 */
#include <effect.h>
#include "path.h"
int query_secs_between_hb() { return 1; }

/** @ignore yes */
string query_classification() { return "disease.infection.general.flu"; }

/** @ignore yes */
int beginning(object player, mixed arg) {
   tell_object(player, "You shiver.\n");
   return arg;
}

int effect_heart_beat(object player, int hbnum, mixed arg, int id){
   /* test for the duration finished yet */

   if (hbnum == arg) return REMOVE_THIS_EFFECT;
   return arg;
}

/** @ignore yes */
void end(object player) {
   tell_object(player, "You feel altogether healthy again.\n");
}
/** @ignore yes */
string query_shadow_ob(){ return SHADOWS+"flu_shadow"; }
