.DT
tokeniser.c
Disk World autodoc help
tokeniser.c

.SH Description
.SP 5 5

Does tokenising the the parser used by the mudlib when someone uses
the ';' command.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/tokenise.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^tokenise%^RESET%^
.EI
.SI 5
mixed tokenise(string inp)
.EI
.SP 7 5

Tokenise, we rip out strings and make them as seperate enties
Otherwise things inside strings will get processed elsewhere
which could be bad...
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
inp - the input string
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the tokenised string

.EP


