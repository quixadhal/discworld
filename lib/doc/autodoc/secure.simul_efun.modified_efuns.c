.DT
modified_efuns.c
Disk World autodoc help
modified_efuns.c

.SH Description
.SP 5 5

Cotains some simul_efuns.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /secure/simul_efun/base_name.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/login_handler.h, /include/login.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^call_out%^RESET%^
.EI
.SI 5
varargs int call_out(mixed fun, int delay, mixed * args ...)
.EI

.SI 3
* %^BOLD%^cap_words%^RESET%^
.EI
.SI 5
string cap_words(string words)
.EI
.SP 7 5

This method returns a string with each word capitalized.



.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the string to capitalize
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string with all words capitalized

.EP

.SI 3
* %^BOLD%^cat%^RESET%^
.EI
.SI 5
void cat(string file, int start_line, int number)
.EI
.SP 7 5

Write the file out to the screen.  This should  not be used if possible.
It is an interface to read_file and other things.  You should perhaps
look at using more_string ort more_file.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
file - the file name to cat
.EP
.SP 9 5
start_line - the line to start on
.EP
.SP 9 5
number - the number of lines to print

.EP

.SI 3
* %^BOLD%^db_connect%^RESET%^
.EI
.SI 5
varargs int db_connect(mixed * args ...)
.EI

.SI 3
* %^BOLD%^debug_info%^RESET%^
.EI
.SI 5
string debug_info(int operation, object ob)
.EI
.SP 7 5

Debug info Simul Efun
.EP

.SI 3
* %^BOLD%^flush_log_files%^RESET%^
.EI
.SI 5
void flush_log_files()
.EI
.SP 7 5

This method flushes out all the buffered stuff for the log files.

.EP

.SI 3
* %^BOLD%^input_to%^RESET%^
.EI
.SI 5
void input_to(mixed fun, mixed flag, mixed args ...)
.EI

.SI 3
* %^BOLD%^log_file%^RESET%^
.EI
.SI 5
varargs void log_file(string name, string fmt, mixed * args ...)
.EI
.SP 7 5

This method writes a message out ot a log file.  The log files are
normally in "/log".  If a file doesn't start with '/ then "/log/" will be
preppended to it.
This does automatic removal of the log files after they get over
a certain length.  The fmt and args parameters are used to print the
actual message.


If more than one argument is passed to this function, then sprintf will be used to print out the results. 

ie: log_file("ROGER", "%s: frog and %s\n", ctime(time()), query_gumboot()); 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the log file
.EP
.SP 9 5
fmt - the format string
.EP
.SP 9 5
args - the arguments to the sprintf

.EP

.SI 3
* %^BOLD%^md5%^RESET%^
.EI
.SI 5
string md5(string data)
.EI

.SI 3
* %^BOLD%^memory_summary%^RESET%^
.EI
.SI 5
string memory_summary()
.EI

.SI 3
* %^BOLD%^mud_name%^RESET%^
.EI
.SI 5
string mud_name()
.EI
.SP 7 5

This method returns the name of the mud.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the mud

.EP

.SI 3
* %^BOLD%^query_loggers%^RESET%^
.EI
.SI 5
mapping query_loggers()
.EI

.SI 3
* %^BOLD%^query_verb%^RESET%^
.EI
.SI 5
string query_verb()
.EI
.SP 7 5

This returns the current verb name.  This works if the verb ius used
through add_command or through add_action.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the verb

.EP

.SI 3
* %^BOLD%^reference_allowed%^RESET%^
.EI
.SI 5
varargs int reference_allowed(object referree, mixed referrer)
.EI
.SP 7 5

This method is used to test to see if a reference is allowed to this
object.  It tests things like the allow array and that sort of stuff.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - the object to test
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the reference is allowed

.EP

.SI 3
* %^BOLD%^replace%^RESET%^
.EI
.SI 5
varargs string replace(string str, mixed bing, string rep)
.EI
.SP 7 5

Replaces all occurances of a set of strings in the input string.  Replaces
an individual or an array of strings with new values.  If the second
argument is an array of strings, then the 1st, 3rd, 5th...  elements will
be the strings to search for and the 2nd, 4th, 6th etc will be the strings
to replace with.  If three arguments are specified then the second is the string
to search for, the third the one to replace.


Eg:
.EP
.SP 7 5
str = replace(str, ({ "search", "replace", "orange", "apple" });

That will replace all occurances of "search" with "replace" and "orange" with "apple". 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the string to do the replacement in
.EP
.SP 9 5
bing - the search or array argument
.EP
.SP 9 5
rep - the replacement string, or null

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string with all the replacements done

.EP

.SI 3
* %^BOLD%^sha256%^RESET%^
.EI
.SI 5
string sha256(string data)
.EI

.SI 3
* %^BOLD%^shout%^RESET%^
.EI
.SI 5
varargs void shout(string words, object avoid)
.EI
.SP 7 5

This method sends a polite shout to everyone on line.  It checks
for earmuffs and all that sort of stuff.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
words - the message to shout
.EP
.SP 9 5
avoid - who not to tell the message to

.EP

.SI 3
* %^BOLD%^strcasecmp%^RESET%^
.EI
.SI 5
int strcasecmp(string str1, string str2)
.EI
.SP 7 5

Does a string compare...  But case insensative...

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str1 - the string to compare
.EP
.SP 9 5
str2 - the other string to compare
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 if they are the same, <0 if they are less than each other, >0 if not
.EP

.SI 3
* %^BOLD%^tail%^RESET%^
.EI
.SI 5
varargs string tail(string fname, int nlines)
.EI
.SP 7 5

This will print the last bit of a file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the file to read
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it succeeded, 0 if it failed

.EP

.SI 3
* %^BOLD%^uniq_array%^RESET%^
.EI
.SI 5
mixed * uniq_array(mixed * arr)
.EI
.SP 7 5

This method will return an array that contains no duplicates.
 uniq_array.c
 Written by: Wodan


This function will return an array that contains no duplicates. Gotta love them mappings. :)
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
arr - the array to convert
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array with no duplicates

.EP

.SI 3
* %^BOLD%^user_event%^RESET%^
.EI
.SI 5
void user_event(mixed from, mixed first, mixed args ...)
.EI
.SP 7 5

This method calls an event on all the users online.
It does not do any filtering, so it will send messages to people who
are earmuffed and ignoring or lord invisible or whatever.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
from - the person it is from
.EP
.SP 9 5
name - the name of the event
.EP
.SP 9 5
args - the arguments to the event

.EP

.SI 3
* %^BOLD%^wizardp%^RESET%^
.EI
.SI 5
int wizardp(mixed arg)
.EI
.SP 7 5

This method tells us if the object is a wizard.

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
is the object a wizard

.EP

.SI 3
* %^BOLD%^write%^RESET%^
.EI
.SI 5
void write(mixed str)
.EI
.SP 7 5

This method prints a message to the player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the message to send

.EP


