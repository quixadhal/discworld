.DT
bank_handler.c
Disk World autodoc help
bank_handler.c

.SH Description
.SP 5 5

Keep track of each banks data.
.EP
.SP 10 5

.EP

.SH Classes

.SI 3
* %^BOLD%^bank_data%^RESET%^
class bank_data {
string master_office;
class franchise * franchises;
int cost_to_start;
int default_cost_per_month;
int default_cost_per_new_account;
}

.EI

.SI 3
* %^BOLD%^franchise%^RESET%^
class franchise {
string office;
int cost_per_month;
int cost_per_new_account;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_new_franchise%^RESET%^
.EI
.SI 5
void add_new_franchise(string name, string franchise_office)
.EI
.SP 7 5

This method adds a new franchise to the operation.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the bank set
.EP
.SP 9 5
franchise_office - the new franchise to add

.EP

.SI 3
* %^BOLD%^adjust_account%^RESET%^
.EI
.SI 5
void adjust_account(string name, string bank_name, int amount)
.EI
.SP 7 5

Change the amount of money a player has at a given bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player
.EP
.SP 9 5
bank_name - the name of the bank
.EP
.SP 9 5
amount - the amount to change the balance by

.EP

.SI 3
* %^BOLD%^create_new_bank%^RESET%^
.EI
.SI 5
void create_new_bank(string name, string master)
.EI
.SP 7 5

THis method creates a new bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the bank
.EP
.SP 9 5
master - the location of the master bank

.EP

.SI 3
* %^BOLD%^do_cheat_check%^RESET%^
.EI
.SI 5
void do_cheat_check(string name, string bank_name, int amount, string str, int after)
.EI

.SI 3
* %^BOLD%^get_data_file%^RESET%^
.EI
.SI 5
int get_data_file(string name)
.EI
.SP 7 5

Start up the system by restoreing the players data.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the player being restored

.EP

.SI 3
* %^BOLD%^query_account%^RESET%^
.EI
.SI 5
int query_account(string word, string bank_name)
.EI
.SP 7 5

Find out how much a player has at a given bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
word - the name of the player
.EP
.SP 9 5
bank_name - the name of the bank
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current balance, -1 on failure

.EP

.SI 3
* %^BOLD%^query_accounts%^RESET%^
.EI
.SI 5
string * query_accounts(string player)
.EI
.SP 7 5

Query the accounts a player has.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
player - the name of the player

.EP

.SI 3
* %^BOLD%^query_bank_data%^RESET%^
.EI
.SI 5
class bank_data query_bank_data(string name)
.EI
.SP 7 5

This method is to be used for testing.

.EP

.SI 3
* %^BOLD%^query_bank_default_cost_per_month%^RESET%^
.EI
.SI 5
int query_bank_default_cost_per_month(string name)
.EI
.SP 7 5

This method returns the new cost for opening a franchise for the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bank - the bank to change the cost for
.EP
.SP 9 5
amount - the new amount to charge to open

.EP

.SI 3
* %^BOLD%^query_bank_default_cost_per_new_account%^RESET%^
.EI
.SI 5
int query_bank_default_cost_per_new_account(string name)
.EI
.SP 7 5

This method returns the new cost for opening a franchise for the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bank - the bank to change the cost for
.EP
.SP 9 5
amount - the new amount to charge to open

.EP

.SI 3
* %^BOLD%^query_bank_default_cost_to_open%^RESET%^
.EI
.SI 5
int query_bank_default_cost_to_open(string name)
.EI
.SP 7 5

This method returns the new cost for opening a franchise for the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bank - the bank to change the cost for
.EP
.SP 9 5
amount - the new amount to charge to open

.EP

.SI 3
* %^BOLD%^query_bank_franchise_cost_per_month%^RESET%^
.EI
.SI 5
int query_bank_franchise_cost_per_month(string name, string franchise_office)
.EI
.SP 7 5

This method returns the new cost for opening a franchise for the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bank - the bank to change the cost for
.EP
.SP 9 5
franchise - the franchise to change the amounts for
.EP
.SP 9 5
amount - the new amount to charge to open

.EP

.SI 3
* %^BOLD%^query_bank_franchise_cost_per_new_account%^RESET%^
.EI
.SI 5
int query_bank_franchise_cost_per_new_account(string name, string franchise_office)
.EI
.SP 7 5

This method returns the new cost for opening a franchise for the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bank - the bank to change the cost for
.EP
.SP 9 5
amount - the new amount to charge to open

.EP

.SI 3
* %^BOLD%^query_bank_master_office%^RESET%^
.EI
.SI 5
string query_bank_master_office(string name)
.EI
.SP 7 5

This method returns the master office of the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the bank name to check in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the master office of the bank

.EP

.SI 3
* %^BOLD%^query_banks%^RESET%^
.EI
.SI 5
string * query_banks()
.EI
.SP 7 5

This method returns the current list of banks.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current list of banks

.EP

.SI 3
* %^BOLD%^query_franchises%^RESET%^
.EI
.SI 5
string * query_franchises(string bank)
.EI
.SP 7 5

This method returns the franchises for the specified bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bank - the bank to get the franchises of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of franchises

.EP

.SI 3
* %^BOLD%^remove_bank%^RESET%^
.EI
.SI 5
void remove_bank(string name)
.EI
.SP 7 5

This method removes a bank from the current list ofbanks.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the bank to remove

.EP

.SI 3
* %^BOLD%^set_account%^RESET%^
.EI
.SI 5
void set_account(string name, string bank_name, int amount)
.EI
.SP 7 5

Sets the balance at a certain bank to the specificed amount.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the player
.EP
.SP 9 5
bank_name - the name of the bank
.EP
.SP 9 5
amount - the amount to set the bank to.
Query the accounts a player has.

.EP
.SP 9 5
player - the name of the player

.EP

.SI 3
* %^BOLD%^set_bank_default_cost_per_month%^RESET%^
.EI
.SI 5
void set_bank_default_cost_per_month(string name, int amount)
.EI
.SP 7 5

This method sets the new cost for opening a franchise for the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bank - the bank to change the cost for
.EP
.SP 9 5
amount - the new amount to charge to open

.EP

.SI 3
* %^BOLD%^set_bank_default_cost_per_new_account%^RESET%^
.EI
.SI 5
void set_bank_default_cost_per_new_account(string name, int amount)
.EI
.SP 7 5

This method sets the new cost for opening a franchise for the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bank - the bank to change the cost for
.EP
.SP 9 5
amount - the new amount to charge to open

.EP

.SI 3
* %^BOLD%^set_bank_default_cost_to_open%^RESET%^
.EI
.SI 5
void set_bank_default_cost_to_open(string name, int amount)
.EI
.SP 7 5

This method sets the new cost for opening a franchise for the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bank - the bank to change the cost for
.EP
.SP 9 5
amount - the new amount to charge to open

.EP

.SI 3
* %^BOLD%^set_bank_franchise_cost_per_month%^RESET%^
.EI
.SI 5
void set_bank_franchise_cost_per_month(string name, string franchise_office, int amount)
.EI
.SP 7 5

This method sets the new cost for opening a franchise for the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bank - the bank to change the cost for
.EP
.SP 9 5
franchise - the franchise to change the amounts for
.EP
.SP 9 5
amount - the new amount to charge to open

.EP

.SI 3
* %^BOLD%^set_bank_franchise_cost_per_new_account%^RESET%^
.EI
.SI 5
void set_bank_franchise_cost_per_new_account(string name, int amount)
.EI
.SP 7 5

This method sets the new cost for opening a franchise for the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bank - the bank to change the cost for
.EP
.SP 9 5
amount - the new amount to charge to open

.EP

.SI 3
* %^BOLD%^set_bank_master_office%^RESET%^
.EI
.SI 5
void set_bank_master_office(string name, string master)
.EI
.SP 7 5

This method set the master office of the bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the bank name to check in
.EP
.SP 9 5
master - the master office of the bank

.EP


