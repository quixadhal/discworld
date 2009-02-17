/*
 * $Id: finde_rrors.c,v 1.4 2003/05/01 01:37:47 pinkfish Exp $
 */
/* finderrors command -*- LPC -*-
 * Handles bugreports in a collection of dirs for a creator 
 */

#include <log.h>
#include <board.h>
#include <mail.h>

inherit "/cmds/base";

// #define HELPER "/w/taffyd/handlers/finderror_helper"
#define HELPER "/obj/handlers/finderror_helper"
#define ERROR_CMD "/cmds/creator/errors"


private void finished_count_errors( object player, int status, mixed data ) {
    int *count; 
    string *paths; 
    mapping errors = ([ ]); 
    int *total = ({ 0, 0, 0 });
    string txt; 

    // tell_creator( player, "%O, %O\n", status, data );

    foreach( mapping direc in data ) {
        count = errors[ direc[ "Directory" ] ];

        if ( undefinedp( count ) ) { 
            count = ({ 0, 0, 0 });
        }

        switch( direc[ "Type" ] ) {
            case "BUG":
                count[ 0 ] = direc[ "COUNT(Id)" ]; 
                total[ 0 ] += count[ 0 ];
            break;
            case "TYPO":
                count[ 1 ] = direc[ "COUNT(Id)" ]; 
                total[ 1 ] += count[ 1 ];
            break;
            case "IDEA":
            default:
                count[ 2 ] = direc[ "COUNT(Id)" ]; 
                total[ 2 ] += count[ 2 ];
        }

        errors[ direc[ "Directory" ] ] = count; 
    } 

    paths = sort_array( keys( errors ), 1 ); 
    
    txt = "$P$finderrors$P$BUGS TYPO IDEA\n"; 



    foreach( string key in paths ) {
        txt += sprintf( "%4d %4d %4d %s\n", errors[ key ][ 0 ],
            errors[ key ][ 1 ], errors[ key ][ 2 ], key ); 
    } 
    
    txt += "-------------\n"; 
    txt += sprintf( "%4d %4d %4d\n", 
        total[ 0 ], total[ 1 ], total[ 2 ] ); 

    tell_object( player, txt );
    
} /* finished_count_errors() */ 

private int count_errors() { 
    int result;
    
    result = HELPER->query_dirs_count( this_player(), 
        (: finished_count_errors :) ); 

    if ( result ) { 
        add_succeeded_mess( ({ "Retrieving error count, this may take several "
            "seconds.\n", "" }) ); 
        return 1;
    }

    add_failed_mess(  "You have no finderrors directories defined.  "
        "Add them with 'finderrors add'\n" ); 
    return -1;
} /* count_errors() */ 

private void finished_next_dir( object player, int status, mixed data ) {
    // tell_creator( player, "%O, %O\n", status, data );

    if ( sizeof( data ) != 1 || !mapp( data[0] ) ) { 
        tell_object( player, "Unable to find the next directory "   
            "with errors.\n" );
    }
    else { 
        //start_error_command( player, data[ 0 ][ "Directory" ] ); 
        ERROR_CMD->errors_in_dir(data[0]["Directory"], 0);
    }
} /* finished_next_dir() */ 

private int get_next_error() { 
    HELPER->query_next_dir( this_player(), (: finished_next_dir :) ); 
    add_succeeded_mess( ({ "Retrieving error count, this may take several "
        "seconds.\n", "" }) ); 
    return 1;
} /* cmd() */ 

private int list_error_directories() {
    string *dirs;

    dirs = HELPER->query_directories( this_player()->query_name() );

    if ( sizeof( dirs ) ) { 
        tell_object( this_player(), "$P$finderrors list$P$" + 
            implode( dirs, "\n" ) + "\n" ); 
        add_succeeded_mess( "" );
        return 1;
    }
    else {
        add_failed_mess( "No directories have been added to finderrors.\n" );
        return -1;
    }
} /* list_error_directories() */ 

private int add_directory( string directory, int recursive ) { 
    
    if ( directory[ <1 ] != '/' ) {
        directory += "/";
    }

    if ( file_size( directory ) != -2 ) { 
        add_failed_mess( directory + " is not a directory.\n" ); 
        return -1; 
    }
     
    HELPER->add_directory( this_player(), directory, recursive );
    return 1; 
} /* add_directory() */ 

private int remove_directory( string directory, int recursive ) { 
    int result;
    
    result = HELPER->remove_directory( this_player(), directory, recursive ); 

    if ( result ) {
        add_succeeded_mess( "Directory removed successfully.\n" );
        return 1;
    }
    
    add_failed_mess( "Could not remove directory.  Was it on your "
        "finderrors list?\n" );
    return -1;
} /* remove_directory() */ 

public mixed *query_patterns() { 
    return ({ 
              "add <word>", (: add_directory( $4[ 0 ], 0 ) :), 
              "add <word> recursive", (: add_directory( $4[ 0 ], 1 ) :),
              "remove <word>", (: remove_directory( $4[ 0 ], 0 ) :), 
              "remove <word> recursive", (: remove_directory( $4[ 0 ], 1 ) :),
              "list", (: list_error_directories :), 
              "count", (: count_errors :),
              "", (: get_next_error :) }); 
} /* query_patterns() */ 
