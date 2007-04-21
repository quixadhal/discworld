/**
 * testchar command.  Adds a few useful properties to test characters.
 * @author Taffyd
 */

inherit "/cmds/base";

private mapping properties = ([ "no score" : 1, 
    "no delete" : 1, 
    "test character" : (: $1->query_name() :), 
    "authorised player" : 1 ]);

int cmd( object player, string status ) {
    foreach( string property, mixed value in properties ) {
        if ( status == "on" ) {

            if ( functionp( value ) ) {
                player->add_property( property, evaluate( value, 
                    this_player() ) );
            }
            else { 
                player->add_property( property, value );
            }
        }
        else {
            player->remove_property( property );
        }
    }

    if ( status == "on" ) {
        player->save_me();
        add_succeeded_mess( ({ "$I is now a test character.\n", "" }),
            ({ player }) );
    }
    else {
        add_succeeded_mess( ({ "$I is no longer a test character.\n", "" }),
            ({ player }) );
    }

    return 1;
} /* cmd() */

mixed *query_patterns() {
    return ({ "<indirect:player> {on|off}", (: cmd($1[0], $4[1]) :) });
} /* query_patterns() */

