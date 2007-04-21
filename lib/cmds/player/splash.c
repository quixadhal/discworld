/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: splash.c,v 1.7 2001/09/24 01:43:51 presto Exp $
 * $Log: splash.c,v $
 * Revision 1.7  2001/09/24 01:43:51  presto
 * Oopd, should only be able to splash on the surface
 *
 * Revision 1.6  2001/09/24 01:31:53  presto
 * Can splash in water rooms too
 *
 * Revision 1.5  1999/07/23 10:33:39  taffyd
 * Made it so that you could splash people if they were in a bath house. :)
 *
 * Revision 1.4  1998/09/27 21:44:17  ceres
 * Fixed splashing yourself.
 *
 * Revision 1.2  1998/04/14 02:38:07  pinkfish
 * Changed to use add_command.
 *
 * Revision 1.1  1998/01/06 05:29:43  ceres
 * Initial revision
 * 
*/
/* make command for making snowballs and snowmen. Moved from /global/pweath.c
 * by Ceres 24/3/96
 */
inherit "/cmds/base";
#include <weather.h>

#define TP this_player()

mixed cmd(object *obs) {
    object env, ob;
    string str;

    env = environment(TP);

    if ( !env->query_baths()  &&  (!env->query_water()  ||  !env->query_surface()) ) {
        if ((string)env->query_property("location") == "inside")
            return notify_fail("You must be outside to do that.\n");

        if (!WEATHER->query_raining(env))
            return notify_fail("It must be raining to splash someone.\n");
    }

    foreach(ob in obs) {
        ob->add_effect("/std/effects/other/wetness", 2 + random(4));
        if(ob == this_player())
            continue;

        tell_object(ob, (string)TP->a_short()+" splashes "+
          query_multiple_short(obs - ({ ob }) + ({ "you" }))+".\n");
    }


    str = query_multiple_short(obs);
    write("You splash "+str+".\n");
    say(TP->a_short()+" splashes "+str+".\n", obs);
    return 1;
} /* cmd() */

mixed *query_patterns() {
    return ({ "<indirect:living>",(: cmd($1) :) });
} /* query_patterns() */
