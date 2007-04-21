/**
 * @main
 * The effect put on NPCs who are protected by Unseen University's Porters.
 *
 * The argument is an array containing, in this order, the attack string, 
 * (what the  NPC does/says when attacked), the enter string (what the 
 * NPC does/says when a criminal enters), the warning string (how the NPC 
 * warns the guards), and the integer severity of attacking the NPC. 
 * (Usually 1, but potentially more for "better" NPCS).
 * The strings will be passed right into init_command, so they will need to 
 * have the appropriate command at the front. 
 * (for example, ":cowers away in terror."). Optionally, zeroes may be passed 
 * to leave out that string.
 * There is no timing on this effect because, well, that's just silly.
 *
 * @classification npc.protection.wizards
 * @author Rodion
 */

#include <effect.h>

#define SHADOWS "/std/shadows/npc/"
#define SHADOW_OB  "uu_staff_protection"

/** @ignore */
string query_classification(){return "npc.protection.wizards"; }

/** @ignore */
void beginning(object player, mixed arg){
   player->submit_ee( 0, -1, EE_REMOVE );
}/*beginning()*/

/** @ignore */
string query_shadow_ob() {
   return (SHADOWS SHADOW_OB);
} /* query_shadow_ob() */

// If a new effect is added, the arguments from it will replace those of the 
// prior effect.
/** @ignore */
mixed merge_effect(object player, mixed old_arg, mixed new_arg){
   return new_arg;
}/*merge_effect()*/

/** @ignore */
int query_indefinite() { return 1; }
/*A SkankyCode Creation, by Rodion*/