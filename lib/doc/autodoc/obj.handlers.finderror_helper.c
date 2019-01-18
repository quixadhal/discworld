.DT
finderror_helper.c
Disk World autodoc help
finderror_helper.c

.SH Description
.SP 5 5

This handler contains the methods to handle the creators list of
directories to control bugs in.

.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/db.h, /include/config.h and /include/log.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^error_data%^RESET%^
class error_data {
string name;
string * dirs;
int no_calls;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_directory%^RESET%^
.EI
.SI 5
int add_directory(object player, string dir, int recursive)
.EI
.SP 7 5

This method adds the directory to the creators directory list to check.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the creator to add the directory to
.EP
.SP 9 5
dir - the directory to add
.EP
.SP 9 5
recursive - to add the directory recursively

.EP

.SI 3
* %^BOLD%^escape%^RESET%^
.EI
.SI 5
string escape(string txt)
.EI

.SI 3
* %^BOLD%^finished_query%^RESET%^
.EI
.SI 5
void finished_query(int status, mixed data, function func, object player)
.EI

.SI 3
* %^BOLD%^query_directories%^RESET%^
.EI
.SI 5
string * query_directories(string player)
.EI

.SI 3
* %^BOLD%^query_dirs_count%^RESET%^
.EI
.SI 5
int query_dirs_count(mixed player, function finish_func)
.EI
.SP 7 5

This method returns the error counts for all the directories in the
list of directories to watch.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the creator to get the dir count for

.EP

.SI 3
* %^BOLD%^query_errors_in_dirs%^RESET%^
.EI
.SI 5
int query_errors_in_dirs(string * directories, function finish_func)
.EI
.SP 7 5

This method returns the error counts for all the directories in the
list of directories to watch.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the creator to get the dir count for

.EP

.SI 3
* %^BOLD%^query_next_dir%^RESET%^
.EI
.SI 5
void query_next_dir(object player, function finish_func)
.EI

.SI 3
* %^BOLD%^remove_directory%^RESET%^
.EI
.SI 5
int remove_directory(object player, string directory, int recursive)
.EI


