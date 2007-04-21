/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: callouts.c,v 1.4 2003/03/27 11:38:02 taffyd Exp $
 * $Log: callouts.c,v $
 * Revision 1.4  2003/03/27 11:38:02  taffyd
 * Rewritten for easy maintainence of various (legacy?) functions, added a new "before|at|after" time syntax.
 *
 * Revision 1.3  2000/05/16 21:41:27  ceres
 * Modified so it doesn't try sorting if there's more than 1000 callouts -- this caused over evals :(
 *
 * Revision 1.2  1999/10/28 02:00:15  ceres
 * improved filtering
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/

#define CALL_OUT_SORT_THRESHOLD 1000
#define CALL_OUT_FILE_NAME 0
#define CALL_OUT_FUNCTION 1
#define CALL_OUT_TIME 2

inherit "/cmds/base";


private mixed *get_call_outs() {
    mixed *junk;

    junk = call_out_info();
    
    // Only sort the array if we have less than CALL_OUT_SORT_THRESHOLD
    // elements in the array.
    if (sizeof(junk) < CALL_OUT_SORT_THRESHOLD) {
        junk = sort_array( junk, (: ( $1[ 2 ] > $2[ 2 ] ) -  
            ( $1[ 2 ] < $2[ 2 ] ) :) );
    } 
    
    return junk; 
} /* get_call_outs() */ 

int count_callouts() {
    mixed *call_outs = get_call_outs();
    mapping count = ([ ]);
    string ret = ""; 

    foreach( mixed *func in call_outs) {
        count[ file_name( func[ CALL_OUT_FILE_NAME ] ) + ":" + 
            func[ CALL_OUT_FUNCTION ] ] += 1; 
    }
    
    foreach( string key, int value in count ) {
        ret += sprintf( "%-30s %d\n", key, value );
    }
    
    this_player()->more_string( ret, "Callouts" ); 
    return 1; 
} /* count_callouts() */ 

int count_clean() {
    mixed *call_outs = get_call_outs();
    string ret = ""; 

    foreach( mixed *func in call_outs) {
        if ( base_name( func[CALL_OUT_FUNCTION] ) != "effect_timer" &&
          base_name( func[CALL_OUT_FUNCTION] ) != "print_messages" ) { 
            ret += sprintf( "%O: %O in %O\n", 
                        file_name( func[ CALL_OUT_FILE_NAME ] ),
                        func[ CALL_OUT_FUNCTION ], func[ CALL_OUT_TIME ] );
        }
    }

    this_player()->more_string( ret, "Callouts" ); 
    return 1; 
} /* count_clean() */ 

int count_for_file( string str ) {
    mixed *call_outs = get_call_outs();
    string ret = ""; 

    foreach( mixed *func in call_outs) {
        if ( sscanf( file_name( func[ 0 ] ), "%*s"+ str +"%*s" ) == 2 ) { 
            ret += sprintf( "%O: %O in %O\n", 
                file_name( func[ CALL_OUT_FILE_NAME ] ),
                func[ CALL_OUT_FUNCTION ], func[ CALL_OUT_TIME ] );
        }
    }

    this_player()->more_string( ret, "Callouts" ); 
    return 1; 
} /* call_out_for_file() */ 

int count_time(string choice, int mtime ) {
    mixed *call_outs = get_call_outs();
    string ret = ""; 

    foreach( mixed *func in call_outs) {
        if ( choice == "before" && func[ CALL_OUT_TIME ] >= mtime ) {
            continue;
        } else if ( choice == "after" && func[ CALL_OUT_TIME ] <= mtime ) {
            continue;
        } else if ( choice == "at" && func[ CALL_OUT_TIME ] != mtime ) { 
            continue;
        }

        ret += sprintf( "%O: %O in %O\n", func[ CALL_OUT_FILE_NAME ],
            func[ CALL_OUT_FUNCTION ], func[ CALL_OUT_TIME ] );
    }

    if ( ret == "" ) {
        add_failed_mess( "No callouts matching that criteria found.\n" );
        return -1; 
    }

    this_player()->more_string( ret, "Callouts" ); 
    return 1;
} /* count_time() */ 

int cmd() {
    mixed *call_outs = get_call_outs();
    string ret = ""; 

    foreach( mixed *func in call_outs) {
        ret += sprintf( "%O: %O in %O\n", func[ CALL_OUT_FILE_NAME ],
            func[ CALL_OUT_FUNCTION ], func[ CALL_OUT_TIME ] );
    }

    this_player()->more_string( ret, "Callouts" ); 
    return 1; 
} /* cmd() */

mixed *query_patterns() {
    return ({ "{-c|count}", (: count_callouts :), 
              "{-b|no effects}", (: count_clean :),
              "<string'file'>", (: count_for_file( $4[0] ) :), 
              "{before|at|after} <number'time'>", 
                (: count_time($4[0], $4[1]) :),
              "", (: cmd :), 
    }); 

} /* query_patterns() */ 