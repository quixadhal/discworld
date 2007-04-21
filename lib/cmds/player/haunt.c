/** 
 * Haunt command. 
 * @author Taffyd
 */

#define HAUNT_EARMUFF "remote-soul"

inherit "/cmds/base";

string ooo() {
    return implode( allocate( 3 + random( 15 ),
        (: ({ "O", "o" })[ random( 2 ) ] :) ), "" );
} /* ooo() */

int haunt_player( object *victims ) {
    victims = filter( victims, (: !sizeof( $1->query_ignoring( ({ this_player() }) ) ) :) );
    victims = filter( victims, (: !$1->check_earmuffs( HAUNT_EARMUFF, 
        $(this_player()) ) :) ); 
    victims = filter( victims, 
        (: interactive( $1 ) || ( $1->query_property( "npc" ) && 
        environment( $1 ) == environment( this_player() ) ) :) );

    if ( !this_player()->query_property("dead") ) {
        add_failed_mess( "You are nowhere near wispy enough to "
          "haunt people.  You could probably scare a table or "
          "chair, though.\n" );
        return -1;
    }

    if ( !sizeof( victims ) ) {
        return 0;
    }

    add_succeeded_mess( "$N wave$s $p arms about anxiously and say$s: \""
   + ooo() + "\" at $I.\n", victims );
    return 1;
} /* haunt_player() */


int cmd() {
    if ( !this_player()->query_property("dead") ) {
        add_failed_mess( "You are nowhere near wispy enough to "
          "haunt people.  You could probably scare a table or "
          "chair, though.\n" );
        return -1;
    }
    add_succeeded_mess( "$N wave$s $p arms about anxiously and intone$s: \"" +
        ooo() + "\".\n" );
    return 1;
} /* cmd() */

mixed *query_patterns() {
    return ({ "", (: cmd() :),
      "<indirect:any-living>", (: haunt_player($1) :)  });
} /* query_patterns() */
