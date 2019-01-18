.DT
amtime.c
Disk World autodoc help
amtime.c

.SH Description
.SP 5 5

Cotains some simul_efuns.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/am_time.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^amtime%^RESET%^
.EI
.SI 5
varargs string amtime(int number, int format)
.EI
.SP 7 5

This method returns the amtime for the input time number.  This function takes
the same input as the efun ctime().  You pass in the time number and
it returns a string detailing the time in ankh morpork.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the time number
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string containing the amtime

.EP

.SI 3
* %^BOLD%^ordinal%^RESET%^
.EI
.SI 5
string ordinal(int number)
.EI
.SP 7 5

Adds a "rd", "th", "st" onto a number.  Takes a number as an input and
adds the ordinal type on it.  So you get 3rd and 2nd etc as return
strings.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the number to add the ordinal too
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string being the number plus the ordinal

.EP

.SI 3
* %^BOLD%^query_time_string%^RESET%^
.EI
.SI 5
varargs string query_time_string(int t, int max_elements, int am_time, int use_words)
.EI
.SP 7 5

 This function will take an integer and turn it into a pretty string
 based on the various and sundry options you pass to it.  You pass the
 variable t to the function, which represents the number of seconds
 you want to turn into a string.  max_elements is the first option, and
 it specifies how many elements you would like to return.  It will
 default to two, and -1 will mean "any number of elements".

am_time is a boolean value which tells the function how to interpret the 't' variable. If you pass true (1) then it will interpret the seconds as game seconds, if you pass false (0), then it will interpret them as real-life seconds.

Finally, use_words will determine whether or not the function will use numbers (e.g., 2 years and 24 hours) or words (e.g., two years and twenty-four hours).

@param t how many seconds to turn into a string @param max_elements the maximum number of time elements to return @param am_time whether to interpret t as game time or real time @param use_words whether to return numbers or words @example query_time_string (86410) will return "1 day and 10 seconds" @example query_time_string (86461) will return "1 day and 1 minute" @example query_time_string (86461, -1) will return "1 day, 1 minute, and 1 second" @example query_time_string (86461, -1, 0, 1) will return "one day, one minute, and one second" @author Sousjagne 
.EP


