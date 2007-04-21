/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: inh_eritance.c,v 1.3 2002/11/09 02:41:00 ceres Exp $
 * $Log: inh_eritance.c,v $
 * Revision 1.3  2002/11/09 02:41:00  ceres
 * Fixed error if part isn't loaded
 *
 * Revision 1.2  2001/10/23 18:04:23  taffyd
 * Updated to use wiz-present to make it easier to find out the inheritance of objects. ie: inh tree in taffyd
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 * 
*/
#include <creator.h>

inherit "/cmds/base";

private string inherith(string ob, string ind, string leaf);

private int cmd(string fn, string fn2) {
    object ob, ob2;

    // notify_fail("Syntax: inh[eritance] <filename> [<filename>]\n");

    // I'd like to use WIZ_PRESENT here, but I'm not sure how to handle
    // the optional filename at the end.

    fn = this_player()->get_path( fn ); 

    if (!( ob = find_object( fn ))) {
        add_failed_mess( "No such object: " + fn + "\n" );
        return 0;
    }

    if ( fn2 ) { 
        fn2 = this_player()->get_path( fn2 ); 

        if (!( ob2 = find_object( fn2 ))) {
            add_failed_mess( "No such object: " + fn2 + "\n" );
            return 0;
        }
    }

    fn = file_name(ob);
    write(fn + "\n" + inherith(fn, "  ", fn2 ));
    return 1;
} /* cmd() */ 

/* Print inherit hierarchy */
private string inherith(string ob, string ind, string leaf) {
    string *fn, rtn = "", s;
    int i;

    if(!load_object(ob))
      return ind + "unable to find " + ob + "\n";
    fn = inherit_list(find_object(ob));
    for (i = 0; i < sizeof(fn); i++) {
        //write("*Scanning " + fn[i] + "\n");
        if (leaf && (find_object(fn[i]) == find_object(leaf))) {
            //write("**Leaf match\n");
            rtn += ind + fn[i] + "\n";
        } else {
            //write("**No leaf match\n");
            s = inherith(fn[i], ind + "  ", leaf);
            if (!leaf || (s != "")) {
                //write("***Good branch\n");
                rtn += ind + fn[i] + "\n" + s;
            }
        }
    }
    return rtn;
} /* inherith() */ 

// Experimental wiz present interface to this command. 
// Argle.
//
// x0r
int wiz_present_cmd( mixed indirect_obs ) { 
    object fo, fo2; 
    string fn, fn2; 
    if ( !arrayp( indirect_obs ) ) {
        return 0;
    }

    if ( sizeof( indirect_obs ) < 1 ) { 
        return 0;
    }

    if ( arrayp( indirect_obs[0] ) ) { 
        fo = indirect_obs[0][0];

        if ( sizeof( indirect_obs ) == 2 ) {
            fo2 = indirect_obs[ 1 ][ 0 ]; 
        }
    }
    else {
        fo = indirect_obs[ 0 ];

        if ( sizeof( indirect_obs ) == 2 ) { 
            fo2 = indirect_obs[ 1 ]; 
        }
    }

    if ( fo ) {
        fn = file_name( fo );
    }

    if ( fo2 ) {
        fn2 = file_name( fo2 );
    }

    debug_printf( "%O, %O\n", fn, fn2 );

    return cmd( fn, fn2 ); 
} /* wiz_present_cmd() */ 

public mixed *query_patterns() { 
    return ({ 
              "<word> [with] <word>", (: cmd( $4[0], $4[1] ) :),              
              "<word>", (: cmd( $4[0], 0 ) :),
              "<indirect:wiz-present> with <indirect:wiz-present>", (: wiz_present_cmd( $1 ) :),
              "<indirect:wiz-present>", (: wiz_present_cmd( $1 ) :),
    });
} /* query_patterns() */
