/**
 * Mighty Gnaaaah command. 
 * @author Taffyd.
 */ 

inherit "/cmds/base";

// #define USE_WIZ_PRESENT 1

protected int cmd( string pattern, object *obs ) {
    string gn; 
    object ob;
    string verb = query_verb();

    if ( sizeof( explode( verb, "" ) - ({ "g", "n", "a", "h" }) ) ) {
        return 0;
    }

    gn = "gn" + implode( allocate( sizeof( pattern ) - 2, "a" ), "" ) + "h!";

    if ( arrayp( obs ) ) {
        obs = filter( obs, (: !$1->check_earmuffs( "remote-soul" ) :) );
        obs -= ({ this_player() });
        
         if ( sizeof( obs ) ) {
            tell_object( this_player(), "You go '$C$" + gn + "' at " + 
                query_multiple_short( obs ) + ".\n" ); 
                
            foreach (ob in obs) {
                tell_object(ob, this_player()->the_short() + " goes '$C$" +
                    gn + "' at you.\n" );
            }
        
            return 1;    
        }
    }
    
    write( capitalize( gn ) + "\n" );
    return 1;
} /* cmd() */

mixed *query_patterns() {
    mixed *pat = ({ "", (: cmd($6, 0) :) });

    if ( previous_object(1)->query_lord() ) {
        pat += ({ " <indirect:wiz-present>", (: cmd($6, $1) :) });
    }
    else {
        pat += ({ " <indirect:any-living>", (: cmd($6, $1) :),
                  " <indirect:object>", (: cmd($6, $1) :) });
    }
    
    return pat;
} /* query_patterns() */

