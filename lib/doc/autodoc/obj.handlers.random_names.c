.DT
random_names.c
Disk World autodoc help
random_names.c

.SH Description
.SP 5 5

Generates random names for whatever reason.


Werd - a perl script that generates random words according to some simple rules (inspired by phrase structure trees in Syntax). 

Based on a program by Mark Rosenfelder (markrose@spss.com) called psrGrammar 

Rewritten in perl by Chris Pound (pound@rice.edu) 

Rewritten in lpc by Pinkfish@Discworld MUD. 10th of January 1996. 
.EP
.SP 10 5

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/player_handler.h and /include/random_names.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_test_language%^RESET%^
.EI
.SI 5
void add_test_language(string lang)
.EI
.SP 7 5

This method adds in a language as a test language.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - the test language to add

.EP

.SI 3
* %^BOLD%^open_file%^RESET%^
.EI
.SI 5
void open_file(string f)
.EI
.SP 7 5

Read in a random name grammer from the data dir.   Any line with
# at the start is considered a comment, and anything after
a # on a line is ignored.   Every line in the file will have
3 fields seperated by a :.   The first field is the name
the second is a one letter expansion string, the third is a
a space separeted list of things to expand it to.


The word starts off as a "W", this is looked up in the expansion first string list. If it is found then that is expanded to a random selection of the space seperated data elements. This process is repeated until there are no bits in the word that can be expanded.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
f - the name of the file to read

.EP

.SI 3
* %^BOLD%^query_languages%^RESET%^
.EI
.SI 5
string * query_languages()
.EI
.SP 7 5

The list of all the current languages known.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array of the language names.

.EP

.SI 3
* %^BOLD%^query_test_languages%^RESET%^
.EI
.SI 5
string * query_test_languages()
.EI
.SP 7 5

This is the list of test rules in the handler.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the list of test rules

.EP

.SI 3
* %^BOLD%^random_name%^RESET%^
.EI
.SI 5
string random_name(string lang)
.EI
.SP 7 5

Generate a random name in the given language.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - the language to generate teh name int
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the random name

.EP

.SI 3
* %^BOLD%^remove_test_language%^RESET%^
.EI
.SI 5
void remove_test_language(string lang)
.EI
.SP 7 5

This method removes a language as a test language.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - the test language to remove

.EP

.SI 3
* %^BOLD%^unique_name%^RESET%^
.EI
.SI 5
string unique_name(string lang)
.EI
.SP 7 5

Return a name that is not banished or a player name already.
It is > 2 and > 11 chars, it is not banished, a player or
in the game.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - the language to generat ethe name in
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the generated name

.EP


