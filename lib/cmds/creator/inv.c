#include <creator.h>

inherit "/cmds/base";

protected int cmd( object *obs ) {
    object ob;
    object *inventory;

    foreach( ob in obs ) {
        inventory = filter( all_inventory( ob ),
          (: reference_allowed( $1, $( this_player() ) ) :) );

        tell_object( this_player(), "Inv of " + WIZ_PRESENT->desc_object( ob ) +
          " in " + WIZ_PRESENT->desc_object( 
            environment( ob ) ) + ":\n" );
        tell_object( this_player(), implode( map( inventory,
              (: "  " + WIZ_PRESENT->desc_f_object( $1 ) :) ), "\n" ) + "\n" );

        if ( !sizeof( inventory ) ) {
            continue;
        }
    }
    return 1;
} /* cmd() */

mixed *query_patterns() {
    return ({ "[of] <indirect:wiz-present>", (: cmd($1) :) });
} /* query_patterns() */
