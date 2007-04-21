
/**
 * This handler deals with complaints.  It manages posting to the "complaints"
 * board and numbering the posts sequentially (helps to follow threads if
 * anybody follows up the posts on the board).
 *
 * @see /std/room/inherit/complaints_room.c
 * @see /std/room/complaints_room.c
 * @author Arienne
 * @started Wed Apr  4 14:11:50 BST 2001
**/

#include <board.h>

#define SAVEFILE "/save/complaints/complaints_handler.o"
#define BOARD "complaints"

int _complaintNumber;

/**
 * @ignore
**/
void save_file() {
    unguarded( (: save_object( SAVEFILE ) :) );
} /* save_file() */

/**
 * @ignore
**/
void load_file() {
    unguarded( (: restore_object( SAVEFILE ) :) );
} /* load_file() */

/**
 * @ignore
**/
void create() {
    seteuid( getuid() );
    _complaintNumber = 1;
    load_file();
} /* create() */

/**
 * Submits a complaint to the complaints board anonymously.
 *
 * @param str The text of the complaint.
 * @return The note number, or 0 on failure.
**/
int do_submit_complaint( string str ) {
    int ret = 0;

    if ( str ) {
        ret = BOARD_HAND->
            add_message( BOARD, "Anonymous",
                         "Anonymous complaint #" + _complaintNumber,
                         sprintf( "%-=78s\n", str ) );

        _complaintNumber++;
        save_file();
    }
    
    return ret;
} /* do_submit_complaint() */

/**
 * @ignore
**/
int board_access_check( int type, string, object previous, string name ) {
    switch ( type ) {
    case B_ACCESS_READ:
        return ( "/secure/master"->query_lord( name ) );
        
    case B_ACCESS_WRITE:
        return ( ( previous == this_object() ) ||
                 ( "/secure/master"->query_lord( name ) ) );
        
    case B_ACCESS_DELETE :
        // Players can always delete their own notes, do not need to code
        // for that.
        return 0;
    }
} /* board_access_check() */
