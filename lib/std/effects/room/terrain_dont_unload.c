#include <effect.h>

/**
* This is the effect skelton docs.  This effect
* has a classification of "terrain.dont.unload".
* p
* Describe the arguments in here.
* p
* This effect has a shadow associated with it.
* @classification terrain.dont.unload
* @see help::effects
*/

/** @ignore
*/
string query_classification() {return "terrain.dont.unload"; }
/** @ignore */
string query_shadow_ob() {return "/std/shadows/room/terrain_dont_unload"; }

/** @ignore */
mixed beginning( object player, mixed *arg) {
    player->setup_shad();
    return arg;
}

/** @ignore */
mixed merge_effect( object player, mixed *old, mixed *newish ) {
    player->setup_shad();
    return newish;
}
