.DT
random_num.c
Disk World autodoc help
random_num.c

.SH Description
.SP 5 5

Generate reproducible sequences of random numbers.
This is designed to produce a random number from the same
seed.  This will make sequences of reproduceable random
numbers.

Useful for things like garbling of text and stuff so the garble
always looks the same...

.EP
.SP 10 5

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^query_seed%^RESET%^
.EI
.SI 5
int query_seed()
.EI
.SP 7 5

Returns the currently specified seed.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the current seed

.EP

.SI 3
* %^BOLD%^random%^RESET%^
.EI
.SI 5
varargs int random(int max, mixed lseed)
.EI
.SP 7 5

Generate a random number.  If lseed is an int, it is used as the seed.
If lseed is a one-element array of ints, lseed[0] is used as the seed
and the new seed is passed back.  Otherwise, the previously set seed
is used.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
max - the maximum value for the number.
.EP
.SP 9 5
lseed - the seed to use, either an int or a one-element array
of int.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
a number from 0..max-1 (inclusive)

.EP

.SI 3
* %^BOLD%^set_seed%^RESET%^
.EI
.SI 5
void set_seed(int new_seed)
.EI
.SP 7 5

Set the seed for the generator.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
new_seed - the seed to use.

.EP


