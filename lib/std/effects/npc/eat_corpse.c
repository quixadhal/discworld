/**
 * This effect tears small edible parts from the corpse (bits with no sub 
 * bits) and eats them immediately.  It should be used instead of the
 * savage_corpse effect if the NPC is too small to rip apart a corpse
 * completely.
 * @author Lemming
 * @started 3/8/2000
 * @classification npc.eat.savage
 * @see /std/effects/npc/savage_corpse.c
 * @see /std/effects/npc/eat_edible.c
 */

#include <effect.h>

#include "path.h"

/** @ignore */
void beginning( object player, string dest, int id ) {
} /* beginning() */

/** @ignore */
int merge_effect( object player, int time1, int time2, int id ) {
   return time1;
} /* merge_effect() */
 
/** @ignore */
string query_classification() { return "npc.eat.corpse"; }

/** @ignore */
string query_shadow_ob() { return SHADOWS + "eat_corpse"; }

/** @ignore */
int query_indefinate() { return 1; }
