.DT
bank_franchise.c
Disk World autodoc help
bank_franchise.c

.SH Description
.SP 5 5

The inherit to use for franchises for the bank.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Jun 15 18:52:48 PDT 2001

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/inherit/bank_base.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h and /include/money.h.
.EP

.SH Classes

.SI 3
* %^BOLD%^credit_note%^RESET%^
class credit_note {
int num;
int amt;
int made;
string maker;
}

.EI


.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_create%^RESET%^
.EI
.SI 5
int do_create()
.EI
.SP 7 5

This method creates a credit note for the store based on wild and
exciting stuff.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^is_allowed_credit%^RESET%^
.EI
.SI 5
int is_allowed_credit(string name)
.EI
.SP 7 5

Are they allowed for credit transactions, listing outstanding credit,
making credit notes, etc.  This should allow the standard owner and the
owner of the main branch.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the player name

.EP

.SI 3
* %^BOLD%^is_valid_note%^RESET%^
.EI
.SI 5
int is_valid_note(int id)
.EI
.SP 7 5

This method checks to find if the note is valid or not.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the not to check
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP

.SI 3
* %^BOLD%^process_note%^RESET%^
.EI
.SI 5
int process_note(int id)
.EI
.SP 7 5

THis is called when a specific credit note is processed
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
id - the id of the note to process
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on a sucessful processing

.EP

.SI 3
* %^BOLD%^query_credit_notes%^RESET%^
.EI
.SI 5
class credit_note * query_credit_notes()
.EI
.SP 7 5

This method returns the array of outstanding credit notes.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the array of outstanding credit notes

.EP

.SI 3
* %^BOLD%^query_credit_num%^RESET%^
.EI
.SI 5
int query_credit_num()
.EI
.SP 7 5

This method returns the current credit number.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current credit number

.EP

.SI 3
* %^BOLD%^query_outstanding_credit_value%^RESET%^
.EI
.SI 5
int query_outstanding_credit_value()
.EI
.SP 7 5

This method returns the amount of money currently in outstanding credit
notes.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount in outstanding credit notes

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^set_credit_notes%^RESET%^
.EI
.SI 5
void set_credit_notes(class credit_note * notes)
.EI
.SP 7 5

This allowed the set of outstanding credit notes to be set.
.EP

.SI 3
* %^BOLD%^set_credit_num%^RESET%^
.EI
.SI 5
void set_credit_num(int num)
.EI
.SP 7 5

This allows the current credit number for credit notes to be set.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the new credit number

.EP


