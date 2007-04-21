/**
 * This curse stops a player from having a player 
 * title.
 * @author Taffyd
 */

#include <effect.h>
#include "path.h"

void beginning( object ob, int time, int id ) {
    if ( time > 0 ) {
        ob->submitt_ee( 0, time, EE_REMOVE );
    }
} /* beginning() */ 

void end( object ob, int time, int id ) {
    // nothing.
} /* end() */

string query_classification() {
    return "player.no-title-allowed";
} /* query_classification() */

string query_shadow_ob() {
    return SHADOWS + "no_player_title";
} /* query_shadow_ob() */

void spam( object player, mixed args, int id ) {
tell_creator ("taffyd", "Spam\n" );
} /* spam() */
