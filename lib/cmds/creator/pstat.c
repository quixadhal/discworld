inherit "/cmds/base";

int cmd( object *indirect_obs ) {
    object ob;
    mapping prop;
    string *sorted, property;
    string txt;

    txt = "$P$Properties$P$";

    foreach( ob in indirect_obs ) {
        prop = filter( ob->query_properties(), (: $2 :) );
        sorted = sort_array( keys( prop ), 1 );
        foreach( property in sorted ) {
            txt += sprintf( "$I$0,5=(%s): %O$I$0,-5=\n", property, prop[ property ] );
        }
    }

    tell_object( this_player(), txt );
    return 1;
} /* cmd() */

mixed *query_patterns() {
    return ({ "<indirect:wiz-present>", (: cmd( $1 ) :) });
} /* query_patterns() */

