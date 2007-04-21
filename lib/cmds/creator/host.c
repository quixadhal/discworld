/*  -*- LPC -*-  */

/*
** The equivalent of the nslookup or host commands in unix.
** -- Arienne, 10/Mar/2001
*/

inherit "/cmds/base";

mapping lookups = ([]);

int cmd( string w ) {
    string ip = lower_case( this_player()->expand_nickname( w ) );
    string ip2;
    
    if ( find_player( ip ) ) {
        ip2 = query_ip_number( find_player( ip ) );
        lookups[ resolve( ip2, "lookup_callback" ) ] = this_player();
        tell_object( this_player(), "Starting lookup for " + ip +
                     " (" + ip2 + ").\n" );
    } else {
        lookups[ resolve( ip, "lookup_callback" ) ] = this_player();
        tell_object( this_player(), "Starting lookup for " + ip + ".\n" );
    }
    
    return 1;

} // cmd()

void lookup_callback( string address, string resolved, int key ) {
    if ( lookups[ key ] ) {
        tell_object( lookups[ key ], "Resolved " +
                     ( address ? address : "?unknown" ) +
                     " to " +
                     ( resolved ? resolved : "?unknown" ) + ".\n" );
    }
    map_delete( lookups, key );
} // lookup_callback


mixed *query_patterns() {
   return ({ "<string'ip-address, host or player'>",
                 (: cmd( $4[0] ) :) });
} /* query_patterns() */

