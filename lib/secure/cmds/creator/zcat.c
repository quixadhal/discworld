
#include <creator.h>
#include <runtime_config.h>

#define TP globals[fd]
#define RET ret[fd]

#define CMD_NUM 9

inherit "/cmds/base";

private mapping globals = ([]), files = ([]), ret = ([]);

int cmd( string fname, int list ) {
    string *files;
    string euid; 
    string args;
    int fd;
    int count; 

    files = this_player()->get_files( fname ); 

    if ( !sizeof( files ) ) { 
        files += ({ this_player()->get_path( fname ) }); 
    }
    
    euid = geteuid( this_player(1) ); 
    
    // Place any default arguments in here.
    args = "--stdout";

    if ( list ) { 
        args += " --list";
    }

    files = uniq_array( files ); 
    count = 0;

    foreach( string file in files ) { 
        if ( !master()->valid_read( file, euid, "cat" ) ) { 
            tell_object( this_player(), "You don't have write access to "
                +  file + ".\n" ); 
        }
        else {
            args += " " + file[1..];
            ++count;
        }
    }

    if ( count ) {   
        log_file("/d/admin/log/ZCAT", ctime(time()) + ": " + 
                 this_player()->query_name() + " " + args + "\n");
        fd = external_start( CMD_NUM, args, "read_call_back", 
            "write_call_back", "close_call_back" ); 
    }
    else { 
        add_failed_mess( "zcat must have at least one argument.\n" );
        return -1;
    }

    if ( fd == -1 ) {
        add_failed_mess( "zcat failed to start.\n" );
        return -1;
    }
    
    TP = this_player(); 
    RET = ""; 

    tell_object( this_player(), "zcat started.\n" );
    return 1; 
} /* cmd() */ 

void read_call_back(int fd, mixed mess) {
    int max_len; 
    int new_total;

    mess = replace(mess, "/bin/", "");
    max_len = get_config( __MAX_STRING_LENGTH__ ); 

    new_total = strlen( RET ) + strlen( mess ); 

    if ( new_total > max_len ) { 
        mess = mess[ 0.. (new_total - max_len - 1) ];
    }

    RET += mess;
} /* read_call_back() */ 

void write_call_back( int fd ) {
    tell_object(TP, "zcat: A fatal error has occurred -- write_call_back "
        "was called.\n");
} /* write_call_back() */ 

void close_call_back(int fd) {
    if ( RET != "" ) { 
        if ( catch( TP->more_string(strip_colours(RET)) ) ) { 
            efun::tell_object( TP, RET );             
        }
    }
    else { 
        tell_object(TP, "zcat finished.\n");
    }
    
    map_delete(ret, fd);
    map_delete(globals, fd);
}

mixed *query_patterns() { 
    return ({ "<string>", (: cmd( $4[0], 0 ) :),
              "{-l|--list|list} <string>", (: cmd( $4[1], 1 ) :)       
    });
} /* query_patterns() */ 
