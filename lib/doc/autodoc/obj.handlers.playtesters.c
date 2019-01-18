.DT
playtesters.c
Disk World autodoc help
playtesters.c

.SH Description
.SP 5 5

Keeps track of all the current playtesters.  This keeps track of all
the current playtesters.

.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/project_management.h, /include/board.h, /include/player_handler.h, /include/login_handler.h, /include/login.h and /include/playtesters.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_jump_point%^RESET%^
.EI
.SI 5
void add_jump_point(string from, string to)
.EI
.SP 7 5

This method adds a jump point to the list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
from - the from location
.EP
.SP 9 5
to - the to location

.EP

.SI 3
* %^BOLD%^add_playtester%^RESET%^
.EI
.SI 5
int add_playtester(string name)
.EI
.SP 7 5

This method adds the playtester to the array of current play
testers.  NOTE: This uses get_age_raw(), which will cause lag if called on
lots of playerfiles.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player to add
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 on failure
.EP

.SI 3
* %^BOLD%^add_pt_exec%^RESET%^
.EI
.SI 5
int add_pt_exec(string name)
.EI
.SP 7 5

This method adds the player as a playtester executive.  The playtester
needs to already be added as a playtester before becoming a
playtester executive.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player to add
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^add_senior_playtester%^RESET%^
.EI
.SI 5
int add_senior_playtester(string name)
.EI
.SP 7 5

This method adds the player as a senior playtester.  The playtester
needs to already be added as a normal playtester before becoming a
senior playtester.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player to add
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^adjust_quota%^RESET%^
.EI
.SI 5
void adjust_quota(string name, int amount)
.EI
.SP 7 5

This adjusts the quota of a playtester.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - The query_name() of the playtester.
.EP
.SP 9 5
amount - A integer to adjust by.

.EP

.SI 3
* %^BOLD%^check_playtester%^RESET%^
.EI
.SI 5
int check_playtester( string)
.EI
.SP 7 5

This method should never ever be used.  Use of this call is meant to be
broken.  Old code that uses check_playtester() probably meant to use
query_playtester().  Use this instead (or better, query_tester()).  If you
wish to hire a playtester using this call (which is what it actually did!),
please look at hire_playtester() and add_playtester().
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
Always returns 0.

.EP

.SI 3
* %^BOLD%^check_playtesters%^RESET%^
.EI
.SI 5
void check_playtesters(string * who)
.EI
.SP 7 5

This goes through the list of playtesters and checks to make
sure they are all still valid.  It does so slowly, using a call_out until
it gets to the end.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
who - A string* of the query_name()'s of the playtesters to check.
.EP

.SI 3
* %^BOLD%^get_age_cached%^RESET%^
.EI
.SI 5
int get_age_cached(string name)
.EI
.SP 7 5

This method will get the age of a playtester from this handlers' cache.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the playtester to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an integer representing the age of the playtester in
seconds.  Always >= 0.

.EP

.SI 3
* %^BOLD%^get_age_uncached%^RESET%^
.EI
.SI 5
int get_age_uncached(string name)
.EI
.SP 7 5

This method will update the playtesters' age if they are online and return
the value from the cache.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the playtester to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an integer representing the age of the playtester in seconds.
Always >= 0.

.EP

.SI 3
* %^BOLD%^get_age_uncached_offline%^RESET%^
.EI
.SI 5
int get_age_uncached_offline(string name)
.EI
.SP 7 5

This method will update the playtesters' age from the player handler.
NOTE: This causes the playerfile to be loaded if it hasn't been already -
this will cause LOTS OF LAG if done on a log of playerfiles.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the playtester to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an integer representing the age of the playtester in seconds.
Always >= 0.

.EP

.SI 3
* %^BOLD%^hire_playtester%^RESET%^
.EI
.SI 5
int hire_playtester(object thing)
.EI
.SP 7 5

