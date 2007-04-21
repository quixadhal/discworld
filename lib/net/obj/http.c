/**
 * A simple HTTP/1.0 complaint browser for retrieving web pages.
 * @author Taffyd
 * @started 13/11/2001
 */ 

#include <network.h>

#define DEFAULT_HTTP_PORT 80 

inherit CLIENT;
inherit "/std/object";

class http_connection { 
    object owner;
    string address;
    string ip_address;
    string path; 
    int port; 
    string data; 
}

mapping _connections = ([ ]);
mapping _dns_lookups = ([ ]); 

int do_http_get( string url ) {
    string protocol, full_host, hostname, path; 
    int port; 
    int key;
    int pos; 

    if ( sscanf( url, "%s://%s/%s", protocol, full_host, path ) != 3 ) {
        add_failed_mess( "Incomplete URL\n" );
        return 0;
    }

    if ( protocol != "http" ) {
        add_failed_mess( "This client only supports the HTTP protocol.\n" );
        return 0;
    }
    
    pos = strsrch( full_host, ":" );

    if ( pos != -1 ) {
        hostname = full_host[0 .. pos-1];
        port = to_int( full_host[pos+1..] );
    }
    else {
        hostname = full_host;
        port = DEFAULT_HTTP_PORT;
    }

    tell_object( this_player(), "Connecting to " + hostname + "...\n" );

    key = resolve( hostname, "got_ip_address" );
    _dns_lookups[ key ] = ({ this_player(), port, path }); 
    
    add_succeeded_mess( "" );
    return 1;
} /* do_http_get() */

void create() {
    client::create();
    SetSocketType(STREAM);
    object::create();
} /* create() */

void setup() { 
    set_name( "browser" );
    set_short( "http browser" );
    add_adjective( "http" );
    set_long( "This small object can be used to retrieve web pages using "
        "the 'http get' command.\n" );
    set_weight( 0 ); 
} /* setup() */ 

void close_socket( int fd ) {
    map_delete( _connections, fd );
    eventAbortCallback( fd );
    eventSocketClose( fd );
} /* close_socket() */

void init() { 
    add_command( "http", "get <string>", (: do_http_get( $4[0] ) :) );
} /* init() */ 

void eventRead(int fd, string message) {
    class http_connection tmp;

    tmp = _connections[ fd ];

    if ( tmp->owner ) {
        tell_object( tmp->owner, message );
    }
} /* eventRead() */ 

void got_ip_address( string address, string ip_address, int key ) {
    object player;
    string path;
    int port;
    int fd;

    tell_creator( "taffyd", "%O, %O, %O\n", address, ip_address, key );

    if ( undefinedp( _dns_lookups[ key ] ) ) {
        return;
    }
    
    player = _dns_lookups[ key ][ 0 ]; 
    port = _dns_lookups[ key ][ 1 ];
    path = _dns_lookups[ key ][ 2 ]; 
    
    map_delete( _dns_lookups, key );

    if ( !ip_address ) {
        tell_object( player, "Unable to resolve " + ip_address + ".\n" );
        return;
    }

    if ( sizeof( path ) == 0 ) {
        path = "/";
    }
    else {
        if ( path[0] != '/' ) {
            path = "/" + path;
        }
    }
    
    tell_object( player, 
        sprintf( "Connecting to %s on port %d.\n", ip_address, port ) );

    fd = eventCreateSocket( ip_address, port ); 
    
    if ( fd != -1 ) { 
        eventWrite( fd, sprintf( "GET %s HTTP/1.0\r\n\r\n", path ) ); 

        _connections[ fd ] = new( class http_connection, 
            owner: player,
            address: address,
            ip_address: ip_address,
            port: port,
            path: path, 
            data: "" );            
        call_out( "close_socket", 30, fd );
    }
} /* got_ip_address() */

void dest_me() {
    foreach( int fd, mixed data in _connections ) {
        close_socket( fd );
    }

    object::dest_me();
    client::dest_me();
} /* dest_me() */

int query_prevent_shadow() { 
    return 1;
} /* query_prevent_shadow() */

