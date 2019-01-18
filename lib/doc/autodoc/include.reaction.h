.DT
reaction.h
Disk World autodoc help
reaction.h

.SH Description
.SP 5 5

Include file for reaction handler.
This file includes some useful defines for parameters passed to
function pointers.
To use these, you'd do $OBJ_A instead of $1, etc.
.EP
.SP 10 5


Written by Jeremy

.EP

.SH Classes

.SI 3
* %^BOLD%^reaction%^RESET%^
class reaction {
mixed ratio;
string result;
float result_amt;
string message;
mixed * func;
}

.EI
.SP 7 5

The class used in the reaction handler.

.EP


.SH Defines
.SI 3
* %^BOLD%^AMT_A%^RESET%^
.EI
.SP 7 5

The amount of obj_a that will be consumed.
These last two were necessary because the amounts aren't actually
consumed until after the function pointers are called.  So
$OBJ_A->query_amount() returns the original amount, and $AMT_A
is the amount actually used.

.EP

.SI 3
* %^BOLD%^AMT_B%^RESET%^
.EI
.SP 7 5

The amount of obj_b that will be consumed.
These last two were necessary because the amounts aren't actually
consumed until after the function pointers are called.  So
$OBJ_A->query_amount() returns the original amount, and $AMT_A
is the amount actually used.

.EP

.SI 3
* %^BOLD%^ENV%^RESET%^
.EI
.SP 7 5
The environment in which the reaction is taking place. 
.EP

.SI 3
* %^BOLD%^ENV2%^RESET%^
.EI
.SP 7 5
The environment of the environment. 
.EP

.SI 3
* %^BOLD%^OBJ_A%^RESET%^
.EI
.SP 7 5
The first reagent in the reaction spec. 
.EP

.SI 3
* %^BOLD%^OBJ_B%^RESET%^
.EI
.SP 7 5
The second reagent in the reaction spec. 
.EP

.SI 3
* %^BOLD%^OBJ_X%^RESET%^
.EI
.SP 7 5
The result of the reaction. 
.EP

