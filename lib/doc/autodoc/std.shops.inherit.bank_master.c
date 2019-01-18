.DT
bank_master.c
Disk World autodoc help
bank_master.c

.SH Description
.SP 5 5

This is the bank's main branch inherit.  Allows people to trade in
credit notes for the money, or deposit credit notes.
.EP
.SP 10 5


Written by Pinkfish

Started Fri Jun 15 01:05:55 PDT 2001

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/inherit/bank_base.
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
int do_process(object * obs)
.EI
.SP 7 5

Processes the specified credit objects to figure out if they need to deposit
or withdraw money.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
obs - the objects to process
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP


