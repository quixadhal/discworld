varargs string ctime_elapsed( int time_elapsed, int mode ) {
    int sec, min, hour, day;
    string seconds, minutes, hours, days; 
    string *retval;

    sec = time_elapsed % 60;
    min = (time_elapsed / 60) % 60;
    hour = (time_elapsed / (60 * 60)) % 24;
    day = time_elapsed / (60 * 60 * 24);

    if ( mode ) {
        seconds = query_num( sec );
        minutes = query_num( min );
        hours = query_num( hour );
        days = query_num( day );
    }
    else {
        seconds = sec + "";
        minutes = min + "";
        hours = hour + "";
        days = day + "";
    }

    retval = ({ });

    if (day > 0) {
        retval += ({ days + " day" });
        if (day > 1) {
            retval[<1] += "s";
        }
    }

    if (hour > 0) {
        retval += ({ hours + " hour" });
        if (hour > 1) {
            retval[<1] += "s";
        }
    }

    if (min > 0) {
        retval += ({ minutes + " minute" });
        if (min > 1) {
            retval[<1] += "s";
        }
    }

    if (sec > 0) {
        retval += ({ seconds + " second" });
        if (sec > 1) {
            retval[<1] += "s";
        }
    }

    if (sizeof(retval) == 0) {
        return "0 seconds";
    }

    if (sizeof(retval) == 1) {
        return retval[0];
    }
    return implode(retval[0..<2], ", ") + " and " + retval[<1];
} /* ctime_elapsed() */

