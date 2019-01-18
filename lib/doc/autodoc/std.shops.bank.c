.DT
bank.c
Disk World autodoc help
bank.c

.SH Description
.SP 5 5

This is the bank inheritable.  It handles all the bank transactions
and other such exciting stuff.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/room/basic_room.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/move_failures.h, /include/shops/bank.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^check_open%^RESET%^
.EI
.SI 5
int check_open(string word)
.EI

.SI 3
* %^BOLD%^do_balance%^RESET%^
.EI
.SI 5
int do_balance()
.EI

.SI 3
* %^BOLD%^do_close%^RESET%^
.EI
.SI 5
int do_close()
.EI

.SI 3
* %^BOLD%^do_deposit%^RESET%^
.EI
.SI 5
int do_deposit(string words)
.EI

.SI 3
* %^BOLD%^do_open%^RESET%^
.EI
.SI 5
int do_open()
.EI

.SI 3
* %^BOLD%^do_save%^RESET%^
.EI
.SI 5
void do_save()
.EI

.SI 3
* %^BOLD%^do_withdraw%^RESET%^
.EI
.SI 5
int do_withdraw(int number, string type)
.EI

.SI 3
* %^BOLD%^get_account%^RESET%^
.EI
.SI 5
int get_account()
.EI

.SI 3
* %^BOLD%^make_bank_robbers%^RESET%^
.EI
.SI 5
int make_bank_robbers()
.EI

.SI 3
* %^BOLD%^query_account_cost%^RESET%^
.EI
.SI 5
int query_account_cost()
.EI
.SP 7 5

This method returns the amount of money it costs to start an account
at this bank.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the account cost

.EP

.SI 3
* %^BOLD%^query_bank_name%^RESET%^
.EI
.SI 5
string query_bank_name()
.EI

.SI 3
* %^BOLD%^query_money_place%^RESET%^
.EI
.SI 5
string query_money_place()
.EI

.SI 3
* %^BOLD%^query_percentage%^RESET%^
.EI
.SI 5
int query_percentage()
.EI
.SP 7 5

This method queries the percentage the bank charges on transactions.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the percentage the bank charges

.EP

.SI 3
* %^BOLD%^query_place%^RESET%^
.EI
.SI 5
string query_place()
.EI

.SI 3
* %^BOLD%^query_save_file%^RESET%^
.EI
.SI 5
string query_save_file()
.EI

.SI 3
* %^BOLD%^query_total_account%^RESET%^
.EI
.SI 5
int query_total_account()
.EI
.SP 7 5

This method returns the total value of all the accounts.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the totaly value of all the accounts

.EP

.SI 3
* %^BOLD%^query_total_made%^RESET%^
.EI
.SI 5
int query_total_made()
.EI
.SP 7 5

This method returns the amount of money the bank has made off the
players.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
total_made - the amount of money mae

.EP

.SI 3
* %^BOLD%^set_account%^RESET%^
.EI
.SI 5
int set_account(int amount)
.EI

.SI 3
* %^BOLD%^set_account_cost%^RESET%^
.EI
.SI 5
void set_account_cost(int number)
.EI
.SP 7 5

This method sets the amount of money that it costs to start an account
at this bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the cost of starting an account

.EP

.SI 3
* %^BOLD%^set_bank_name%^RESET%^
.EI
.SI 5
void set_bank_name(string word)
.EI

.SI 3
* %^BOLD%^set_percentage%^RESET%^
.EI
.SI 5
void set_percentage(int number)
.EI
.SP 7 5

This method sets the percentage the bank charges on transactions.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
number - the percentage to charge

.EP

.SI 3
* %^BOLD%^set_place%^RESET%^
.EI
.SI 5
void set_place(string word)
.EI

.SI 3
* %^BOLD%^set_save_file%^RESET%^
.EI
.SI 5
void set_save_file(string word)
.EI

.SI 3
* %^BOLD%^stats%^RESET%^
.EI
.SI 5
mixed * stats()
.EI


