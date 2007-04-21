#include <effect.h>

inherit "/std/effect_shadow";

varargs int add_property(string var, mixed val, int tim) {      
    if ( var == "player_title" ) {
        tell_object( player, "You are not allowed to have a player title.\n" );
        return 0;
    }

    return player->add_property( var, val, tim );
} /* add_property() */

mixed query_property( string prop ) {
    if ( prop == "player_title" ) {
        return 0;
    }

    return player->query_property( prop );
} /* query_property() */

void bing() { 
player->submit_ee2( player->sid_to_enum( id ), "spam", 5, EE_CONTINUOUS );
} /* bing() */
