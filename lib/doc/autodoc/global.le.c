.DT
le.c
Disk World autodoc help
le.c

.SH Description
.SP 5 5

This is a quick and dirty line editor.  Useful when ed fails
or for a few other other things.


This is a very quick and dirty line editor cooked up by [als] in half an hour on a not-so-stormy night of 10/12/91 It uses NO input_to's, so ALL commands are prefixed with the command word it was simply intended for those times when ed goes down the proverbial gurgler. If you HAVE to use it, set up aliases :) commands: r file, w[file], >insert text, /search text, number goes to line number, +, -, d, N-> clears line editor Have fun, and win awards One other possible use for LE is for cute aliases
.EP
.SP 10 5


Written by Ember


Started 10th of December 1991
.EP

.SH Protected Functions
.SP 5 5
These are functions that only objects inheriting the class can access.
.EP

.SI 3
* %^BOLD%^le%^RESET%^
.EI
.SI 5
int le(string s)
.EI
.SP 7 5

The main access point for le.  This takes in the comamnd
line arg and uses it as it should and does wild and
exciting thins, especially on those stormy  nights...
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the input string to the line editor
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if successful, 0 on failure

.EP

.SI 3
* %^BOLD%^le_commands%^RESET%^
.EI
.SI 5
void le_commands()
.EI
.SP 7 5

This method adds in all the commands needed to access le.

.EP