This method checks to make sure an object can be a playtester, and if so,
adds them.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - The player object to add as a playtester
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure.

.EP

.SI 3
* %^BOLD%^query_bugs%^RESET%^
.EI
.SI 5
mapping query_bugs(string name)
.EI
.SP 7 5

This will return bug information for a given playtester.  The mapping
returned will map months to arrays of ints.  The format of these arrays is:
({ bugs, online_that_month, age_at_end_of_month }).
.EP

.SI 3
* %^BOLD%^query_exec_access%^RESET%^
.EI
.SI 5
int query_exec_access(string name)
.EI

.SI 3
* %^BOLD%^query_guilds%^RESET%^
.EI
.SI 5
mapping query_guilds()
.EI
.SP 7 5

This method returns a mapping which contains which the playtesters mapped
to their guild objects.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a lovely mapping

.EP

.SI 3
* %^BOLD%^query_jump_destination%^RESET%^
.EI
.SI 5
string * query_jump_destination(string from)
.EI
.SP 7 5

This method finds the destination from the start location.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
from - the location to find the jump point too

.EP

.SI 3
* %^BOLD%^query_jump_points%^RESET%^
.EI
.SI 5
mapping query_jump_points()
.EI
.SP 7 5

This method returns the current mapping of jump points.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the jump points

.EP

.SI 3
* %^BOLD%^query_leave%^RESET%^
.EI
.SI 5
int query_leave(string name)
.EI
.SP 7 5

This tests whether a playtester is "on leave".

.EP

.SI 3
* %^BOLD%^query_months%^RESET%^
.EI
.SI 5
string * query_months()
.EI
.SP 7 5

This returns a sorted list of months, earliest month first.

.EP

.SI 3
* %^BOLD%^query_playtester%^RESET%^
.EI
.SI 5
int query_playtester(string name)
.EI
.SP 7 5

This method tests to see if the given name is a playtester.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are a playtester, 0 if they are not.
.EP

.SI 3
* %^BOLD%^query_playtester_guilds%^RESET%^
.EI
.SI 5
mapping query_playtester_guilds()
.EI
.SP 7 5

This method returns a mapping which contains which Guilds
the playtesters belong to.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a lovely mapping

.EP

.SI 3
* %^BOLD%^query_playtesters%^RESET%^
.EI
.SI 5
string * query_playtesters()
.EI
.SP 7 5

This method returns a list of the current play testers.  This is
a list of string names in an array.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of current playtesters

.EP

.SI 3
* %^BOLD%^query_pt_data%^RESET%^
.EI
.SI 5
mixed query_pt_data(string name)
.EI
.SP 7 5

Returns the raw playtester data for processing!  DEPRECATED AND WILL NOT
RETURN UP TO DATE INFORMATION!
.EP

.SI 3
* %^BOLD%^query_pt_exec%^RESET%^
.EI
.SI 5
int query_pt_exec(string name)
.EI

.SI 3
* %^BOLD%^query_senior_playtester%^RESET%^
.EI
.SI 5
int query_senior_playtester(string name)
.EI
.SP 7 5

This method determines if they are a senior playtester.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the playtester to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are a senior playteser, 0 if they are not
.EP

.SI 3
* %^BOLD%^query_show_list%^RESET%^
.EI
.SI 5
string query_show_list()
.EI
.SP 7 5

This is a nice way of showing a list of the current play testers.
It prints a table of the previous four and current months, with columns
of bug reports made and number of hours online spent.

.EP

.SI 3
* %^BOLD%^query_show_list_colour%^RESET%^
.EI
.SI 5
string query_show_list_colour()
.EI
.SP 7 5

This is a nice way of showing a list of the current play testers.
It prints a table of the previous four and current months, with columns
of bug reports made and number of hours online spent.

.EP

.SI 3
* %^BOLD%^query_tester%^RESET%^
.EI
.SI 5
int query_tester(mixed person)
.EI
.SP 7 5

