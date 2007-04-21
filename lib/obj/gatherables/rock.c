/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: rock.c,v 1.1 1998/08/06 02:20:31 jeremy Exp $
 *
 *
 */

/*
 * rock.c - base hidden object for gathering rocks.
 */

inherit "/obj/gatherable";

void setup() {
    set_name("rock");
    // The short is a problem.  Currently it only shows up if they
    // try to "get" it (I think), which is handled in /obj/gatherable.
    set_short("hidden rock");
    set_long("There are rocks of all shapes and sizes here.  Use "
             "\"gather\" to collect specific types.\n");
    set_get_failure_mess("There are all sorts of rocks and minerals "
                         "around here.  Use \"gather\" to get "
                         "specific varieties.\n");
}
