.DT
multiple_short.c
Disk World autodoc help
multiple_short.c

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
* %^BOLD%^query_multiple_short%^RESET%^
.EI
.SI 5
varargs string query_multiple_short(mixed * args, string type, int no_dollars, int quiet, int dark)
.EI
.SP 7 5

Returns a nice string which looks like "Frog, fish and chicken".  This
takes in an array of objects and or strings and returns a message
like that shown above.  It also does things like adding the number of
each type at the start of the function.


This function has two main uses: to produce the control codes for a set of objects and to produce a nice list from a set of strings. The "type" argument is optional and may be "a", "the", "one" or "poss", defaulting to "a": it determines which of a_short, the_short, one_short or poss_short will be used to make the control code sequence for the objects in "args". If all of the elements of "args" are objects, it simply returns the block control code for the relevant shorts of the objects. Otherwise, it looks through the elements one at a time, replacing objects by the result of calling the relevant short function on them; finally it concatenates the elements with commas between each pair, except between the last pair which have " and " between them. Since the output will contain processing codes if there are any objects in "args", it should be passed through convert_message before being stored. 

If you pass in a 1 for the 'quiet' flag, it will not take any of the objects out of hiding. 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
args - the array to do the processing on
.EP
.SP 9 5
type - the type of short to use "a", "the", "one", "poss"
.EP
.SP 9 5
no_dollars - this forces the output to be a real string
.EP
.SP 9 5
quiet - do not bring the objects out of hiding
.EP
.SP 9 5
dark - how dark it is, used to hide object lists in the dark
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the processed string

.EP


