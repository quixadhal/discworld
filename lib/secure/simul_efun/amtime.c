/*  -*- LPC -*-  */
/*
 * $Locker:  $
 * $Id: amtime.c,v 1.13 2003/03/25 20:02:49 wodan Exp $
 * $Log: amtime.c,v $
 * Revision 1.13  2003/03/25 20:02:49  wodan
 * Forcibly released due to inactivity
 *
 * Revision 1.12  2001/08/21 23:52:21  pinkfish
 * Put in some pre-defs to setop the simul_efun object from crashing on the
 * reboot.
 *
 * Revision 1.11  2001/08/21 19:26:39  pinkfish
 * Remove the debug prints.
 *
 * Revision 1.10  2001/08/21 19:25:29  pinkfish
 * Add in sousjagne's code for a query_time_string.
 *
 * Revision 1.9  2001/07/22 08:43:11  pinkfish
 * Fix up the time.
 *
 * Revision 1.8  2001/07/21 01:28:49  pinkfish
 * Fix up the amtime to take a second argument for the format.
 *
 * Revision 1.7  1998/06/27 09:10:25  sojan
 * Added the alternative am_time function for purely aesthetic reasons.
 *
 * Revision 1.6  1998/03/24 07:28:59  pinkfish
 * Add some docs to make the simulefuns more usefully lookupable.
 *
 * Revision 1.5  1998/03/19 12:41:45  pinkfish
 * Changed it to use the new handler object
 *
 * Revision 1.4  1998/03/19 11:21:43  pinkfish
 * Fixed up the time array stuff...
 *
 * Revision 1.3  1998/03/19 11:10:39  presto
 * Whatever changes presto made.
 *
 * Revision 1.2  1998/03/06 10:55:49  pinkfish
 * Stuf ot deal with the am time stuff..
 *
 * Revision 1.1  1998/01/06 05:13:15  ceres
 * Initial revision
 * 
*/
#include <am_time.h>

/**
 * Cotains some simul_efuns.
 * @author Pinkfish
 */

string query_num(int num);
#if !efun_defined(query_multiple_short)
varargs string query_multiple_short( mixed *args, string type,
                                     int no_dollars, int quiet,
                                     int dark );
#endif
/**
 * Adds a "rd", "th", "st" onto a number.  Takes a number as an input and
 * adds the ordinal type on it.  So you get 3rd and 2nd etc as return
 * strings.
 *
 * @param number the number to add the ordinal too
 * @return a string being the number plus the ordinal
 */
string ordinal( int number ) {
   if ( ( number % 100 > 10 ) && ( number % 100 < 14 ) )
      return number +"th";
   if ( number % 10 == 1 )
      return number +"st";
   if ( number % 10 == 2 )
      return number +"nd";
   if ( number % 10 == 3 )
      return number +"rd";
   return number +"th";
} /* ordinal() */ 
   
/**
 * This method returns the amtime for the input time number.  This function takes
 * the same input as the efun ctime().  You pass in the time number and
 * it returns a string detailing the time in ankh morpork.
 *
 * @param number the time number
 * @return a string containing the amtime
 */
varargs string amtime( int number, int format ) {
   return AM_TIME_HANDLER->query_am_time(number, format);
} /* amtime() */

/**
 *  This function will take an integer and turn it into a pretty string
 *  based on the various and sundry options you pass to it.  You pass the
 *  variable t to the function, which represents the number of seconds
 *  you want to turn into a string.  max_elements is the first option, and
 *  it specifies how many elements you would like to return.  It will
 *  default to two, and -1 will mean "any number of elements".<p>
 *
 *  am_time is a boolean value which tells the function how to interpret
 *  the 't' variable.  If you pass true (1) then it will interpret the
 *  seconds as game seconds, if you pass false (0), then it will interpret
 *  them as real-life seconds.<p>
 *
 *  Finally, use_words will determine whether or not the function will use
 *  numbers (e.g., 2 years and 24 hours) or words (e.g., two years and
 *  twenty-four hours).<p>
 *
 *  @param t how many seconds to turn into a string
 *  @param max_elements the maximum number of time elements to return
 *  @param am_time whether to interpret t as game time or real time
 *  @param use_words whether to return numbers or words
 *
 *  @example query_time_string (86410) will return "1 day and 10 seconds"
 *  @example query_time_string (86461) will return "1 day and 1 minute"
 *  @example query_time_string (86461, -1) will return "1 day, 1 minute,
 *   and 1 second"
 *  @example query_time_string (86461, -1, 0, 1) will return "one day,
 *   one minute, and one second"
 *
 *  @author Sousjagne
 *
 */

#include <am_time.h>

varargs string query_time_string (int t, int max_elements, int am_time,
                                  int use_words) {
  string *toret = ({ }), tmp_string;
  int year, day, hour, minute, tmp_int;

  if (am_time) {
    year = AM_SECONDS_PER_HALF_YEAR * 2;
    day = AM_SECONDS_PER_DAY;
    hour = AM_SECONDS_PER_HOUR;
    minute = AM_SECONDS_PER_MINUTE;
  }
  else {
    year = 60 * 60 * 24 * 365;
    day = 60 * 60 * 24;
    hour = 60 * 60;
    minute = 60;
  }

  if (t >= year) {
    toret = ({ toret ..., sprintf ("%d %s", (t / year),
      (t >= year * 2 ? "years" : "year")) });
    t %= year;
    year = -1; // placeholder
  }

  if (t >= day) {
    toret = ({ toret ..., sprintf ("%d %s", (t / day),
      (t >= day * 2 ? "days" : "day")) });
    t %= day;
    day = -1;
  }
  else if (year == -1) {
    toret = ({ toret ..., "0 days" });
  }

  if (t >= hour) {
    toret = ({ toret ..., sprintf ("%d %s", (t / hour),
      (t >= hour * 2 ? "hours" : "hour")) });
    t %= hour;
  }
  else if (day == -1) {
    toret = ({ toret ..., "0 hours" });
  }

  if (t >= minute) {
    toret = ({ toret ..., sprintf ("%d %s", (t / minute),
      (t >= minute * 2 ? "minutes" : "minute")) });
    t %= minute;
  }
  else if (hour == -1) {
    toret = ({ toret ..., "0 minutes" });
  }

  if (t > 0) {
    toret = ({ toret ..., sprintf ("%d %s", t,
      (t > 1 ? "seconds" : "second")) });
  }

  if (!max_elements) {
    max_elements = 2;
  }

  if (max_elements > 0) {
    toret = toret[0..(max_elements - 1)];
  }

  // Lastly, convert all numbers to strings if we need to, and remove all
  // zeroes from the array.
  for (int x = 0; x < sizeof (toret); ++x) {
    sscanf (toret[x], "%d %s", tmp_int, tmp_string);

    if (tmp_int == 0) {
      toret -= ({ toret[x] });
    }
    else if (use_words) {
      toret[x] = query_num (tmp_int) + " " + tmp_string;
    }
  }

  if (!toret || !sizeof (toret)) {
    return "no time at all";
  }

  return query_multiple_short (toret);
} /* query_time_string() */
