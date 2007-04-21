/**
 * Fetch command to retrieve objects from the broken room for players.
 * @author Taffyd
 * @started 29/01/0 2:43AM
 */
#include <move_failures.h>
#include <playerinfo.h>

#define BROKEN_ROOM "/room/broken"

inherit "/cmds/base";

int cmd( mixed *indirect_obs ) {
    object *broken_items, *failed, *success;
    object player;
    
    if ( sizeof( indirect_obs ) != 2 ) {
        return 0;    
    }
    
    if ( sizeof( indirect_obs[ 1 ] ) > 1 ) {
        add_failed_mess( "You cannot $V an object for more than one "
            "player at once.\n" );
        return 0;
    }
    player = indirect_obs[ 1 ][ 0 ];    
    
    broken_items = indirect_obs[ 0 ];
    
    failed = filter( broken_items,
        (: $1->move( $(player), "$N appear$s in your inventory." ) != MOVE_OK :) );
    
    success = broken_items - failed;
    
    if ( sizeof( failed ) ) {
        tell_object( this_player(), "Warning-- could not move " + 
            query_multiple_short( failed ) + " to " + 
            player->the_short() + ".\n" );
    }
    
    if ( sizeof( success ) ) {
        PLAYERINFO_HANDLER->add_entry(
            this_player(), player->query_name(), "replace", 
            "Fetched " + implode( success->short(), "," ) + " from /room/broken.");
        
        add_succeeded_mess( "$N $V $I for " + player->the_short() + " from "
            "the broken room.\n", success );
        return 1;
    }

    add_failed_mess( "No objects could be retrieved for $I.\n", 
        ({ player }) );
    return -1;
} /* cmd() */

mixed *query_patterns() {
    return ({ "<indirect:object:" + BROKEN_ROOM + "> [for] <indirect:player>", 
        (: cmd($1) :) });
} /* query_patterns() */
