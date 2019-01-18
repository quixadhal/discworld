.DT
twiki.c
Disk World autodoc help
twiki.c

.SH Description
.SP 5 5

This handler will do any mud base manipulations of the twiki
database.
.EP
.SP 10 5


Written by Pinkfish

Started Wed May 24 17:19:06 PDT 2000

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/board.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^find_changed_pages%^RESET%^
.EI
.SI 5
mapping find_changed_pages(int limit, string database)
.EI
.SP 7 5

This method finds the pages that have changed since the specified time.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
limit - the time after which to find the changes
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the mapping of pages to who changed them

.EP

.SI 3
* %^BOLD%^find_changes%^RESET%^
.EI
.SI 5
string find_changes(int limit)
.EI
.SP 7 5

This method prints out any changes since the specified limit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
limit - the time after which to show any changes
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the changes since that time

.EP

.SI 3
* %^BOLD%^find_changes_in_database%^RESET%^
.EI
.SI 5
string find_changes_in_database(string database, int limit)
.EI
.SP 7 5

This method prints out any changes since the specified limit.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
database - the database to find the changes in
.EP
.SP 9 5
limit - the time after which to show any changes
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the changes since that time

.EP

.SI 3
* %^BOLD%^query_file%^RESET%^
.EI
.SI 5
string query_file(string fname)
.EI
.SP 7 5

This method finds the specified file and shows it to us.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
fname - the file name to find
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the text of the file, 0 if the file is not found

.EP


