/**
 * This file defines the array offsets for use in the am_time functions
 * and the length of things like days, minutes and seconds in the
 * am calendar.  It also defines the names of the months and days
 * and their corresponding values.
 * @see /obj/handlers/am_time_handler
 */

#ifndef __AM_TIME_H
/** @ignore yes */
#define __AM_TIME_H

/**
 * Thgis is the pointer to the handler for am time functions.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_HANDLER "/obj/handlers/am_time_handler"

/**
 * This is the month index into the am_time_array() return value.
 * @example
 * mixed *arr;
 *
 * arr = AM_TIME_HANDLER->am_time_array(time());
 * write("We are in the " + arr[AM_TIME_MONTH] + " month.\n");
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_MONTH 0
/**
 * This is the day of month index into the am_time_array() return value.
 * @example
 * mixed *arr;
 *
 * arr = AM_TIME_HANDLER->am_time_array(time());
 * write("We are in the " + arr[AM_TIME_DAY_OF_MONTH] + " day of month.\n");
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_DAY_OF_MONTH 1
/**
 * This is the day of week index into the am_time_array() return value.
 * @example
 * mixed *arr;
 *
 * arr = AM_TIME_HANDLER->am_time_array(time());
 * write("We are in the " + arr[AM_TIME_DAY_OF_WEEK] + " day of week.\n");
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_DAY_OF_WEEK 2
/**
 * This is the hour index into the am_time_array() return value.
 * @example
 * mixed *arr;
 *
 * arr = AM_TIME_HANDLER->am_time_array(time());
 * write("Time is " + arr[AM_TIME_HOUR] + ":" + arr[AM_TIME_MINUTE]".\n");
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_HOUR 3
/**
 * This is the minute index into the am_time_array() return value.
 * @example
 * mixed *arr;
 *
 * arr = AM_TIME_HANDLER->am_time_array(time());
 * write("Time is " + arr[AM_TIME_HOUR] + ":" + arr[AM_TIME_MINUTE]".\n");
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_MINUTE 4
/**
 * This is the year index into the am_time_array() return value.
 * @example
 * arr = AM_TIME_HANDLER->am_time_array(time());
 * write("The year is " + arr[AM_TIME_YERAR] + ".\n");
 */
#define AM_TIME_YEAR 5
/**
 * This is the primus index into the am_time_array() return value.
 * A year is primus if it is in the first 400 days, not the second.
 * @example
 * arr = AM_TIME_HANDLER->am_time_array(time());
 * write("The year is " + arr[AM_TIME_YERAR] + ".\n");
 */
#define AM_TIME_PRIMUS 6


/* The month name defines. */

/**
 * This is the value associated with the month of Offle in the 
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_OFFLE 1
/**
 * This is the value associated with the month of Feburary in the 
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_FEBURARY 2
/**
 * This is the value associated with the month of March in the 
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_MARCH 3
/**
 * This is the value associated with the month of April in the 
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_APRIL 4
/**
 * This is the value associated with the month of May in the 
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_MAY 5
/**
 * This is the value associated with the month of June in the 
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_JUNE 6
/**
 * This is the value associated with the month of Grune in the 
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_GRUNE 7
/**
 * This is the value associated with the month of August in the 
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_AUGUST 8
/**
 * This is the value associated with the month of Spune in the 
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_SPUNE 9
/**
 * This is the value associated with the month of Sektober in the 
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_SEKTOBER 10
/**
 * This is the value associated with the month of Ember in the 
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_EMBER 11
/**
 * This is the value associated with the month of December in the 
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_DECEMBER 12
/**
 * This is the value associated with the month of Ick in the 
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_ICK 13

/* This is the defines for the days of the week. */

/**
 * This is the value associated with the day Monday in the
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_MONDAY 1
/**
 * This is the value associated with the day Tuesday in the
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_TUESDAY 2
/**
 * This is the value associated with the day Wednessday in the
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_WEDNESSDAY 3
/**
 * This is the value associated with the day Thursday in the
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_THURSDAY 4
/**
 * This is the value associated with the day Friday in the
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_FRIDAY 5
/**
 * This is the value associated with the day Saturday in the
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_SATURDAY 6
/**
 * This is the value associated with the day Sunday in the
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_SUNDAY 7
/**
 * This is the value associated with the day Octeday in the
 * am time calendar.
 * @see /obj/handlers/am_time_handler
 */
#define AM_TIME_OCTEDAY 8

/* the length of time defines and stuff. */

/**
 * The number of days in each year in the Ankh-Morpork calendar.
 */
#define AM_DAYS_PER_HALF_YEAR 400
/**
 * The numbers of seconds per minute in the Ankh-Morpork calenda.
 */
#define AM_SECONDS_PER_MINUTE 18
/**
 * The numbers of minutes per hour in the Ankh-Morpork calenda.
 */
#define AM_MINUTES_PER_HOUR 60
/**
 * The numbers of minutes per hour in the Ankh-Morpork calenda.
 */
#define AM_HOURS_PER_DAY 24
/**
 * The number of days per week in the Ankh-Morpork calenda.
 */
#define AM_DAYS_PER_WEEK 8
/**
 * The numbers of minutes per day in the Ankh-Morpork calenda.
 */
#define AM_MINUTES_PER_DAY (AM_MINUTES_PER_HOUR * AM_HOURS_PER_DAY)
/**
 * The numbers of seconds in each day in the Ankh-Morpork calendar.
 */
#define AM_SECONDS_PER_DAY (AM_MINUTES_PER_DAY * AM_SECONDS_PER_MINUTE)
/**
 * The numbers of seconds in each month in the Ankh-Morpork calendar.
 */
#define AM_SECONDS_PER_WEEK (AM_DAYS_PER_WEEK * AM_MINUTES_PER_DAY * AM_SECONDS_PER_MINUTE)
/**
 * The number of seconds per year in the Ankh-Morpork calendar.
 */
#define AM_SECONDS_PER_HALF_YEAR (AM_SECONDS_PER_DAY * AM_DAYS_PER_HALF_YEAR)

/**
 * The numbers of seconds per house in the Ankh-Morpork calendar.
 */
#define AM_SECONDS_PER_HOUR (AM_SECONDS_PER_MINUTE*AM_MINUTES_PER_HOUR)

/**
 * This is the offset added to the year when it is calculated from the
 * real year.
 */
#define AM_START_YEAR_DATE 1966

/**
 * A flag used to be passed into the format parameter for the amtime
 * function.  It turns off the displaying of the time; ie: 12:48am
 * disappears.
 */
#define AM_TIME_FORMAT_NO_TIME 1

/**
 * A class to hold all the information for am times.
 * @member year the year of the time
 * @member prime if the section is price (the first 400 days) or secundus
 * @member minute the minute of the time
 * @member hour the hour of the time
 * @member month the month of the time
 * @member day_of_week the day of the week
 * @member day_of_month the day of the month
 * @see /obj/handlers/am_time->query_am_time_class()
 */
class am_time {
   int year;
   int primus;
   int minute;
   int hour;
   int month;
   int day_of_week;
   int day_of_month;
   int day_of_year;
}

#endif
