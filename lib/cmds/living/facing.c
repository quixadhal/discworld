/**
 * This command is used to tell what direction the player or a living 
 * object is facing.
 * @author Taffyd
 * @started 28/03/99 1:02AM
 */
#include <dirs.h>

inherit "/cmds/base";

string find_direction( object who );
int check_reference_point( object where );

/** @ignore yes */
int cmd( object *indirect_obs ) {
    mapping details;
    object *oblist;
    string *show, dir;
    
    if ( !check_reference_point( environment( this_player() ) ) ) {
        add_failed_mess( "You can't make out a point of reference "
            "to use to determine which way you're facing!\n" );
        return 0;
    }
    
    
    if ( !indirect_obs || sizeof( indirect_obs ) == 1 && 
        indirect_obs[ 0 ] == this_player()) {
        tell_object( this_player(), "You are facing " +
            find_direction( this_player() ) + ".\n" );
        return 1;
    }   
    
    indirect_obs -= ({ this_player() });
    
    details = unique_mapping( indirect_obs, 
        (: find_direction( $1 ) :) );
    
    /* This doesn't quite work, you get:
       A is facing north and B and C is facing south.  */
//    details = map( details, (: query_multiple_short( $2 ) +
//         " $V$0=is,are$V$ facing " + $1 :) );

    /* So we will do this instead... -- Presto, 31 May 2000 */
    show = ({ });
    foreach(dir, oblist in details)  {
      if (sizeof(oblist) > 1)  {
        show += ({ query_multiple_short(oblist) + " are facing " + dir });
      }
      else  {
        show += ({ oblist[0]->one_short() + " is facing " + dir });
      }
    }
    write(query_multiple_short(show) + ".\n");

    return 1;
} /* cmd() */

/** @ignore yes */
string find_direction( object who ) {
    int facing;
    mapping directions;
    
    facing = who->query_facing()[ 0 ];
    directions = filter( ABS_FACING, (: $2 == $(facing) :) );
    
    if ( !sizeof( directions ) )
        return "north";
    
    return keys( directions )[ 0 ];
} /* find_direction() */

int check_reference_point( object where ) {
    string *relatives;
    
    relatives = map( where->query_direc(), 
        (: $(where)->query_relative( $1 ) :) );
    
    return member_array( 0, relatives ) > -1;
} /* check_reference_point() */


/** @ignore yes */
mixed *query_patterns() {
    return ({ "", (: cmd( 0 ) :),
        "<indirect:living:here>", (: cmd($1) :) });
} /* query_patterns() */

