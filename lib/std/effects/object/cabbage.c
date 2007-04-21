
#include <effect.h>

/** @ignore yes */
void beginning( object player, int time, int id ) {
/* Commenting out these first messages because they appear almost before
** the player has eaten the cabbage.
**   tell_object( player, "A foul smell wafts from your nether regions.\n" );
**   tell_room( environment( player ), "A loud trumpet"
**   " comes from "+ (string)player->one_short() +
**   "'s backside.  The accompanying smell is quite pungent.\n", player );
*/
   player->submit_ee( "make_stink", ({ 20, 20 }), EE_CONTINUOUS );
   player->submit_ee( 0, time, EE_REMOVE );
} /* beginning() */

/** @ignore yes */
int merge_effect( object player, int time1, int time2, int id ) {
   player->submit_ee( 0, time1 + time2 - (int)player->expected_tt(),
         EE_REMOVE );
   return time1 + time2;
} /* merge_effect() */
 
/** @ignore yes */
void end( object player, int time, int id ) {
   tell_object( player, "Your stomach calms down at last.\n" );
} /* end() */
 
void make_stink( object player, int time, int id ) {
   tell_object( player, "A huge trumpeting noise comes from your "
   "rear, closely followed by a stench of boiled cabbage.\n" );
   tell_room( environment( player ), "The smell of sprouts and cabbages from "
   + (string)player->one_short() +
         " wafts over you and you find yourself thinking of school dinners."
         "\n", player );
} /* make_stink() */

/** @ignore yes */
string query_classification() { return "body.smell.scent"; }

string smell_string( object player, int time ) {
   return "the delicious doughnutty smell";
} /* smell_string() */
