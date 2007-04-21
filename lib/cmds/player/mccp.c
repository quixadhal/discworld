inherit "/cmds/base";

int cmd() { 
    object *players; 
    int compressed; 

    if ( compressedp( this_player() ) ) {
        tell_object( this_player(), "You are currently using MCCP to "
          "connect to " + mud_name() + ".\n" );
    }
    else {
        tell_object( this_player(), "You are not currently using MCCP to "
          "connect to " + mud_name() + ".  Please read 'help compression' for "
          "more information about MCCP.\n" );
    }

    players = filter( users(), (: !$1->query_login_ob() &&
        reference_allowed( $1, $(this_player()) ) :) ); 

    compressed = sizeof( filter( players, (: compressedp :) ) );
    
    tell_object( this_player(), "%^BOLD%^$C$" + query_num( compressed, 1000 ) +
                 " users (" + ((compressed * 100) / sizeof(players)) + "%) are "
                 "currently using MCCP and " + query_num(sizeof(players) - compressed, 1000) + " users (" + (((sizeof(players) - compressed) * 100) / sizeof(players)) + "%) are uncompressed.%^RESET%^\n" );

    return 1;
} /* cmd() */

mixed *query_patterns() { 
    return ({ "", (: cmd :) });
} /* query_patterns() */ 


