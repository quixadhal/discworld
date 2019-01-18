.DT
money.h
Disk World autodoc help
money.h

.SH Description
.SP 5 5

This file contains all the data needed for handling money.  This
includes the money handler and the money object itself.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h.
.EP

.SH Defines
.SI 3
* %^BOLD%^MONEY_ALIAS%^RESET%^
.EI
.SP 7 5

The money alias, this is used by the money object to identify
itself.  The reason it has spaces in it is so that it is not
matched by find_match(), only by present().
.EP

.SI 3
* %^BOLD%^MONEY_DETAILS_COMPOSITION%^RESET%^
.EI
.SP 7 5

The index into the array returned by query_details_for function
for the return array.  This is what the coin is made of. 

.EP

.SI 3
* %^BOLD%^MONEY_DETAILS_HEADS%^RESET%^
.EI
.SP 7 5

The index into the array returned by query_details_for function
for the return array.  This is the word printed when the coin is 
flipped and it hands in its head.

.EP

.SI 3
* %^BOLD%^MONEY_DETAILS_OBVERSE%^RESET%^
.EI
.SP 7 5

The index into the array returned by query_details_for function
for the return array.  This is what is contained on the obverse
of the coin

.EP

.SI 3
* %^BOLD%^MONEY_DETAILS_PLURAL%^RESET%^
.EI
.SP 7 5

The index into the array returned by query_details_for function
for the return array.  This is what the plural for the coin
is.

.EP

.SI 3
* %^BOLD%^MONEY_DETAILS_REVERSE%^RESET%^
.EI
.SP 7 5

The index into the array returned by query_details_for function
for the return array.   This is what is contained on the reverse
of the coin.

.EP

.SI 3
* %^BOLD%^MONEY_DETAILS_TAILS%^RESET%^
.EI
.SP 7 5

The index into the array returned by query_details_for function
for the return array.  This is the word printed when the coin is 
flipped and it hands in its tail.

.EP

.SI 3
* %^BOLD%^MONEY_HAND%^RESET%^
.EI
.SP 7 5

The money handler object, used for routines and opetations on money.

.EP

.SI 3
* %^BOLD%^MONEY_OBJECT%^RESET%^
.EI
.SP 7 5

The object to use for money.

.EP

.SI 3
* %^BOLD%^MONEY_PAY_CHANGE%^RESET%^
.EI
.SP 7 5

The index into the array returned by the make_payment functions for
the change.

.EP

.SI 3
* %^BOLD%^MONEY_PAY_DEPLETED%^RESET%^
.EI
.SP 7 5

The index into the array returned by make_payment functions for the
the depleted money array.

.EP

.SI 3
* %^BOLD%^MONEY_PAY_RETURN%^RESET%^
.EI
.SP 7 5

The index into the array returned by the make_payment functions for
the return array.  This is the actual value to fiddle with.

.EP

.SI 3
* %^BOLD%^MONEY_VAULT%^RESET%^
.EI
.SP 7 5

The money recycling room

.EP

.SI 3
* %^BOLD%^NO_LEGAL%^RESET%^
.EI

.SI 3
* %^BOLD%^NO_MATCH%^RESET%^
.EI

.SI 3
* %^BOLD%^NO_MONEY%^RESET%^
.EI

