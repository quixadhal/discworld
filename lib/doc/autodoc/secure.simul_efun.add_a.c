.DT
add_a.c
Disk World autodoc help
add_a.c

.SH Description
.SP 5 5

Contains some simul_efuns.
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^add_a%^RESET%^
.EI
.SI 5
string add_a(string s)
.EI
.SP 7 5

Adds an 'a' or 'an' to a string.  Determins if a 'a' or 'an' should be added
based on the ending of the word.  If you are doing this on an object you
should probably use a_short or the_short on the object.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
s - the string to add an a to

.EP

.SI 3
* %^BOLD%^vowel%^RESET%^
.EI
.SI 5
int vowel(int i)
.EI
.SP 7 5

Determine if the input is a vowel.  Is the input integer a vowel?

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
i - the int to check for vowelness
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the int is a vowel, 0 if not

.EP


