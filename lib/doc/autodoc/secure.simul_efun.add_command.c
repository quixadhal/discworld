.DT
add_command.c
Disk World autodoc help
add_command.c

.SH Description
.SP 5 5

Cotains some simul_efuns.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_command%^RESET%^
.EI
.SI 5
varargs void add_command(string verb, string pattern, function func)
.EI
.SP 7 5

A simul_efun for add_command.  This allows you to call add_command without
having to go this_player()->add_command.  This will only work on non-living
objects.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
verb - the verb to add
.EP
.SP 9 5
pattern - the pattern associated with the verb
.EP
.SP 9 5
func - the funvction to call on success

.EP

.SI 3
* %^BOLD%^add_failed_mess%^RESET%^
.EI
.SI 5
void add_failed_mess(mixed mess, object * indir)
.EI
.SP 7 5

An interface to add_failed_mess.  This is added to make life easier,m
you do not need to call this_player()->add_failed_mess() if you are
writing add_command code in a non-living object.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the message to print
.EP
.SP 9 5
indir - the indirect objects the message is for

.EP

.SI 3
* %^BOLD%^add_succeeded_mess%^RESET%^
.EI
.SI 5
void add_succeeded_mess(mixed mess, object * indir)
.EI
.SP 7 5

An interface to add_succeeded_mess.  This is added to make life easier,m
you do not need to call this_player()->add_succeeded_mess() if you are
writing add_command code in a non-living object.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
mess - the message to print
.EP
.SP 9 5
indir - the indirect objects the message is for

.EP

.SI 3
* %^BOLD%^add_succeeded_ob%^RESET%^
.EI
.SI 5
void add_succeeded_ob(object ob)
.EI
.SP 7 5

An interface to add_succeeded.  This is added to make life easier,m
you do not need to call this_player()->add_succeeded() if you are
writing add_command code in a non-living object.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
ob - the object to add as succeeded
.EP


