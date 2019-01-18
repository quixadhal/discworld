.DT
bank_credit_note.c
Disk World autodoc help
bank_credit_note.c

.SH Description
.SP 5 5

A credit note for transfering money to and from a franchise and a
master shop.
.EP
.SP 10 5


Written by Pinkfish

Started Thu Jun 28 16:46:26 PDT 2001

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/object.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_process%^RESET%^
.EI
.SI 5
int do_process()
.EI
.SP 7 5

This marks this credit note as processed.

.EP

.SI 3
* %^BOLD%^is_valid_note%^RESET%^
.EI
.SI 5
int is_valid_note()
.EI
.SP 7 5

This checks to see if the note is valid still or not.

.EP

.SI 3
* %^BOLD%^query_amount%^RESET%^
.EI
.SI 5
int query_amount()
.EI
.SP 7 5

This method returns the amount the credit note is for.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the amount the note is for

.EP

.SI 3
* %^BOLD%^query_bank_name%^RESET%^
.EI
.SI 5
string query_bank_name()
.EI
.SP 7 5

This method returns the bank name for the credit note.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the bank name

.EP

.SI 3
* %^BOLD%^query_credit_num%^RESET%^
.EI
.SI 5
int query_credit_num()
.EI
.SP 7 5

This method returns the credit number id for the credit not.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the credit id

.EP

.SI 3
* %^BOLD%^query_franchise%^RESET%^
.EI
.SI 5
string query_franchise()
.EI
.SP 7 5

This method returns the franchise for the credit note
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
this returns the franchise for the note

.EP

.SI 3
* %^BOLD%^set_amount%^RESET%^
.EI
.SI 5
void set_amount(int amount)
.EI
.SP 7 5

This method sets the amount the credit note is for.  +ve means that
we are transporting money to the master bank, -ve means we are taking
money out of the master bank.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
amount - the amount the note is for

.EP

.SI 3
* %^BOLD%^set_bank_name%^RESET%^
.EI
.SI 5
void set_bank_name(string bank)
.EI
.SP 7 5

This method sets the bank name of the credit note.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
bank - set the bank name

.EP

.SI 3
* %^BOLD%^set_credit_num%^RESET%^
.EI
.SI 5
void set_credit_num(int num)
.EI
.SP 7 5

This method sets the credit number id for the credit note.  This will
be unique for each franchise.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
num - the credit id

.EP

.SI 3
* %^BOLD%^set_franchise%^RESET%^
.EI
.SI 5
void set_franchise(string franchise)
.EI
.SP 7 5

This method sets the franchise that will be used as the main base for
this credit note.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
franchise - the franchise as the base

.EP

.SI 3
* %^BOLD%^setup_read_messages%^RESET%^
.EI
.SI 5
void setup_read_messages()
.EI
.SP 7 5

This method is called to setup the read messages for the note once it
is made.

.EP


