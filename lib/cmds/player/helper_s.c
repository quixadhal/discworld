/**
 * Helpers command, used to display which newbie helpers are online at 
 * the moment or to test if someone offline is a newbie helper.
 * <p>
 * Trial version.
 * <p>
 * @author Taffyd
 * @started 23/04/99 7:45PM
 */

#include <player_handler.h>

inherit "/cmds/base";

int cmd() {
    object *helpers;
    string mess;
        
    helpers = filter( users(), (: !$1->query_creator() && 
        $1->query_property( "newbie helper" ) &&
        !$1->query_login_ob() :) );
    
    switch( sizeof( helpers ) ) {
        case 0:
            add_failed_mess("There are no newbie helpers logged on at the "
                "moment.\n");
            return 0;
        
        case 1:
            if ( helpers[ 0 ] == this_player() ) {
                mess = "You are the only newbie helper logged on at the "
                    "moment.\n";
            }
            else {
                mess = helpers[ 0 ]->query_short() + " is the only newbie "
                    "helper logged on at the moment.\n";
            }
        break;
        
        default:
            mess = query_multiple_short( helpers->query_short() ) + " are "
                "newbie helpers.\n";
    }
   
    this_player()->add_succeeded_mess(this_object(), ({mess, ""}),
      ({ }));      
    return 1;
} /* cmd() */

int test_helper( string player_name ) {
    
    player_name = lower_case( player_name );
    
    if ( PLAYER_HANDLER->test_property( player_name, "newbie helper" ) ) {
        this_player()->add_succeeded_mess(this_object(), ({
          capitalize( player_name ) + " is a newbie helper.\n",
          ""}), ({ }));
        return 1;
    }
    
    add_failed_mess(capitalize( player_name ) + 
        " is not a newbie helper.\n" );
    return 0;
} 

mixed *query_patterns() {
    return ({ "", (: cmd :),
              "<string'name'>", (: test_helper( $4[ 0 ] ) :) });
} /* query_patterns() */
