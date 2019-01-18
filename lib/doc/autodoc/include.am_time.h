.DT
am_time.h
Disk World autodoc help
am_time.h

.SH Description
.SP 5 5

This file defines the array offsets for use in the am_time functions
and the length of things like days, minutes and seconds in the
am calendar.  It also defines the names of the months and days
and their corresponding values.
.EP
.SP 10 5

.EP



.SH See also
.SP 5 5
/obj/handlers/am_time_handler

.EP
.SH Classes

.SI 3
* %^BOLD%^am_time%^RESET%^
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

.EI
.SP 7 5

A class to hold all the information for am times.
.EP
.SI 7
%^BOLD%^Members:%^RESET%^
.EI
.SP 9 5
year - the year of the time
.EP
.SP 9 5
prime - if the section is price (the first 400 days) or secundus
.EP
.SP 9 5
minute - the minute of the time
.EP
.SP 9 5
hour - the hour of the time
.EP
.SP 9 5
month - the month of the time
.EP
.SP 9 5
day_of_week - the day of the week
.EP
.SP 9 5
day_of_month - the day of the month
.EP


.SH Defines
.SI 3
* %^BOLD%^AM_DAYS_PER_HALF_YEAR%^RESET%^
.EI
.SP 7 5

The number of days in each year in the Ankh-Morpork calendar.

.EP

.SI 3
* %^BOLD%^AM_DAYS_PER_WEEK%^RESET%^
.EI
.SP 7 5

The number of days per week in the Ankh-Morpork calenda.

.EP

.SI 3
* %^BOLD%^AM_HOURS_PER_DAY%^RESET%^
.EI
.SP 7 5

The numbers of minutes per hour in the Ankh-Morpork calenda.

.EP

.SI 3
* %^BOLD%^AM_MINUTES_PER_DAY%^RESET%^
.EI
.SP 7 5

The numbers of minutes per day in the Ankh-Morpork calenda.

.EP

.SI 3
* %^BOLD%^AM_MINUTES_PER_HOUR%^RESET%^
.EI
.SP 7 5

The numbers of minutes per hour in the Ankh-Morpork calenda.

.EP

.SI 3
* %^BOLD%^AM_SECONDS_PER_DAY%^RESET%^
.EI
.SP 7 5

The numbers of seconds in each day in the Ankh-Morpork calendar.

.EP

.SI 3
* %^BOLD%^AM_SECONDS_PER_HALF_YEAR%^RESET%^
.EI
.SP 7 5

The number of seconds per year in the Ankh-Morpork calendar.

.EP

.SI 3
* %^BOLD%^AM_SECONDS_PER_HOUR%^RESET%^
.EI
.SP 7 5

The numbers of seconds per house in the Ankh-Morpork calendar.

.EP

.SI 3
* %^BOLD%^AM_SECONDS_PER_MINUTE%^RESET%^
.EI
.SP 7 5

The numbers of seconds per minute in the Ankh-Morpork calenda.

.EP

.SI 3
* %^BOLD%^AM_SECONDS_PER_WEEK%^RESET%^
.EI
.SP 7 5

The numbers of seconds in each month in the Ankh-Morpork calendar.

.EP

.SI 3
* %^BOLD%^AM_START_YEAR_DATE%^RESET%^
.EI
.SP 7 5

This is the offset added to the year when it is calculated from the
real year.

.EP

.SI 3
* %^BOLD%^AM_TIME_APRIL%^RESET%^
.EI
.SP 7 5

This is the value associated with the month of April in the 
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_AUGUST%^RESET%^
.EI
.SP 7 5

This is the value associated with the month of August in the 
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_DAY_OF_MONTH%^RESET%^
.EI
.SP 7 5

This is the day of month index into the am_time_array() return value.
.EP

.SI 3
* %^BOLD%^AM_TIME_DAY_OF_WEEK%^RESET%^
.EI
.SP 7 5

This is the day of week index into the am_time_array() return value.
.EP

.SI 3
* %^BOLD%^AM_TIME_DECEMBER%^RESET%^
.EI
.SP 7 5

This is the value associated with the month of December in the 
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_EMBER%^RESET%^
.EI
.SP 7 5

This is the value associated with the month of Ember in the 
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_FEBURARY%^RESET%^
.EI
.SP 7 5

This is the value associated with the month of Feburary in the 
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_FORMAT_NO_TIME%^RESET%^
.EI
.SP 7 5

A flag used to be passed into the format parameter for the amtime
function.  It turns off the displaying of the time; ie: 12:48am
disappears.

.EP

.SI 3
* %^BOLD%^AM_TIME_FRIDAY%^RESET%^
.EI
.SP 7 5

This is the value associated with the day Friday in the
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_GRUNE%^RESET%^
.EI
.SP 7 5

This is the value associated with the month of Grune in the 
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_HANDLER%^RESET%^
.EI
.SP 7 5

Thgis is the pointer to the handler for am time functions.
.EP

.SI 3
* %^BOLD%^AM_TIME_HOUR%^RESET%^
.EI
.SP 7 5

This is the hour index into the am_time_array() return value.
.EP

.SI 3
* %^BOLD%^AM_TIME_ICK%^RESET%^
.EI
.SP 7 5

This is the value associated with the month of Ick in the 
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_JUNE%^RESET%^
.EI
.SP 7 5

This is the value associated with the month of June in the 
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_MARCH%^RESET%^
.EI
.SP 7 5

This is the value associated with the month of March in the 
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_MAY%^RESET%^
.EI
.SP 7 5

This is the value associated with the month of May in the 
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_MINUTE%^RESET%^
.EI
.SP 7 5

This is the minute index into the am_time_array() return value.
.EP

.SI 3
* %^BOLD%^AM_TIME_MONDAY%^RESET%^
.EI
.SP 7 5

This is the value associated with the day Monday in the
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_MONTH%^RESET%^
.EI
.SP 7 5

This is the month index into the am_time_array() return value.
.EP

.SI 3
* %^BOLD%^AM_TIME_OCTEDAY%^RESET%^
.EI
.SP 7 5

This is the value associated with the day Octeday in the
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_OFFLE%^RESET%^
.EI
.SP 7 5

This is the value associated with the month of Offle in the 
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_PRIMUS%^RESET%^
.EI
.SP 7 5

This is the primus index into the am_time_array() return value.
A year is primus if it is in the first 400 days, not the second.
.EP

.SI 3
* %^BOLD%^AM_TIME_SATURDAY%^RESET%^
.EI
.SP 7 5

This is the value associated with the day Saturday in the
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_SEKTOBER%^RESET%^
.EI
.SP 7 5

This is the value associated with the month of Sektober in the 
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_SPUNE%^RESET%^
.EI
.SP 7 5

This is the value associated with the month of Spune in the 
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_SUNDAY%^RESET%^
.EI
.SP 7 5

This is the value associated with the day Sunday in the
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_THURSDAY%^RESET%^
.EI
.SP 7 5

This is the value associated with the day Thursday in the
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_TUESDAY%^RESET%^
.EI
.SP 7 5

This is the value associated with the day Tuesday in the
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_WEDNESSDAY%^RESET%^
.EI
.SP 7 5

This is the value associated with the day Wednessday in the
am time calendar.
.EP

.SI 3
* %^BOLD%^AM_TIME_YEAR%^RESET%^
.EI
.SP 7 5

This is the year index into the am_time_array() return value.
.EP

