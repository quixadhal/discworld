.DT
punishment_inherit.c
Disk World autodoc help
punishment_inherit.c

.SH Description
.SP 5 5

This is the basic inheritable punishment class.
.EP
.SP 10 5


Written by Pinkfish

Started Tue Sep 19 14:37:28 PDT 2000

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/nomic_system.h and /include/nroff.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^complete_punishement%^RESET%^
.EI
.SI 5
void complete_punishement(string area, class nomic_case the_case, string type, mixed data)
.EI
.SP 7 5

Complete the punishment, this actually does anythign that really needs to
be done to do the real work.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the case is in
.EP
.SP 9 5
the_case - the case to work on
.EP
.SP 9 5
type - the type of the punishement
.EP
.SP 9 5
data - any extra data to use

.EP

.SI 3
* %^BOLD%^query_description%^RESET%^
.EI
.SI 5
string query_description(string area, string type)
.EI
.SP 7 5

The desscription of the punishement.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the case is in
.EP
.SP 9 5
type - the type name of the punishment
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the description

.EP

.SI 3
* %^BOLD%^query_description_file%^RESET%^
.EI
.SI 5
string query_description_file()
.EI
.SP 7 5

This method returns the description file for the punishment.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the description file

.EP

.SI 3
* %^BOLD%^query_mail_information%^RESET%^
.EI
.SI 5
string query_mail_information(string area, class nomic_case the_case, string type, mixed data)
.EI
.SP 7 5

This method is used to print extra data in the mail message which
gets sent.  This should tell the person how to go about dealing with
whatever has happened to them.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the case is in
.EP
.SP 9 5
the_case - the case to work on
.EP
.SP 9 5
type - the type of the punishment
.EP
.SP 9 5
data - the data associated with the punishment

.EP

.SI 3
* %^BOLD%^query_short%^RESET%^
.EI
.SI 5
string query_short(string area, class nomic_case the_case, string type, mixed data)
.EI
.SP 7 5

The short desscription of the punishement.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the case is in
.EP
.SP 9 5
the_case - the case
.EP
.SP 9 5
type - the type name of the punishment
.EP
.SP 9 5
data - the data associated with the type
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the short description

.EP

.SI 3
* %^BOLD%^set_description_file%^RESET%^
.EI
.SI 5
void set_description_file(string str)
.EI
.SP 7 5

This method sets the description file for this type of punishment.  What it
does and so on.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the description

.EP

.SI 3
* %^BOLD%^set_mail_information%^RESET%^
.EI
.SI 5
void set_mail_information(string mail)
.EI
.SP 7 5

This method sets the mail information to use when sending out a mail for
this punishment.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
info - the mail information

.EP

.SI 3
* %^BOLD%^set_short%^RESET%^
.EI
.SI 5
void set_short(string short)
.EI
.SP 7 5

This method sets the base short description of the punishment.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the base short description

.EP

.SI 3
* %^BOLD%^start_punishment%^RESET%^
.EI
.SI 5
void start_punishment(string area, class nomic_case the_case, string type, function finish)
.EI
.SP 7 5

The start of the punishment stuff, figure out any extra data.  The
default result calls the function back with a 0 data arguement.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the case is in
.EP
.SP 9 5
the_case - the case
.EP
.SP 9 5
type - the type name of the punishment
.EP
.SP 9 5
finish_func - the function to call when we are finished

.EP

.SI 3
* %^BOLD%^suspend_punishment%^RESET%^
.EI
.SI 5
void suspend_punishment(string area, class nomic_case the_case, string type, mixed data)
.EI
.SP 7 5

This method suspends the punishment.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
area - the area the case is in
.EP
.SP 9 5
the_case - the case to work on
.EP
.SP 9 5
type - the type of the punishment
.EP
.SP 9 5
data - the data associated with the punishment

.EP


