.DT
expression_util.c
Disk World autodoc help
expression_util.c

.SH Description
.SP 5 5

This sets the thingy type.  It is what is put in front of the
control functions so many objects can set up exciting and confusing
controls of expressions.

.EP
.SP 10 5

.EP

.SH Inherits
.SP 5 5
This class inherits the following classes /std/basic/expressions.
.EP

.SH Includes
.SP 5 5
This class includes the following files /include/nroff.h, /include/expressions.h and /include/shops/craft_shop.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_approve_status_functions%^RESET%^
.EI
.SI 5
int do_approve_status_functions(int hints)
.EI
.SP 7 5

This method shows the status of all the user defined functions.
These can be used in other exciting functions.

.EP

.SI 3
* %^BOLD%^do_approve_status_internal_functions%^RESET%^
.EI
.SI 5
int do_approve_status_internal_functions()
.EI
.SP 7 5

This method shows the status of all the user defined functions.
These can be used in other exciting functions.

.EP

.SI 3
* %^BOLD%^do_function_help%^RESET%^
.EI
.SI 5
int do_function_help(string name)
.EI
.SP 7 5

Get help on a specific function.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the function to get help on

.EP

.SI 3
* %^BOLD%^do_function_help_list%^RESET%^
.EI
.SI 5
int do_function_help_list()
.EI
.SP 7 5

Get help on a specific function.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the name of the function to get help on

.EP

.SI 3
* %^BOLD%^do_user_function_add%^RESET%^
.EI
.SI 5
int do_user_function_add(string def, string expr)
.EI
.SP 7 5

This method allows a function to be added to the base set of user 
functions.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
def - the name and arguements
.EP
.SP 9 5
expr - the expression itself
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0  on failure

.EP

.SI 3
* %^BOLD%^do_user_function_remove%^RESET%^
.EI
.SI 5
int do_user_function_remove(string name)
.EI
.SP 7 5

This method allows a function to be added to the base set of user 
functions.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
def - the name and arguements
.EP
.SP 9 5
expr - the expression itself
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0  on failure

.EP

.SI 3
* %^BOLD%^query_expression_type%^RESET%^
.EI
.SI 5
string query_expression_type()
.EI
.SP 7 5

This returns the expression type.
It is what is put in front of the
control functions so many objects can set up exciting and confusing
controls of expressions.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the expression type

.EP

.SI 3
* %^BOLD%^set_expression_type%^RESET%^
.EI
.SI 5
void set_expression_type(string type)
.EI


