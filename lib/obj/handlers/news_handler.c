/**
 * The News Handler
 * @author Taffyd
 * @started 19/09/2000
 */

#include <board.h>
#include <login_handler.h>

#define BOARD_NAME "announcements"

/**
 * Time in seconds before displaying a reminder message.
 */
#define DELAY_TIME 30

int has_unread_announcements( string player ) {
    mapping news_rc;
    mixed subjects;

    news_rc = BOARD_HAND->query_newsrc( player );

    if ( !news_rc ) {
        news_rc = ([ ]);
    }

    subjects = BOARD_HAND->get_subjects( BOARD_NAME );

    if ( !sizeof( subjects ) ) {
        return 0;
    }

    return subjects[ <1 ][ B_TIME ] > news_rc[ BOARD_NAME ];
} /* has_unread_announcements() */

void show_message( mixed person ) {
    object player;
   
    if ( stringp( person ) ) {
        player = find_player( person );
    }
    else {
        player = person;
    }

    if ( !player ) {
        return;
    }

    if ( has_unread_announcements( player->query_name() ) ) {
        tell_object( player, "\nThere are %^YELLOW%^NEW%^RESET%^ "
            "announcements!  You can read them with \"news\".\n\n" );
    }
} /* show_message() */

void login_event(mixed person, string type) {
    
    if ( type == LOGIN || type == RECONNECT ) {
        if ( objectp( person ) ) {
            person = person->query_name();
        }
        
        call_out( (: show_message :), DELAY_TIME, person );
    }
} /* login_event() */

void broadcast_announcement() {   
    foreach( object user in users() ) { 
        if ( interactive( user ) ) { 
            show_message( user );
        }
    }

} /* broadcast_announcement() */


