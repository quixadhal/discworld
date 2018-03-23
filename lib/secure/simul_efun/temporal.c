/*  -*- LPC -*-  */

/*
 * This is a set of simul_efuns that deal with
 * time and date functionality.
 */

#include <localtime.h>
varargs string timestamp(int the_time, int use_local) {
    string ret;
    string zone;
    mixed *bits;

    if(undefinedp(the_time))
        the_time = time();

    bits = localtime(the_time);
    if( !use_local ) {
        // Add GMT offset if we aren't using local time
        bits = localtime(the_time + bits[LT_GMTOFF]);
        zone = "GMT";
    } else {
        zone = bits[LT_ZONE];
    }
    ret = sprintf("%04d-%02d-%02d %02d:%02d:%02d %s",
            bits[LT_YEAR], bits[LT_MON]+1, bits[LT_MDAY],
            bits[LT_HOUR], bits[LT_MIN], bits[LT_SEC],
            zone);
    return ret;
}

