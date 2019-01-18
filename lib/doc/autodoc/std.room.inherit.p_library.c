.DT
p_library.c
Disk World autodoc help
p_library.c

.SH Description
.SP 5 5

Library inherit. Allows players to take out books on loan. If
set to be player contributable, then books (most likely player 
written, but any kind) can be added/removed by designated 
librarian players.

.EP
.SP 10 5


Written by Aquilo

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h, /include/player.h, /include/am_time.h, /include/language.h, /include/shops/bank.h, /include/playtesters.h, /include/money.h and /include/book_handler.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^_account%^RESET%^
class _account {
int total_fines_paid;
int lost_damaged;
class _loan * loans;
}

.EI
.SP 7 5

Holds info on a player.
.EP

.SI 3
* %^BOLD%^_book%^RESET%^
class _book {
string path;
string auto_load;
string cap_title;
string cap_author;
mapping loaned;
int borrowable;
int copies;
int total_borrowed;
}

.EI
.SP 7 5

This is all the info on one of our stored books.
.EP

.SI 3
* %^BOLD%^_loan%^RESET%^
class _loan {
int id;
int due_back;
}

.EI
.SP 7 5

This holds info on a book a player has borrowed
.EP


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_book_to_library%^RESET%^
.EI
.SI 5
varargs int add_book_to_library(mixed thing, int copies, string cap_title, string cap_author, int borrowable)
.EI
.SP 7 5

This function adds the specified object to the library 
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
thing - Either a filename, or object. If its an object we record its autoload info
.EP
.SP 9 5
copies - how many copies to add
.EP
.SP 9 5
cap_title - The capitalized title its cataloged under
.EP
.SP 9 5
cap_author - The capitalized author of the work
.EP
.SP 9 5
borrowable - 1 if it can be borrowed, 0 if its reference only

.EP

.SI 3
* %^BOLD%^clear_loan%^RESET%^
.EI
.SI 5
void clear_loan(mixed player, int id, int calc_fines)
.EI
.SP 7 5

This clears the loan of library book id the specified player.
If its overdue it sorts out adding a fine to his name.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - Either the player name or object
.EP
.SP 9 5
id - the library book id
.EP
.SP 9 5
calc_fines - if this is 1, then we add fines onto the player
if appropriate, if 0 then we ignore fines and just clear the loan

.EP

.SI 3
* %^BOLD%^do_lost_damaged_fine%^RESET%^
.EI
.SI 5
int do_lost_damaged_fine(int i)
.EI

.SI 3
* %^BOLD%^do_mark%^RESET%^
.EI
.SI 5
int do_mark(int id, int flag)
.EI

.SI 3
* %^BOLD%^do_report%^RESET%^
.EI
.SI 5
int do_report(int id, string arg, string player)
.EI

.SI 3
* %^BOLD%^do_set_fine%^RESET%^
.EI
.SI 5
int do_set_fine(int i)
.EI

.SI 3
* %^BOLD%^do_set_loan_length%^RESET%^
.EI
.SI 5
int do_set_loan_length(int days)
.EI

.SI 3
* %^BOLD%^query_access%^RESET%^
.EI
.SI 5
int query_access(mixed player)
.EI

.SI 3
* %^BOLD%^query_borrow_func%^RESET%^
.EI
.SI 5
function query_borrow_func()
.EI

.SI 3
* %^BOLD%^query_borrowed_by%^RESET%^
.EI
.SI 5
mapping query_borrowed_by(int id)
.EI
.SP 7 5

Returns a mapping of players who borrowed the given book id, mapped to their 
due back dates.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - valid book id

.EP

.SI 3
* %^BOLD%^query_fine%^RESET%^
.EI
.SI 5
int query_fine(string name)
.EI
.SP 7 5

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the string name of the person to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
How much of a fine this person has

.EP

.SI 3
* %^BOLD%^query_fine_per_day%^RESET%^
.EI
.SI 5
int query_fine_per_day()
.EI

.SI 3
* %^BOLD%^query_library_name%^RESET%^
.EI
.SI 5
string query_library_name()
.EI

.SI 3
* %^BOLD%^query_loan_length%^RESET%^
.EI
.SI 5
int query_loan_length()
.EI

.SI 3
* %^BOLD%^query_loans%^RESET%^
.EI
.SI 5
mapping query_loans(string name)
.EI
.SP 7 5

Returns a mapping of book ids to due back dates for the given player.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the player
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a mapping in the form ([ book id : due back ])

.EP

.SI 3
* %^BOLD%^query_lost_damaged_fine%^RESET%^
.EI
.SI 5
int query_lost_damaged_fine()
.EI

.SI 3
* %^BOLD%^query_max_loans%^RESET%^
.EI
.SI 5
int query_max_loans()
.EI

.SI 3
* %^BOLD%^query_player_contributable%^RESET%^
.EI
.SI 5
int query_player_contributable()
.EI

.SI 3
* %^BOLD%^query_save_file%^RESET%^
.EI
.SI 5
string query_save_file()
.EI

.SI 3
* %^BOLD%^set_access%^RESET%^
.EI
.SI 5
void set_access(string player, int access)
.EI

.SI 3
* %^BOLD%^set_borrow_func%^RESET%^
.EI
.SI 5
void set_borrow_func(function f)
.EI

.SI 3
* %^BOLD%^set_fine_per_day%^RESET%^
.EI
.SI 5
void set_fine_per_day(int i)
.EI

.SI 3
* %^BOLD%^set_library_name%^RESET%^
.EI
.SI 5
void set_library_name(string str)
.EI

.SI 3
* %^BOLD%^set_loan_length%^RESET%^
.EI
.SI 5
void set_loan_length(int i)
.EI

.SI 3
* %^BOLD%^set_lost_damaged_fine%^RESET%^
.EI
.SI 5
void set_lost_damaged_fine(int i)
.EI

.SI 3
* %^BOLD%^set_max_loans%^RESET%^
.EI
.SI 5
void set_max_loans(int i)
.EI

.SI 3
* %^BOLD%^set_player_contributable%^RESET%^
.EI
.SI 5
void set_player_contributable(int i)
.EI

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
void set_save_file(string s)
.EI


