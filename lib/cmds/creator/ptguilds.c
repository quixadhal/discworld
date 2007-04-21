/**
 * PTGuilds command
 * @author Taffyd
 */

#include <player.h>

/** @ignore yes */
string format_guild_list( string guild_path, string *members ) {
    return "Members of the " + (guild_path->query_short() 
      ? guild_path->query_short() : "Misc.") + ": " + 
        query_multiple_short( sort_array( map( members, 
           (: capitalize( $1 ) :) ), 1 ) ) + ".";
} /* format_guild_list() */

/** @ignore yes */
int cmd( string guild ) {
    mapping data;
    string txt;
    string *members;
   
    data = PLAYTESTER_HAND->query_playtester_guilds();
    txt = "Playtester listing by Guild\n\n";
   
    if ( guild && ( members = data[ guild ] ) ) {
        txt = format_guild_list( guild, members );
    }
    else {
        txt += implode( values( map( data, 
            (: format_guild_list( $1, $2 ) :) ) ), "\n\n" );
    }

    tell_object( this_player(), txt + "\n" );
    return 1;
} /* cmd() */

/** @ignore yes */
mixed *query_patterns() {
    return ({ "", (: cmd(0) :),
              "<string'guild'>", (: cmd($4[0]) :) });
} /* query_patterns() */
