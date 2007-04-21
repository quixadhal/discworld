/**
 * This effect keeps track of the hat tent shadow which
 * keeps rooms outside hat tents from unloading.
 */

#include <effect.h>

/** @ignore */
void beginning() {
   // Do nothing?
} // beginning()

// No merge effect!  We want multiple effects/shadows.

/** @ignore */
void end() {
   // Do nothing...
} // end()

/** @ignore */
string query_shadow_ob() {
   return "/std/shadows/room/hat_tent";
} // query_shadow_ob()

/** @ignore */
string query_classification() {
   return "room.hattent";
} // query_classification()
