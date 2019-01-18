.DT
english_handler.c
Disk World autodoc help
english_handler.c

.SH Description
.SP 5 5


English Handler
.EP
.SP 10 5


Written by Shaggy

This handler is a collection of methods that attempt to evaluate how much
like english a line of text is.  Some tests might be more expensive than
others but the general idea is to be much cheaper than a dictionary lookup.

The general principle it to return a value that is closer to 100 the better
the english with 50 being borderline (ie. likely to be gibberish).
The returned value is thus a bit like a percentage chance that it is
english.

.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^FreqTest%^RESET%^
.EI
.SI 5
int FreqTest(string line)
.EI
.SP 7 5

FreqTest is a routine that evaluates how like english a line of text is.  It
compares the frequency of the letters, spaces and non-letters to the typical
number in English (taken from the am board archive).  A typical sentence
should give a percentage change of being english in the range 50-100.
Short or unusual sentences can get borderline evaluations (>60) even if they
are english.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
line - line of text to evaluate
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
percentage chance of being english 50-100 ok < 50 might be gibberish 

.EP

.SI 3
* %^BOLD%^ProfaneTest%^RESET%^
.EI
.SI 5
int ProfaneTest(string strin)
.EI
.SP 7 5

ProfaneTest is a routine that evaluates how likely the text is to
be profane/swearing/obscenity and so on.  These are the sorts of
things that would never be allowed as a player name.  Spaces, non
letters and repeating letters are worked around by this test.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
line - line of text to evaluate
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
percentage chance of being offensive: chances over 100% can be returned
        indicating greater likelihood of offense and/or more than one suspect word.
        If you want it as lily white as possible you should insist on 0.

.EP


