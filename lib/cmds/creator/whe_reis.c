/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: whe_reis.c,v 1.3 2001/03/10 06:15:38 shrike Exp $
 * $Log: whe_reis.c,v $
 * Revision 1.3  2001/03/10 06:15:38  shrike
 * fixing a long-standing (query_terano()?"design flaw":"bug") with the way objects in null-space are handled.
 *
 * Revision 1.2  2000/04/03 13:39:40  taffyd
 * Tried to fix a lord invis bug..
 *
 * Revision 1.1  1998/01/06 05:25:41  ceres
 * Initial revision
 *
*/
/* whereis command, trial out by Turrican for a commands daemon. */

#include <creator.h>


inherit "/cmds/base";

int cmd( object *indirect_obs ) {
    object ob, e;
    int described;
    string txt;

    if ( !sizeof( indirect_obs ) ) {
        add_failed_mess( "Where is what?\n" );
        return 0;
    }

    txt = "$P$Whereis$P$";

    foreach ( ob in indirect_obs ) {
        if ( ob->query_hidden() )
            continue;

        described = 0;
        e = ob;

        while ( e = environment( e ) ) {
            if ( interactive( e ) && !reference_allowed( e, this_player() ) )
                break;

            if ( !described ) {
                txt += WIZ_PRESENT->desc_object(ob) + " is : \n";
                described = 1;
            }

            txt += "  in " + WIZ_PRESENT->desc_f_object(e) + "\n";
        }
        if ( !described )
            txt += WIZ_PRESENT->desc_object(ob) + " is in Null-space.\n";
    }

    tell_object( this_player(), txt + "\n" );
    return 1;
} /* cmd() */

mixed *query_patterns() {
    return ({ "<indirect:wiz-present>", (: cmd($1) :) });
} /* query_patterns() */