This method tests to see if "person" is an authorized tester,
i.e. a playtester, creator or has the "test character" property.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - A string or object to determine tester status for.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are a tester, 0 if they are not.
.EP

.SI 3
* %^BOLD%^reason_invalid_playtester%^RESET%^
.EI
.SI 5
string reason_invalid_playtester(string name)
.EI
.SP 7 5

THis method returns a string reason why they are an invalid play
tester.  NOTE: This uses the player_handler, which will cause lag if
called on lots of playerfiles.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the player to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the string reason, 0 if none
.EP

.SI 3
* %^BOLD%^remove_jump_point%^RESET%^
.EI
.SI 5
void remove_jump_point(string from, string to)
.EI
.SP 7 5

This method removes a specific jump point from the list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
from - the from location to remove
.EP
.SP 9 5
to - the to location to remove

.EP

.SI 3
* %^BOLD%^remove_jump_point_from%^RESET%^
.EI
.SI 5
void remove_jump_point_from(string from)
.EI
.SP 7 5

This method removes a specific jump point from the list.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
from - the specific jump point to remove

.EP

.SI 3
* %^BOLD%^remove_playtester%^RESET%^
.EI
.SI 5
int remove_playtester(string name, string reason)
.EI
.SP 7 5

This method will remove the playtester from the current list of
play testers.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player to remove
.EP
.SP 9 5
reason - the reason for the removal
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^remove_pt_exec%^RESET%^
.EI
.SI 5
int remove_pt_exec(string name)
.EI
.SP 7 5

This method removes the player as a playtester executive.  
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player to add
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^remove_senior_playtester%^RESET%^
.EI
.SI 5
int remove_senior_playtester(string name, string reason)
.EI
.SP 7 5

This method adds the player as a senior playtester.  The playtester
needs to already be added to become a senior playtester.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player to remove
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure
.EP

.SI 3
* %^BOLD%^report_made%^RESET%^
.EI
.SI 5
void report_made(string name, string type, string file, string text)
.EI
.SP 7 5

This is called when a playtester makes a bug report.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the playtester
.EP
.SP 9 5
type - the type of report
.EP
.SP 9 5
file - the file the report is on
.EP
.SP 9 5
text - the text associated with the report

.EP

.SI 3
* %^BOLD%^reset_leave%^RESET%^
.EI
.SI 5
int reset_leave(string name)
.EI
.SP 7 5

This marks a playtester as no longer "on leave", and logs it.

.EP

.SI 3
* %^BOLD%^set_leave%^RESET%^
.EI
.SI 5
int set_leave(string name, string reason)
.EI
.SP 7 5

This marks a playtester as "on leave", logs it, with a reason.

.EP

.SI 3
* %^BOLD%^valid_playtester%^RESET%^
.EI
.SI 5
int valid_playtester(string name)
.EI
.SP 7 5

This method determins if they are a valid playtester or not.  NOTE: This
uses reason_invalid_playtester(), which will cause lag if called on lots of
playerfiles.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player to test
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they are, 0 if they are not
.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^check_playtesters_one%^RESET%^
.EI
.SI 5
void check_playtesters_one(string name)
.EI
.SP 7 5

This method determines if a playtester is valid or not, and if not will
automagically remove them (does not remove senior playtesters
automatically).  NOTE: This uses valid_playtester() and
reason_invalid_playtester(), which will cause lag if called on lots of
playerfiles.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player to test
.EP

.SI 3
* %^BOLD%^get_age_raw%^RESET%^
.EI
.SI 5
int get_age_raw(string name)
.EI
.SP 7 5

This method finds the age of the playtester from the player handler.  NOTE:
This causes the playerfile to be loaded if it hasn't been already - this
will cause LOTS OF LAG if done on a lot of playerfiles.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the playtester to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an integer representing the age of the playtester in seconds.
Always >= 0.

.EP


