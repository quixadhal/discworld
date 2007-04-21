/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: plant.c,v 1.1 1998/08/06 02:20:31 jeremy Exp $
 *
 *
 */

/*
 * plant.c - base hidden object for gathering plants.
 */

inherit "/obj/gatherable";

void setup() {
    set_name("plant");
    // The short is a problem.  Currently it only shows up if they
    // try to "get" it (I think), which is handled in /obj/gatherable.
    set_short("hidden plant");
    set_long("There are all sorts of herbs and flowers here.  Use "
             "\"gather\" to collect specific types.\n");
    set_get_failure_mess("There are all sorts of flowers and herbs "
                         "around here.  Use \"gather\" to get "
                         "specific varieties.\n");
}
