.DT
query_number_and_ctime_elapsed.c
Disk World autodoc help
query_number_and_ctime_elapsed.c

.SH Description
.SP 5 5

Cotains some simul_efuns.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^ctime_elapsed%^RESET%^
.EI
.SI 5
varargs string ctime_elapsed(int time_elapsed, int mode)
.EI

.SI 3
* %^BOLD%^number_as_string%^RESET%^
.EI
.SI 5
string number_as_string(int n)
.EI
.SP 7 5

This method turns a number less than 100 into a string.  This function should
not be used.  It will only work for numbers less than 100.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
n - the number to turn into a string
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number as a nice string
.EP

.SI 3
* %^BOLD%^query_num%^RESET%^
.EI
.SI 5
string query_num(int n, int limit)
.EI
.SP 7 5

This method turns a number into a string.  It returns a string of the
form "one hundred and twenty four".  This function should be used and
not number_as_string below.
The limit argument is used to set the cut off point at which it will
print "many".

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the number to return as a string
.EP
.SP 9 5
limit - the largest number to process
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the number as a nice string

.EP

.SI 3
* %^BOLD%^query_times%^RESET%^
.EI
.SI 5
string query_times(int num)
.EI
.SP 7 5

Convert a number into a number of times. eg. 1 = once, 2 = twice, 3 = three
times etc. etc.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
the - number in question
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a string equivalent.

.EP

.SI 3
* %^BOLD%^word_ordinal%^RESET%^
.EI
.SI 5
string word_ordinal(int num)
.EI
.SP 7 5

This method returns the ordinal of a number as a string (ie, "first",
"thirtieth", etc.)
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
n - the number for which you want the ordinal
.EP


