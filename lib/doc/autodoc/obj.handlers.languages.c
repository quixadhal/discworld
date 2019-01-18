.DT
languages.c
Disk World autodoc help
languages.c

.SH Description
.SP 5 5

The language handler.  This will keep track of the info on languages
Each language can be a written language a spoken language and have
a garble object.


The function garble_say, garble_tell, garble_shout and garble_written should be defined on the garbling object. They should return the entire string to print. So if the routine returns 0 nothing is printed. 
.EP
.SP 10 5


Written by Pinkfish

.EP

.SH Includes
.SP 5 5
This class includes the following files /include/language.h.
.EP

.SH Public Functions
.SP 5 5
These are functions that everyone can access.
.EP

.SI 3
* %^BOLD%^do_replacements%^RESET%^
.EI
.SI 5
string do_replacements(string mess)
.EI
.SP 7 5

Remove "ne1" and suchlike

.EP

.SI 3
* %^BOLD%^fix_shorthand%^RESET%^
.EI
.SI 5
string fix_shorthand(string mess)
.EI

.SI 3
* %^BOLD%^garble_say%^RESET%^
.EI
.SI 5
mixed garble_say(string lang, string start, string mess, object player, object from, string type)
.EI
.SP 7 5

This garbles a say.  Called from the events class to garble the text
that players see.  This looks up the garble object and calls the
correct function on it.  The returned array contains the
garbled start and message.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - the language spoken in
.EP
.SP 9 5
start - the start bit
.EP
.SP 9 5
mess - the message itself
.EP
.SP 9 5
player - the player hearing
.EP
.SP 9 5
from - the player speaking
.EP
.SP 9 5
type - 'say', 'tell', 'shout' etc
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
an array <code>({ start, mess })</code>

.EP

.SI 3
* %^BOLD%^garble_text%^RESET%^
.EI
.SI 5
string garble_text(string lang, mixed text, object thing, object player)
.EI
.SP 7 5

Garble written text.  This is the same as garble_say, but it works on
written text.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - the language written in
.EP
.SP 9 5
text - the text which has been written
.EP
.SP 9 5
thing - the thing which is written on
.EP
.SP 9 5
player - the player doing the reading
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the garbled text as a string

.EP

.SI 3
* %^BOLD%^query_flags%^RESET%^
.EI
.SI 5
int query_flags(string name)
.EI
.SP 7 5

Return all the flags for the language.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the language to get the flags of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
all the flags
.EP

.SI 3
* %^BOLD%^query_garble_object%^RESET%^
.EI
.SI 5
string query_garble_object(string name)
.EI
.SP 7 5

Figure out the garble object.  This returns the garble object for the
language.  The garble object is used to modify the text so that
player does not see plain text if they do not know the language.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
name - the language to get the garble object for
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the garble object

.EP

.SI 3
* %^BOLD%^query_language_always_spoken%^RESET%^
.EI
.SI 5
int query_language_always_spoken(string str)
.EI
.SP 7 5

Determine if the language is always able to be spoken. 

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the language name to test
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the language is spoken, 0 if it is not

.EP

.SI 3
* %^BOLD%^query_language_distance%^RESET%^
.EI
.SI 5
int query_language_distance(string str)
.EI
.SP 7 5

Determine if the language is spoken.
Distance languages can be used with tell and shout.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the language name to test
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is distance language, 0 if it is not

.EP

.SI 3
* %^BOLD%^query_language_magic%^RESET%^
.EI
.SI 5
int query_language_magic(string str)
.EI
.SP 7 5

Determine if it is a magical language.
A magical langage gets a function called on it's object whenever it
is read even if the person knows the language.  It is also the language
spells are written in.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the language to test
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is magical, 0 if it is not

.EP

.SI 3
* %^BOLD%^query_language_size%^RESET%^
.EI
.SI 5
int query_language_size(string str)
.EI
.SP 7 5

The size of the language.  Some languages have bigger letters
and stuff than others.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the language to get the size of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the size of each letter 

.EP

.SI 3
* %^BOLD%^query_language_spoken%^RESET%^
.EI
.SI 5
int query_language_spoken(string str)
.EI
.SP 7 5

Determine if the language is spoken. 

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the language name to test
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the language is spoken, 0 if it is not

.EP

.SI 3
* %^BOLD%^query_language_spoken_skill%^RESET%^
.EI
.SI 5
string query_language_spoken_skill(string lang)
.EI
.SP 7 5

The skill for the spoken language.  This gets the skill name in the skill
tree for the spoken part of the language.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - the language to get the skill of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the skill name

.EP

.SI 3
* %^BOLD%^query_language_written%^RESET%^
.EI
.SI 5
int query_language_written(string str)
.EI
.SP 7 5

Determine if the language is written.

.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the language name to test
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if the language is written, 0 if it is not

.EP

.SI 3
* %^BOLD%^query_language_written_skill%^RESET%^
.EI
.SI 5
string query_language_written_skill(string lang)
.EI
.SP 7 5

The skill for the written language.  This gets the skill name in the skill
tree for the written part of the language.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
lang - the language to get the skill of
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the skill name

.EP

.SI 3
* %^BOLD%^query_languages%^RESET%^
.EI
.SI 5
string * query_languages()
.EI
.SP 7 5

Names of all the languages.
.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
the names of all the languages

.EP

.SI 3
* %^BOLD%^test_language%^RESET%^
.EI
.SI 5
int test_language(string str)
.EI
.SP 7 5

Determine if this is a language.
.EP
.SI 7
%^BOLD%^Parameters:%^RESET%^
.EI
.SP 9 5
str - the language to check

.EP
.SI 7
%^BOLD%^Returns:%^RESET%^
.EI
.SP 9 5
1 if it is a language, 0 if it s not
.EP


