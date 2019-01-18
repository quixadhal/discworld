.DT
craft_shop_category_modify.c
Disk World autodoc help
craft_shop_category_modify.c

.SH Description
.SP 5 5

This room will handle craft shop like things, where you sell stuff to
the store and it will sell it onto other players after taking
a margin.  This can be used for potions, pottery, swords, anything!
.EP
.SP 10 5


Written by Pinkfish

Started Thu Feb  5 15:39:57 CST 1998

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/shops/inherit/craft_shop_category.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/shops/bank.h, /include/shops/craft_shop.h and /include/money.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^set_deletion_time%^RESET%^
.EI
.SI 5
void set_deletion_time(int tim)
.EI
.SP 7 5

This method sets the amount of time it takes for a category deletion
to go through the process.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
time - the deletion time

.EP

.SI 3
* %^BOLD%^set_election_time%^RESET%^
.EI
.SI 5
void set_election_time(int tim)
.EI
.SP 7 5

This method sets the amount of time it takes for a category election
to go through the process.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
time - the election time

.EP

.SI 3
* %^BOLD%^set_minimum_age_to_nominate%^RESET%^
.EI
.SI 5
void set_minimum_age_to_nominate(int age)
.EI
.SP 7 5

This method sets the minimum age you need to be to nominate a new
category.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
age - the minimum age to nominate

.EP

.SI 3
* %^BOLD%^set_minimum_age_to_vote%^RESET%^
.EI
.SI 5
void set_minimum_age_to_vote(int age)
.EI
.SP 7 5

This method sets the minimum age you need to be to vote in category
elections.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
age - the minimum age to vote

.EP

.SI 3
* %^BOLD%^set_minimum_nomination_number%^RESET%^
.EI
.SI 5
void set_minimum_nomination_number(int num)
.EI

.SI 3
* %^BOLD%^set_nomination_time%^RESET%^
.EI
.SI 5
void set_nomination_time(int tim)
.EI
.SP 7 5

This method sets the amount of time it takes for a category nomination
to go through the process.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
time - the nomination time

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^category_function%^RESET%^
.EI
.SI 5
int category_function(string name, int state, string * voted, int yes, int no, int abstain)
.EI
.SP 7 5

This is the function used as a call back from the controller to deal with
category based stuff.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the category being processed
.EP
.SP 9 5
state - the state the category is currently in
.EP
.SP 9 5
voted - the number of people who voted
.EP
.SP 9 5
yes - the number who voted yes
.EP
.SP 9 5
no - the number who voted no
.EP
.SP 9 5
abstain - the number who abstained
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
0 on error, timeout of the next stage

.EP


