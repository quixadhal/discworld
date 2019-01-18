.DT
council_treasury.c
Disk World autodoc help
council_treasury.c

.SH Description
.SP 5 5

This is the council treasury inheritable.  It does fun stuff!  Kudos to
Pinkfish for the deposit/withdrawl stuff, which I shamelessy stole 
from /std/bank.
.EP
.SP 10 5


Written by Kili

Started Late October 2001

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room and /std/room/inherit/council_base.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/nomic_system.h, /include/move_failures.h, /include/shops/bank.h and /include/money.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^transaction%^RESET%^
class transaction {
string person;
int amount;
string reason;
int date;
string action;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_time%^RESET%^
.EI
.SI 5
void check_time()
.EI
.SP 7 5

This will check the time of the last transactions, and move them into a
log file if they are over two weeks old.

.EP

.SI 3
* %^BOLD%^load_me%^RESET%^
.EI
.SI 5
void load_me()
.EI
.SP 7 5

Load the variables for this treasury.

.EP

.SI 3
* %^BOLD%^save_me%^RESET%^
.EI
.SI 5
void save_me()
.EI
.SP 7 5

Save the variables for this treasury.

.EP

.SI 3
* %^BOLD%^security_check%^RESET%^
.EI
.SI 5
int security_check(object player, int type)
.EI
.SP 7 5

This will check to see if the player attempting an action is allowed to
perform that action.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - The player attempting the action
.EP
.SP 9 5
type - The type of action (deposting, reading logs, etc.).

.EP

.SI 3
* %^BOLD%^set_archive%^RESET%^
.EI
.SI 5
void set_archive(string word)
.EI
.SP 7 5

This sets the file to which old logs will be transferred after two weeks.  
You can then modify the council archives to read the file.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The file to write to

.EP

.SI 3
* %^BOLD%^set_place%^RESET%^
.EI
.SI 5
void set_place(string word)
.EI
.SP 7 5

Set the location of the treasury, to determine what type of currency to use
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - The location

.EP

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
void set_save_file(string file)
.EI
.SP 7 5

Set the save file for the treasury
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
file - The file to save to

.EP

.SI 3
* %^BOLD%^set_security_levels%^RESET%^
.EI
.SI 5
void set_security_levels(int read_log, int deposits, int * logging)
.EI
.SP 7 5

This sets what you must be to read the log and deposit cash, and also
wether you are logging withdrawls and deposits.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
read_log - 2 for magistrate, 1 for citizen, 0 for anyone is able to
read the log
.EP
.SP 9 5
deposits - Same as for read_log, to see who can deposit
.EP
.SP 9 5
logging - ({withdrwals, deposits}), are we logging? 0 for no, 1 for yes

.EP


