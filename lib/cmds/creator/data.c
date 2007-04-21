/**
* @ignore
*/
#include <creator.h>

inherit "/cmds/base";


/**
 * This dumps out stuff about the object.
 */
int cmd(object *obs, string var) {
    object ob;

    foreach (ob in obs) {
        if ( userp(ob) && !master()->query_lord( this_player()->query_name() ) ) {
            write("You cannot dump a player object.\n");
            continue;
        }
        else {
            this_player()->more_string( WIZ_PRESENT->desc_object( ob ) + ":\n" +
              debug_info( 2, ob ) + "\n", "Data" );
        }
    }
    return 1;
} /* cmd() */

mixed *query_patterns() {
    return ({ "<indirect:wiz-present>", (: cmd($1, 0) :) });
         //"<indirect:wiz-present> <string'variable'>", (: cmd($1, $4[1]) :),
} /* query_patterns() */

