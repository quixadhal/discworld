/**
 * This shadow is supposed to keep the room outside a hat
 * tent from unloading.
 */

inherit "/std/effect_shadow";

/* Make sure the outside room is not unloaded */
/** @ignore */
int query_keep_room_loaded() {
   return 1;
}

/** @ignore */
int dont_free() {
   return 1;
}

/** @ignore */
int clean_up() {
   return 1;
}

/**
 * This method should be called when a tent is collapsed.
 */
void tent_collapsed() {
   remove_this_effect();
} // tent_collapsed()
