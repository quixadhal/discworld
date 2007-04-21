/**
 * This is the stat adjustment inheritable will add stat
 * adjustments to the player.  It will also keep track of who they are added to
 * so they can be removed correctly.
 * @author Pinkfish
 * @started Sun May  7 14:41:09 PDT 2000
 */
#include <effects/stat_adjustment.h>

private nosave mapping _adjs;

void create() {
   _adjs = ([ ]);
} /* create() */

/**
 * This method adds a set of stat adjustments to a specified player
 * <pre>
 *  ([
 *     "str" : 1,
 *     "dex" : -1,
 *     "wis" : 3
 *  ])
 * </pre>
 * @param player the player to add the adjustments too
 * @param adjs the adjustments to add
 */
void set_stat_adjustment(object player, mapping adjs) {
   _adjs[player] = adjs;
   player->add_effect(STAT_ADJUSTMENT_EFFECT, ({ this_object(), adjs }));
} /* add_stat_adjustment() */

/**
 * This method removes the stat adjustments from the specified player
 * @param player the player to remove the stat adjustments from
 */
void remove_stat_adjustment(object player) {
   STAT_ADJUSTMENT_EFFECT->remove_stat_adjustment_ob(player, this_object());
   map_delete(_adjs, player);
} /* remove_stat_adjustment() */

/**
 * This method removes all the stat adjusments currently setup on
 * the object.
 */
void remove_all_stat_adjustments() {
   mapping adj;
   object play;

   foreach (play, adj in _adjs) {
      STAT_ADJUSTMENT_EFFECT->remove_stat_adjustment_ob(play, this_object());
      map_delete(_adjs, play);
   }
   _adjs = ([ ]);
} /* remove_all_stat_adjustments() */

/**
 * This method returns the current stat adjustments on the specified
 * player.
 * @param play the player to find the stat adjustments for
 * @return the stat adjustments
 */
mapping query_stat_adjustments(object play) {
   return _adjs[play];
} /* query_stat_adjustments() */
