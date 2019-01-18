.DT
wiz_info_comm.c
Disk World autodoc help
wiz_info_comm.c

.SH Description
.SP 5 5

This file contains all the information related creator commands.
.EP
.SP 10 5


Written by Ember

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^driver_version%^RESET%^
.EI
.SI 5
int driver_version()
.EI
.SP 7 5

This method is called slighty after the login time.  It checks
to see if the driver version has changed and pooints to the
change log for more information.

.EP


.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^do_gauge%^RESET%^
.EI
.SI 5
int do_gauge(string str)
.EI
.SP 7 5

This method returns the number of evaluation cycles the
particular command took to run.  It is kind of useful for
figuring out how much time a given command takes
to execute
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the command to execute
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 on success, 0 on failure

.EP


