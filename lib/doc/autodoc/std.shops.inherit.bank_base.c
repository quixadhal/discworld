.DT
bank_base.c
Disk World autodoc help
bank_base.c

.SH Description
.SP 5 5

This is the bank inheritable.  It handles all the bank transactions
and other such exciting stuff.  This is a basic bank bit, does not control
any extra tricky bits of bank work.
.EP
.SP 10 5


Written by Pinkfish

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
* %^BOLD%^adjust_account%^RESET%^
.EI
.SI 5
void adjust_account(string person, int amount)
.EI
.SP 7 5

This method is changed the amouint in the specified account.  This is
just an informational thingy.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to update
.EP
.SP 9 5
amount - the amount to update

.EP

.SI 3
* %^BOLD%^adjust_total_difference%^RESET%^
.EI
.SI 5
void adjust_total_difference(int amount)
.EI
.SP 7 5

This method changes the total difference by the specified amount.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
amount - the amount to change it by

.EP

.SI 3
* %^BOLD%^can_adjust_account%^RESET%^
.EI
.SI 5
int can_adjust_account(string person, int amount)
.EI
.SP 7 5

THis should be overridden in higher inherits to control account
adjustments.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person whose count is being adjusted
.EP
.SP 9 5
amount - the amount it is adjusted by
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if they can adjust it, 0 if not

.EP

.SI 3
* %^BOLD%^find_main_bank%^RESET%^
.EI
.SI 5
object find_main_bank()
.EI
.SP 7 5

This method finds the main bank and returns the object of it.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the main bank

.EP

.SI 3
* %^BOLD%^get_account%^RESET%^
.EI
.SI 5
int get_account(string player)
.EI
.SP 7 5

This method gets the amount of money the person has in their
account.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the person to get the account for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the account value

.EP

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
* %^BOLD%^query_bank%^RESET%^
.EI
.SI 5
int query_bank()
.EI
.SP 7 5

This method returns true for all banks.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 for a bank

.EP

.SI 3
* %^BOLD%^query_bank_name%^RESET%^
.EI
.SI 5
string query_bank_name()
.EI
.SP 7 5

This is the name of the bank, used in creating bank branches.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the name of the bank

.EP

.SI 3
* %^BOLD%^query_franchise%^RESET%^
.EI
.SI 5
string query_franchise()
.EI
.SP 7 5

This method returns the franchise associated with this bank.  For master
banks this will always return 0.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the franchise

.EP

.SI 3
* %^BOLD%^query_main_branch_owner%^RESET%^
.EI
.SI 5
string query_main_branch_owner()
.EI
.SP 7 5

This method returns the owner of the main bank branch.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the owner name

.EP

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
* %^BOLD%^query_total_difference%^RESET%^
.EI
.SI 5
int query_total_difference()
.EI
.SP 7 5

This method returns the difference between the deposits and withdrawls.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the difference between deposits and withdrawls

.EP

.SI 3
* %^BOLD%^set_account%^RESET%^
.EI
.SI 5
void set_account(string player, int amount)
.EI
.SP 7 5

This method set the value of the account for the specified person
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
person - the person to set the account for
.EP
.SP 9 5
amount - the amount to set the account for

.EP

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
.SP 7 5

This method sets the name of the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the bank

.EP

.SI 3
* %^BOLD%^set_franchise%^RESET%^
.EI
.SI 5
void set_franchise(string franchise)
.EI
.SP 7 5

This method sets the franchise associated with this bank.  For master
banks this will always return 0.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
franchise - the franchise address

.EP

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
* %^BOLD%^set_total_difference%^RESET%^
.EI
.SI 5
void set_total_difference(int amount)
.EI
.SP 7 5

This method changes the total difference by the specified amount.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
amount - the amount to change it by

.EP


